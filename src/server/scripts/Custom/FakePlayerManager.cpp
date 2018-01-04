#include "FakePlayerManager.h"
#include "QueryCallback.h"
#include "World.h"
#include "ScriptMgr.h"
#include "AccountMgr.h"
#include "DatabaseEnv.h"
#include "Log.h"
#include "GameTime.h"
#include "WorldSession.h"
#include "Object.h"
#include "ObjectMgr.h"
#include <cmath>
#include <algorithm>

enum UpdateEvents
{
    UPDATE_PLAYER_COUNT = 1,
    UPDATE_PLAYERS,
    UPDATE_ALLOWED_TO_LOGIN
};

uint32 urand_seconds(uint32 min, uint32 max)
{
    return urand(min * IN_MILLISECONDS, max * IN_MILLISECONDS);
}

uint32 urand_minutes(uint32 min, uint32 max)
{
    return urand(min * MINUTE * IN_MILLISECONDS, max * MINUTE * IN_MILLISECONDS);
}

template<typename Format, typename... Args>
std::string FormatString(Format&& sql, Args&&... args)
{
    if (Trinity::IsFormatEmptyOrNull(sql))
        return "";

    return Trinity::StringFormat(std::forward<Format>(sql), std::forward<Args>(args)...);
}

WorldSession* getSession(uint32 accountId, const std::string& accountName)
{
    auto session = sWorld->FindSession(accountId);

    if (session != nullptr)
        return session;

    std::string accName = accountName;
    if (accName.empty())
        if (!sAccountMgr->GetName(accountId, accName))
            return nullptr;

    session = new WorldSession(accountId, std::move(accName), nullptr, SEC_PLAYER, 2, 0, LOCALE_enUS, 0, false);
    session->SetFake(true);

    sWorld->AddSession(session);
    return session;
}

uint32 createKey(uint8 n1, uint8 n2, uint8 n3, uint8 n4)
{
    uint32 value = 0;
    uint8* valuePtr = (uint8*)&value;

    valuePtr[0] = n1;
    valuePtr[1] = n2;
    valuePtr[2] = n3;
    valuePtr[3] = n4;

    return value;
}

void fillVisualValue(uint8 race, uint8 Class, uint8 gender, std::unordered_map<uint32, std::vector<uint8>>& values)
{
    for (uint8 v1 = 0; v1 < 15; v1++)
    {
        for (uint8 v2 = 0; v2 < 15; v2++)
        {
            for (uint8 v3 = 0; v3 < 15; v3++)
            {
                for (uint8 v4 = 0; v4 < 15; v4++)
                {
                    for (uint8 v5 = 0; v5 < 15; v5++)
                    {
                        if (Player::ValidateAppearance(race, Class, gender, v1, v2, v3, v4, v5, true))
                        {
                            values[createKey(race, Class, gender, 0)] = std::vector<uint8>{ v1, v2, v3, v4, v5, 0 };
                            return;
                        }
                    }
                }
            }
        }
    }
}

void buildCharVisual(WorldPacket& pkt, uint8 race, uint8 chrClass, uint8 gender)
{
    static std::unordered_map<uint32, std::vector<uint8>> values;

    if (values.empty())
    {
        std::vector<uint8> races{ 1, 2, 3, 4, 5, 6, 7, 8, 10, 11 };
        std::vector<uint8> classes{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 11 };

        for (auto rItr = races.begin(); rItr != races.end(); rItr++)
        {
            uint8 vRace = *rItr;

            for (auto cItr = classes.begin(); cItr != classes.end(); cItr++)
            {
                uint8 vClass = *cItr;

                for (uint8 vGender = 0; vGender < 2; vGender++)
                    fillVisualValue(vRace, vClass, vGender, values);
            }
        }
    }

    auto itr = values.find(createKey(race, chrClass, gender, 0));

    if (itr != values.end())
    {
        auto& value = itr->second;

        for (auto it = value.begin(); it != value.end(); it++)
            pkt << *it;
    }
}

void buildCharCreate(WorldPacket& pkt, std::string name, uint8 race, uint8 chrClass, uint8 gender)
{
    pkt << name << race << chrClass << gender;
    buildCharVisual(pkt, race, chrClass, gender);
}

// Class members

FakePlayerMgr* FakePlayerMgr::instance()
{
    static FakePlayerMgr mgr;

    return &mgr;
}

FakePlayerMgr::FakePlayerMgr()
{
    _requiredCount = 0;
    _events.ScheduleEvent(UPDATE_PLAYER_COUNT, 1);

    _events.ScheduleEvent(UPDATE_PLAYERS, 1000);
    _events.ScheduleEvent(UPDATE_ALLOWED_TO_LOGIN, urand_seconds(sWorld->getIntConfig(CONFIG_FAKE_PLAYER_UPDATE_ALLOWED_TIME_MIN),
        sWorld->getIntConfig(CONFIG_FAKE_PLAYER_UPDATE_ALLOWED_TIME_MAX)));
}

void FakePlayerMgr::LoadNames()
{
    _usedNames.clear();
    _unusedNames.clear();

    auto result = CharacterDatabase.Query("SELECT name, used FROM fake_player_names");

    _usedNames.reserve(result->GetRowCount() * 2);
    _unusedNames.reserve(result->GetRowCount());

    if (result)
    {
        do
        {
            auto fields = result->Fetch();

            if (fields[1].GetBool())
            {
                auto name = fields[0].GetString();

                if (utf8length(name) < MAX_PLAYER_NAME)
                    _usedNames.push_back(name);
            }
            else
                _unusedNames.push_back(result->Fetch()->GetString());

        } while (result->NextRow());
    }

    TC_LOG_INFO("custom.fake_players", "Loaded %u unused and %u used fake player names", _unusedNames.size(), _usedNames.size());
}

void FakePlayerMgr::LoadZones()
{
    std::vector<ZoneInfo*> zones;

    zones.push_back(new ZoneInfo(10, 20, 1, 769.831f, -3302.27f, 238.464f, 0.0735538f, 2 | 32 | 128)); // Barrens, 10-20, orc | tauren | throll
    zones.push_back(new ZoneInfo(26, 40, 1, -5366.85f, -1935.85f, 90.1991f, 2.1631f)); // Thousand needles
    zones.push_back(new ZoneInfo(41, 50, 1, -7308.58f, -3720.69f, 10.6035f, 2.97676f)); // Tanaris
    zones.push_back(new ZoneInfo(51, 54, 1, -7113.57f, -1665.88f, -266.46f, 1.16455f)); // Ungoro
    zones.push_back(new ZoneInfo(56, 60, 1, -6735.95f, 2093.25f, 97.5524f, 0.890451f)); // Silithus
    zones.push_back(new ZoneInfo(40, 50, 1, -5634.03f, 1124.07f, 64.7162f, 5.9292f)); // Feralas
    zones.push_back(new ZoneInfo(30, 40, 1, -688.708f, 1312.31f, 90.2789f, 0.823321f)); // Decolace
    zones.push_back(new ZoneInfo(20, 25, 1, 2513.66f, -168.389f, 225.72f, 5.00401f)); // Ashenvale
    zones.push_back(new ZoneInfo(10, 20, 1, 6241.44f, 225.498f, 43.8216f, 6.19654f, RACEMASK_ALLIANCE)); // Darkshore
    zones.push_back(new ZoneInfo(50, 55, 1, 5707.65f, -484.17f, 419.006f, 0.481544f)); // Felwood
    zones.push_back(new ZoneInfo(53, 58, 1, 5896.42f, -4632.48f, 747.628f, 3.25086f, RACEMASK_ALLIANCE)); // Winterspring
    zones.push_back(new ZoneInfo(45, 55, 1, 3673.96f, -4093.64f, 133.8f, 0.0880871f)); // Ashara
    zones.push_back(new ZoneInfo(10, 20, 530, -2200.12f, -11238.9f, 136.686f, 2.96567f, 1024)); // Bloodmyst isle
    zones.push_back(new ZoneInfo(10, 20, 530, 6691.96f, -6037.01f, 183.172f, 3.29084f, 512)); // Ghostlands
    zones.push_back(new ZoneInfo(10, 20, 0, 377.219f, 907.076f, 193.014f, 0.275389f, 16)); // Silverphine forest
    zones.push_back(new ZoneInfo(20, 30, 0, -367.395f, 410.899f, 292.549f, 1.93145f)); // Hilsbrad
    zones.push_back(new ZoneInfo(30, 40, 0, -1957.44f, -3478.23f, 204.25f, 3.98686f)); // Arathi
    zones.push_back(new ZoneInfo(40, 50, 0, 264.289f, -3249.3f, 133.458f, 0.0661511f)); // Hinterlands
    zones.push_back(new ZoneInfo(50, 55, 0, 2212.67f, -2129.56f, 285.942f, 5.05624f)); // Western plaguelands
    zones.push_back(new ZoneInfo(50, 60, 0, 2752.19f, -5103.77f, 161.793f, 4.91501f)); // Eastern plaguelands
    zones.push_back(new ZoneInfo(20, 30, 0, -3607.23f, -3329.05f, 127.594f, 3.87278f, 4 | 64)); // Wetlands, gnome & dwarf
    zones.push_back(new ZoneInfo(10, 20, 0, -5212.93f, -3960.36f, 338.949f, 3.87748f, 4 | 64)); // Loch modan, gnome & dwarf
    zones.push_back(new ZoneInfo(40, 50, 0, -7340.46f, -2366.37f, 337.282f, 0.165691f)); // Badlands
    zones.push_back(new ZoneInfo(45, 55, 0, -6391.94f, -1727.08f, 394.107f, 0.918369f)); // Searing gorge
    zones.push_back(new ZoneInfo(50, 60, 0, -7887.24f, -2678.46f, 253.857f, 0.437206f)); // Burning steppes
    zones.push_back(new ZoneInfo(20, 25, 0, -9352.28f, -3282.53f, 93.0474f, 3.94794f, 1)); // Redridge, human
    zones.push_back(new ZoneInfo(10, 20, 0, -10412.2f, 2022.97f, 35.436f, 2.37242f, 1)); // Elwyn or something like this, human
    zones.push_back(new ZoneInfo(20, 30, 0, -10915.1f, 59.9301f, 61.7775f, 0.0169992f, 1)); // Duskwood, human
    zones.push_back(new ZoneInfo(30, 45, 0, -14147.5f, 61.6136f, 91.1086f, 4.81448f, RACEMASK_ALLIANCE)); // Stranglethorn valley
    zones.push_back(new ZoneInfo(35, 45, 0, -14147.5f, 61.6136f, 91.1086f, 4.81448f, RACEMASK_HORDE)); // Stranglethorn valley
    zones.push_back(new ZoneInfo(40, 50, 0, -10453, -4311.63f, 30.1767f, 3.17381f)); // Swamp of sorrows
    zones.push_back(new ZoneInfo(50, 60, 0, -11320.1f, -2457.83f, 197.884f, 1.76323f)); // Blasted lands

                                                                                        // Burning crusade
    zones.push_back(new ZoneInfo(58, 62, 530, -644.854f, 4575.59f, 247.604f, 3.77934f));
    zones.push_back(new ZoneInfo(61, 66, 530, 224.277f, 7812.15f, 133.486f, 4.70416f));
    zones.push_back(new ZoneInfo(65, 68, 530, -3123.34f, 6335.11f, 307.076f, 3.7464f));
    zones.push_back(new ZoneInfo(62, 67, 530, -2589.51f, 4812.16f, 131.183f, 5.08368f));
    zones.push_back(new ZoneInfo(67, 70, 530, -4206, 2318.82f, 255.307f, 2.90577f));
    zones.push_back(new ZoneInfo(65, 68, 530, 1946.23f, 4876.14f, 166.425f, 2.20048f));
    zones.push_back(new ZoneInfo(67, 70, 530, 3439.84f, 3222.25f, 109.152f, 5.22976f));

    // WOTLK
    zones.push_back(new ZoneInfo(68, 71, 571, 3936.3f, 4306.17f, 40.2394f, 3.74639f));
    zones.push_back(new ZoneInfo(72, 75, 571, 3516.55f, -643.448f, 420.944f, 1.65331f));
    zones.push_back(new ZoneInfo(75, 78, 571, 5821.94f, 5228.87f, -83.8345f, 6.28008f));
    zones.push_back(new ZoneInfo(68, 71, 571, 1754.74f, -3439.39f, 135.742f, 3.35492f, RACEMASK_ALLIANCE));
    zones.push_back(new ZoneInfo(72, 75, 571, 3590.06f, -2923.73f, 243.422f, 5.89648f, RACEMASK_ALLIANCE));
    zones.push_back(new ZoneInfo(75, 78, 571, 6289.87f, -2267.52f, 380.432f, 5.00741f));
    zones.push_back(new ZoneInfo(78, 79, 571, 7011.54f, -1259.03f, 1102.05f, 2.34939f));
    zones.push_back(new ZoneInfo(77, 79, 571, 7653.53f, 932.646f, 492.2f, 1.08688f));

    // Max level
    //zones.push_back(new ZoneInfo(80, 80, 571, 5800.55f, 813.463f, 1081.06f, 1.5968f));
    zones.push_back(new ZoneInfo(80, 80, 1, 1562.54f, -4322.27f, 81.0706f, 4.29059f, RACEMASK_HORDE));
    zones.push_back(new ZoneInfo(80, 80, 0, -8706.71f, 323.356f, 59.457f, 2.99502f, RACEMASK_ALLIANCE));
    zones.push_back(new ZoneInfo(80, 80, 631, 8.718676f, 2212.940918f, 30.0f, 3.157448f)); // Icecrown citadel

    for (uint32 id = 1; id <= zones.size(); id++)
    {
        auto zone = zones[id - 1];
        zone->id = id;

        _zones[id] = zone;
    }
}

void FakePlayerMgr::LoadPlayers()
{
    if (!_players.Empty())
    {
        TC_LOG_ERROR("custom.fake_players", "Trying to reload fake players");
        return;
    }

    static const uint32 flags = PLAYER_FLAGS_UNK20 | PLAYER_FLAGS_UNK21;

    auto result = CharacterDatabase.PQuery("SELECT `guid`, `account`, `logout_time` FROM `characters` WHERE (`playerFlags` & %u) = %u", flags, flags);

    if (result)
    {
        uint32 size = result->GetRowCount() * 2;

        _players.Reserve(size);

        do
        {
            auto fields = result->Fetch();
            auto player = new FakePlayer();
            player->guid = ObjectGuid(HighGuid::Player, fields[0].GetUInt32());
            player->accountId = fields[1].GetUInt32();
            player->logoutTime = 0;
            player->SetAllowed((player->logoutTime + GetAllowedLoginTime()) >= GameTime::GetGameTime());

            _players.Add(player, false);

        } while (result->NextRow());
    }
    else
        _players.Reserve(10000);

    TC_LOG_INFO("custom.fake_players", "Loaded %u fake players", _players.Size());
}

std::vector<uint8> &FakePlayerMgr::GetAllowedClasses(uint8 race, bool allowHeroic) const
{
    static std::unordered_map<uint8, std::vector<uint8>> allowedMap;
    static std::unordered_map<uint8, std::vector<uint8>> allowedNoHeroicMap;
    static std::vector<uint8> emptyVector;

    if (allowedMap.empty())
    {
        std::vector<uint8> races{ 1, 2, 3, 4, 5, 6, 7, 8, 10, 11 };
        std::vector<uint8> classes{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 11 };

        for (auto itr = races.begin(); itr != races.end(); itr++)
        {
            std::vector<uint8> values;
            std::vector<uint8> noHeroicValues;

            for (auto itr2 = classes.begin(); itr2 != classes.end(); itr2++)
            {
                if (sObjectMgr->GetPlayerInfo(*itr, *itr2))
                {
                    values.push_back(*itr2);

                    if ((*itr2) != CLASS_DEATH_KNIGHT)
                        noHeroicValues.push_back(*itr2);
                }
            }

            allowedMap[*itr] = values;
            allowedNoHeroicMap[*itr] = noHeroicValues;
        }
    }

    auto &container = allowHeroic ? allowedMap : allowedNoHeroicMap;

    auto itr = container.find(race);
    if (itr == container.end())
        return emptyVector;

    return itr->second;
}

std::vector<ZoneInfo*> FakePlayerMgr::GetZones(uint8 level, uint32 raceMask) const
{
    std::vector<ZoneInfo*> result;

    for (auto itr = _zones.begin(); itr != _zones.end(); itr++)
    {
        auto zone = itr->second;

        if (level < zone->minLevel || level > zone->maxLevel)
            continue;

        if (zone->raceMask & raceMask)
            result.push_back(zone);
    }

    return result;
}

std::string FakePlayerMgr::GetRandomName(bool setAsUsed)
{
    std::string name;

    if (!_unusedNames.empty())
    {
        uint32 idx = urand(0, _unusedNames.size() - 1);
        name = _unusedNames[idx];
        _unusedNames.erase(_unusedNames.begin() + idx);
    }
    else if (!_usedNames.empty())
    {
        uint32 idx = urand(0, _usedNames.size() - 1);
        name = _usedNames[idx];
        _usedNames.erase(_usedNames.begin() + idx);

        name += name[urand(0, name.size() - 1)];
    }

    uint32 len = utf8length(name);
    if (setAsUsed && !name.empty() && len < MAX_PLAYER_NAME)
    {
        _usedNames.push_back(name);
        CharacterDatabase.PExecute("REPLACE INTO `fake_player_names` (`name`, `used`) VALUES ('%s', 1)", name.c_str());
    }

    return name;
}

ZoneInfo* FakePlayerMgr::GetZone(uint32 id) const
{
    auto itr = _zones.find(id);

    if (itr != _zones.end())
        return itr->second;

    return nullptr;
}

ZoneInfo* FakePlayerMgr::GetRandomZone(uint8 level, uint32 raceMask) const
{
    const auto &zones = GetZones(level, raceMask);

    if (zones.empty())
        return nullptr;

    if (zones.size() == 1)
        return zones[0];

    uint32 weight = 0;
    std::vector<std::pair<uint32, ZoneInfo*>> map;

    for (auto zone : zones)
    {
        weight += weight;
        map.push_back(std::make_pair(weight, zone));
    }

    uint32 val = urand(0, weight);
    ZoneInfo* last = nullptr;

    for (uint32 i = 0; i < map.size(); i++)
    {
        auto& pair = map[i];

        if (val <= pair.first)
            return pair.second;

        last = pair.second;
    }

    return last;
}

uint8 FakePlayerMgr::GetRandomRace() const
{
    static std::vector<uint8> races{ 1, 2, 3, 4, 5, 6, 7, 8, 10, 11 };

    return races[urand(0, races.size() - 1)];
}

uint8 FakePlayerMgr::GetRandomClass(uint8 race, bool allowHeroic) const
{
    auto& classes = GetAllowedClasses(race, allowHeroic);

    return classes[urand(0, classes.size() - 1)];
}

void FakePlayerMgr::Create(std::string& charName, uint8 race, uint8 Class, uint8 gender, std::function<void(FakePlayer *)> callback)
{
    std::string email;
    std::string accountName = std::string("fp_") + std::to_string(urand(10000000, 4000000000));
    std::string password = std::to_string(urand(10000000, 4000000000));

    Utf8ToUpperOnlyLatin(accountName);
    Utf8ToUpperOnlyLatin(password);

    if (sAccountMgr->GetId(accountName))
    {
        TC_LOG_ERROR("custom.fake_players", "Trying to create account which already exists");
        return;
    }

    _queryProcessor.AddQuery(LoginDatabase.AsyncQuery(FormatString("INSERT INTO account(username, sha_pass_hash, reg_mail, email, joindate, locked) VALUES('%s', '%s', '', '', NOW(), 255)", accountName.c_str(), sAccountMgr->CalculateShaPassHash(accountName, password).c_str()).c_str())
        .WithCallback([this, accountName, charName, callback, race, Class, gender](QueryResult)
    {
        uint32 accountId = sAccountMgr->GetId(accountName);

        if (!accountId)
        {
            TC_LOG_ERROR("custom.fake_players", "Account created, but can't get ID");
            return;
        }

        auto session = getSession(accountId, accountName);
        if (!session)
        {
            TC_LOG_ERROR("custom.fake_players", "Account created, but can't get session");
            return;
        }

        session->CatchPacket(SMSG_CHAR_CREATE, [this, session, callback, accountId](WorldPacket& resultPkt)
        {
            uint8 result;
            resultPkt >> result;

            if (result != CHAR_CREATE_SUCCESS)
            {
                TC_LOG_ERROR("custom.fake_players", "Failed to create fake character. Result: %u", result);
                session->KickPlayer();
            }
            else
            {
                auto fakePlayer = new FakePlayer();
                fakePlayer->guid = session->LastCreatedCharGuid;
                fakePlayer->accountId = accountId;
                fakePlayer->session = session;
                fakePlayer->SetAllowed(true);

                _players.Add(fakePlayer, true);

                callback(fakePlayer);
            }

            return true;
        });

        WorldPacket *pkt = new WorldPacket(CMSG_CHAR_CREATE);
        buildCharCreate(*pkt, charName, race, Class, gender);
        session->QueuePacket(pkt);
    }));
}

void FakePlayerMgr::Login(uint32 accountId, std::function<void(FakePlayer*)> callback)
{
    _players.Get(accountId, [this, callback](FakePlayer* player) 
    {
        Login(player, callback);
    });
}

void FakePlayerMgr::Login(FakePlayer * player, std::function<void(FakePlayer*)> callback)
{
    if (!player->session)
        player->session = getSession(player->accountId, std::string());

    if (!player->session)
    {
        TC_LOG_ERROR("custom.fake_players", "FakePlayer::Login failed, can't acquire session");
        return;
    }

    if (player->session->PlayerLoading() || player->session->GetPlayer())
    {
        return;
    }

    player->session->CatchPacket(SMSG_LOGIN_VERIFY_WORLD, [callback, player](WorldPacket&)
    {
        callback(player);
        return true;
    });

    WorldPacket *pktPtr = new WorldPacket(CMSG_PLAYER_LOGIN, 32);
    WorldPacket& pkt = *pktPtr;
    pkt << player->guid;

    player->session->QueuePacket(pktPtr);
}

void FakePlayerMgr::OnLogin(Player* player)
{
    FakePlayer* fakePlayer = nullptr;

    _players.Get(player->GetSession()->GetAccountId(), [player, &fakePlayer](FakePlayer* plr)
    {
        plr->player = player;
        plr->session = player->GetSession();
        fakePlayer = plr;
    });

    if (!fakePlayer)
        return;

    player->SetVisible(false);
    player->SetGrantableLevels(STRONG_MAX_LEVEL);
    player->SetFlag(PLAYER_FLAGS, PLAYER_FLAGS_UNK20 | PLAYER_FLAGS_UNK21);

    if (player->getClass() == CLASS_DEATH_KNIGHT && !player->HasSpell(50977))
        player->LearnSpell(50977, true);

    auto session = player->GetSession();

    // Join channel

    WorldPacket* pktPtr = new WorldPacket(CMSG_JOIN_CHANNEL);
    WorldPacket& pkt = *pktPtr;

    pkt << (uint32)0 << (uint8)0 << (uint8)0 << "all" << "";

    session->QueuePacket(pktPtr);

    // Events

    player->ScheludeEvent(LEVEL_UP, GetEventTime(LEVEL_UP, player), [this, player]()
    {
        if (player->getLevel() < sWorld->getIntConfig(CONFIG_MAX_PLAYER_LEVEL))
            player->GiveLevel(player->getLevel() + 1);

        return GetEventTime(LEVEL_UP, player);
    });

    player->ScheludeEvent(ZONE_CHANGE, GetEventTime(ZONE_CHANGE, player), [this, player, fakePlayer, session]()
    {
        uint8 level = player->getLevel();
        if (player->getClass() == CLASS_DEATH_KNIGHT && level < 60)
            return GetEventTime(ZONE_CHANGE, player);

        auto currentZone = fakePlayer->zoneId != 0 ? GetZone(fakePlayer->zoneId) : nullptr;
        if (currentZone && level < sWorld->getIntConfig(CONFIG_MAX_PLAYER_LEVEL) && currentZone->minLevel == level)
            return GetEventTime(ZONE_CHANGE, player);

        auto const& zones = GetZones(player->getLevel(), player->getRaceMask());

        if (zones.empty())
            return GetEventTime(ZONE_CHANGE, player);

        std::vector<ZoneInfo*> possibleZones;
        bool changeZoneIfNewAvailable = urand(1, 100) <= 50;
        bool forceZoneChange = !currentZone || currentZone->maxLevel < level;

        for (auto itr = zones.begin(); itr != zones.end(); itr++)
        {
            auto zone = *itr;

            if (forceZoneChange || (zone->minLevel == level && changeZoneIfNewAvailable))
                possibleZones.push_back(zone);
        }

        if (possibleZones.empty())
            return GetEventTime(ZONE_CHANGE, player);

        auto zone = possibleZones[urand(0, possibleZones.size() - 1)];
        fakePlayer->zoneId = zone->id;

        uint32 mapId = player->GetMapId();
        player->TeleportTo(zone->mapId, zone->x, zone->y, zone->z, zone->o);

        if (mapId != zone->mapId)
        {
            WorldPacket* pkt = new WorldPacket(MSG_MOVE_WORLDPORT_ACK, 16);
            session->QueuePacket(pkt);
        }

        return GetEventTime(ZONE_CHANGE, player);
    });

    player->ScheludeEvent(LOGOUT, GetEventTime(LOGOUT, player), [session, player]()
    {
        session->KickPlayer();
        player->RemoveAllEvents();
        return 0;
    });
}

void FakePlayerMgr::OnLogout(Player* player)
{
    _players.Get(player->GetSession()->GetAccountId(), [](FakePlayer* fakePlayer) 
    {
        fakePlayer->player = nullptr;
        fakePlayer->session = nullptr;
    });
}

uint32 FakePlayerMgr::GetEventTime(FakePlayerEvents eventId, Player * player) const
{
    if (eventId == LEVEL_UP)
    {
        uint32 max = sWorld->getIntConfig(CONFIG_MAX_PLAYER_LEVEL);
        if (player->getLevel() >= max)
            return 0;

        uint32 maxLevelTime = sWorld->getIntConfig(CONFIG_FAKE_PLAYER_MAX_LEVEL_TIME) * MINUTE;
        uint32 minLevelTime = sWorld->getIntConfig(CONFIG_FAKE_PLAYER_MIN_LEVEL_TIME) * MINUTE;
        uint32 levelTime = (maxLevelTime / (float)max) * player->getLevel();

        if (levelTime < minLevelTime)
            levelTime = minLevelTime;

        levelTime /= (sWorld->getRate(RATE_XP_KILL) + sWorld->getRate(RATE_XP_QUEST)) / 2.0f;
        levelTime *= sWorld->getFloatConfig(CONFIG_FAKE_PLAYER_LEVELUP_MOD);
        if (levelTime < 1)
            levelTime = 1;

        levelTime *= IN_MILLISECONDS;

        return urand(levelTime, levelTime * 1.5f);
    }
    else if (eventId == ZONE_CHANGE)
    {
        return urand_minutes(sWorld->getIntConfig(CONFIG_FAKE_PLAYER_ZONE_CHANGE_TIME_MIN), 
            sWorld->getIntConfig(CONFIG_FAKE_PLAYER_ZONE_CHANGE_TIME_MIN));
    }
    else if (eventId == LOGOUT)
    {
        return urand_minutes(sWorld->getIntConfig(CONFIG_FAKE_PLAYER_LOGOUT_TIME_MIN),
            sWorld->getIntConfig(CONFIG_FAKE_PLAYER_LOGOUT_TIME_MAX));
    }

    return 0;
}

uint32 FakePlayerMgr::GetAllowedLoginTime() const
{
    return urand_minutes(sWorld->getIntConfig(CONFIG_FAKE_PLAYER_ALLOWED_RELOGIN_TIME_MIN), 
        sWorld->getIntConfig(CONFIG_FAKE_PLAYER_ALLOWED_RELOGIN_TIME_MAX));
}

void FakePlayerMgr::Update(uint32 diff)
{
    _queryProcessor.ProcessReadyQueries();
    _events.Update(diff);

    while (uint32 eventId = _events.ExecuteEvent())
    {
        uint32 eventTime = 0;

        if (eventId == UPDATE_PLAYER_COUNT)
        {
            _requiredCount = urand(sWorld->getIntConfig(CONFIG_FAKE_PLAYER_MIN_COUNT),
                sWorld->getIntConfig(CONFIG_FAKE_PLAYER_MAX_COUNT));

            eventTime = urand_seconds(sWorld->getIntConfig(CONFIG_FAKE_PLAYER_UPDATE_COUNT_TIME_MIN),
                sWorld->getIntConfig(CONFIG_FAKE_PLAYER_UPDATE_COUNT_TIME_MAX));

            const auto timestamp = GameTime::GetGameTime();
            auto timeStruct = *localtime(&timestamp);
            
            uint32 hourValue = _hourValues[timeStruct.tm_hour];
            _requiredCount *= hourValue / 100.0f;

            uint32 uptime = GameTime::GetUptime();
            uint32 fillTime = sWorld->getIntConfig(CONFIG_FAKE_PLAYER_FILL_TIME);
            uint32 burstTime = sWorld->getIntConfig(CONFIG_FAKE_PLAYER_BURST_TIME);
            float burstValue = sWorld->getIntConfig(CONFIG_FAKE_PLAYER_BURST_VALUE) / 100.0f;
            float mod = 1.0f;

            if (burstTime && burstValue >= 0.01 && uptime < burstTime)
            {
                mod = (burstValue / (float)burstTime) * (burstTime - (burstTime - uptime));

                eventTime = 1000;
            }

            if (fillTime && uptime < fillTime)
            {
                float minMod = mod >= 1.0f ? (burstTime && burstValue ? burstValue / 100.0f : 0.01) : mod;

                mod = uptime / (float)fillTime;

                if (mod < minMod)
                    mod = minMod;

                eventTime = 1000;
            }

            if (mod < 0.01f)
                mod = 0.01f;

            _requiredCount *= mod;
        }
        else if (eventId == UPDATE_PLAYERS)
        {
            
        }
        else if (eventId == UPDATE_ALLOWED_TO_LOGIN)
        {
            eventTime = urand_seconds(sWorld->getIntConfig(CONFIG_FAKE_PLAYER_UPDATE_ALLOWED_TIME_MIN),
                sWorld->getIntConfig(CONFIG_FAKE_PLAYER_UPDATE_ALLOWED_TIME_MAX));

            _players.Do([this](FakePlayerContainer::PlayerMap players) 
            {
                for (auto itr = players.begin(); itr != players.end(); itr++)
                {
                    if (auto player = itr->second)
                    {
                        if (player->session || GameTime::GetGameTime() < (itr->second->logoutTime + GetAllowedLoginTime()))
                            player->SetAllowed(false);
                        else
                            player->SetAllowed(true);
                    }
                }
            });
        }

        if (eventTime > 0)
            _events.ScheduleEvent(eventId, eventTime);
    }

    // Special case: Update player count
    static uint32 processTime = urand_seconds(sWorld->getIntConfig(CONFIG_FAKE_PLAYER_UPDATE_PLAYERS_TIME_MIN),
        sWorld->getIntConfig(CONFIG_FAKE_PLAYER_UPDATE_PLAYERS_TIME_MAX));

    static uint32 toProcess = urand(sWorld->getIntConfig(CONFIG_FAKE_PLAYER_PER_UPDATE_MIN),
        sWorld->getIntConfig(CONFIG_FAKE_PLAYER_PER_UPDATE_MAX));

    static uint32 toProcessStatic = (uint32)toProcess;
    static float counter = 0.0f;

    float playersPerMs = (toProcessStatic / (float)processTime);
    counter += playersPerMs * diff;

    uint32 count = std::min((uint32)counter, toProcess);
    if (count < 1)
        return;

    toProcess -= count;
    counter -= count;

    if (toProcess < 1)
    {
        processTime = urand_seconds(sWorld->getIntConfig(CONFIG_FAKE_PLAYER_UPDATE_PLAYERS_TIME_MIN),
            sWorld->getIntConfig(CONFIG_FAKE_PLAYER_UPDATE_PLAYERS_TIME_MAX));

        toProcess = urand(sWorld->getIntConfig(CONFIG_FAKE_PLAYER_PER_UPDATE_MIN),
            sWorld->getIntConfig(CONFIG_FAKE_PLAYER_PER_UPDATE_MAX));

        toProcessStatic = (uint32)toProcess;
        counter = 0;
    }

    _players.Do([this, count](FakePlayerContainer::PlayerMap& all)
    {
        uint32 online = 0;

        for (auto itr = all.begin(); itr != all.end(); itr++)
        {
            if (itr->second->IsOnline())
                online++;
        }

        if (online < _requiredCount)
        {
            auto players = _players.GetRandom(count, [](FakePlayer* p) { return p->IsAllowed(); });

            for (auto itr = players.begin(); itr != players.end(); itr++)
                Login(*itr, [](FakePlayer*) {});

            if (count > players.size())
            {
                for (uint32 i = players.size(); i <= count; i++)
                {
                    std::string name = GetRandomName(true);
                    if (name.empty())
                    {
                        TC_LOG_ERROR("custom.fake_players", "Can't get player name, no names left?");
                        break;
                    }

                    uint8 race = GetRandomRace();
                    uint8 Class = GetRandomClass(race, true); // TODO: Allow heroic only if has many high level players
                    uint8 gender = urand(0, 1);

                    Create(name, race, Class, gender, [this](FakePlayer* player)
                    {
                        player->SetAllowed(false);

                        Login(player->accountId, [](FakePlayer* player)
                        {
                            TC_LOG_INFO("custom.fake_players", "Login success");
                        });
                    });
                }
            }
        }
        else if (online > _requiredCount)
        {
            auto players = _players.GetRandom(count, [](FakePlayer* p) { return p->IsOnline() && p->IsInGame(); });

            for (auto itr = players.begin(); itr != players.end(); itr++)
                (*itr)->session->KickPlayer();
        }
    });
}

size_t FakePlayerContainer::Size()
{
    return _all.size();
}

void FakePlayerContainer::Do(std::function<void(PlayerMap&)> func)
{
    boost::unique_lock<boost::shared_mutex> lock(*GetLock());

    func(_all);
}

FakePlayerContainer::PlayerVector FakePlayerContainer::GetRandom(uint32 count, std::function<bool(FakePlayer*)> predicate) const
{
    if (count < 1)
        return FakePlayerContainer::PlayerVector();

    FakePlayerContainer::PlayerVector players;
    players.reserve(_all.size());

    for (auto itr = _all.begin(); itr != _all.end(); itr++)
    {
        if (predicate(itr->second))
            players.push_back(itr->second);
    }

    if (players.size() > 0)
    {
        if (count == 1)
        {
            FakePlayerContainer::PlayerVector v2;
            v2.push_back(players[urand(0, players.size() - 1)]);
            return v2;
        }

        std::random_shuffle(players.begin(), players.end());
    }

    if (players.size() > count)
        players.resize(count);

    return players;
}

boost::shared_mutex* FakePlayerContainer::GetLock()
{
    static boost::shared_mutex mutex;

    return &mutex;
}

void FakePlayerContainer::Add(FakePlayer* player, bool lock)
{
    if (lock)
    {
        boost::unique_lock<boost::shared_mutex> lock(*GetLock());
        Add(player, false);
    }
    else
    {
        _all[player->accountId] = player;
    }
}

void FakePlayerContainer::Get(uint32 accountId, std::function<void(FakePlayer*)> func)
{
    Do([accountId, func](FakePlayerContainer::PlayerMap& all) 
    {
        auto itr = all.find(accountId);

        if (itr != all.end())
            func(itr->second);
    });
}

bool FakePlayerContainer::Empty()
{
    return _all.empty();
}
