#include "ScriptMgr.h"
#include "Language.h"
#include "GameTime.h"
#include "World.h"
#include "WorldSession.h"
#include "ScriptedGossip.h"
#include "ItemEnchantmentMgr.h"
#include "SpellMgr.h"
#include "SpellHistory.h"
#include "SpellInfo.h"
#include "Pet.h"
#include "pvp_zone.h"
#include "RBAC.h"
#include "Chat.h"

// DELETE FROM spell_area WHERE spell = 42202;

static std::vector<Position> _pvpLocations;
static std::unordered_map<std::string, ObjectGuid> _players;
uint32 lastReportTime = 0, lastReportCount = -1;

void initLocations()
{
    _pvpLocations.push_back(Position(5537.177734f, -3866.294678f, 1614.149780f, 2.153959f));
    _pvpLocations.push_back(Position(5482.461426f, -3826.800049f, 1611.277222f, 2.699811f));
    _pvpLocations.push_back(Position(5423.485352f, -3798.906494f, 1611.153076f, 2.699811f));
    _pvpLocations.push_back(Position(5376.836914f, -3814.131104f, 1612.901733f, 3.453794f));
    _pvpLocations.push_back(Position(5404.326660f, -3768.816162f, 1610.698608f, 1.568838f));
    _pvpLocations.push_back(Position(5365.799805f, -3746.930908f, 1611.041260f, 2.629126f));
    _pvpLocations.push_back(Position(5326.283203f, -3740.309814f, 1611.696167f, 3.335985f));
    _pvpLocations.push_back(Position(5282.061035f, -3737.671387f, 1609.246216f, 3.100365f));
    _pvpLocations.push_back(Position(5234.220703f, -3698.932861f, 1617.054932f, 2.397434f));
    _pvpLocations.push_back(Position(5303.518555f, -3691.259277f, 1593.909790f, 3.104294f));
    _pvpLocations.push_back(Position(5385.756836f, -3688.483154f, 1593.443115f, 6.218398f));
    _pvpLocations.push_back(Position(5450.213379f, -3708.066895f, 1593.533813f, 5.951364f));
    _pvpLocations.push_back(Position(5505.798828f, -3725.766357f, 1593.924438f, 6.049536f));
    _pvpLocations.push_back(Position(5562.804199f, -3737.746582f, 1599.900391f, 6.155564f));
    _pvpLocations.push_back(Position(5592.281250f, -3735.721436f, 1607.320435f, 0.653847f));
    _pvpLocations.push_back(Position(5629.266602f, -3713.553223f, 1618.076660f, 0.539965f));
    _pvpLocations.push_back(Position(5646.637207f, -3682.303711f, 1622.668091f, 1.333217f));
    _pvpLocations.push_back(Position(5623.824707f, -3655.906494f, 1612.765503f, 2.342454f));
    _pvpLocations.push_back(Position(5578.643555f, -3644.173828f, 1600.806030f, 2.888306f));
    _pvpLocations.push_back(Position(5530.858887f, -3633.462402f, 1590.281006f, 2.970773f));
    _pvpLocations.push_back(Position(5483.448730f, -3628.514404f, 1578.589966f, 3.143561f));
    _pvpLocations.push_back(Position(5431.581543f, -3621.024658f, 1561.328125f, 2.915795f));
    _pvpLocations.push_back(Position(5387.823242f, -3595.519043f, 1561.155518f, 2.401359f));
    _pvpLocations.push_back(Position(5414.582520f, -3557.588623f, 1562.740356f, 0.952299f));
    _pvpLocations.push_back(Position(5496.471191f, -3577.503174f, 1564.047852f, 5.955286f));
    _pvpLocations.push_back(Position(5533.482910f, -3549.967041f, 1566.045044f, 0.818780f));
    _pvpLocations.push_back(Position(5563.127930f, -3515.825684f, 1570.113525f, 0.614577f));
    _pvpLocations.push_back(Position(5600.229492f, -3497.975830f, 1579.189819f, 0.716679f));
    _pvpLocations.push_back(Position(5626.792480f, -3464.197510f, 1583.768188f, 0.971933f));
    _pvpLocations.push_back(Position(5600.639160f, -3432.960449f, 1579.231812f, 2.267841f));
    _pvpLocations.push_back(Position(5619.045898f, -3395.807373f, 1584.611694f, 0.987641f));
    _pvpLocations.push_back(Position(5662.107910f, -3381.514648f, 1587.955811f, 0.512475f));
    _pvpLocations.push_back(Position(5600.359863f, -3368.687500f, 1581.945435f, 3.245661f));
    _pvpLocations.push_back(Position(5531.357422f, -3365.904053f, 1576.048096f, 3.858272f));
    _pvpLocations.push_back(Position(5494.396973f, -3388.052490f, 1570.308716f, 3.661922f));
    _pvpLocations.push_back(Position(5467.951172f, -3421.999023f, 1561.981689f, 3.877907f));
    _pvpLocations.push_back(Position(5423.856934f, -3432.333740f, 1560.671509f, 3.563748f));
    _pvpLocations.push_back(Position(5385.018555f, -3454.897705f, 1562.280518f, 3.037531f));
    _pvpLocations.push_back(Position(5344.518066f, -3453.795654f, 1570.507690f, 3.359544f));
    _pvpLocations.push_back(Position(5302.421875f, -3436.868896f, 1570.797729f, 2.782276f));
    _pvpLocations.push_back(Position(5306.057617f, -3483.045166f, 1576.475708f, 4.726135f));
    _pvpLocations.push_back(Position(5299.743652f, -3527.632324f, 1592.341187f, 4.474808f));
    _pvpLocations.push_back(Position(5274.807617f, -3565.469727f, 1593.650269f, 3.870054f));
    _pvpLocations.push_back(Position(5254.480469f, -3599.531494f, 1593.443115f, 4.973536f));
    _pvpLocations.push_back(Position(5257.417480f, -3674.936035f, 1593.595459f, 5.389797f));
    _pvpLocations.push_back(Position(5302.279297f, -3669.284912f, 1593.429810f, 0.115847f));
    _pvpLocations.push_back(Position(5348.879395f, -3598.998291f, 1561.667847f, 4.231339f));
    _pvpLocations.push_back(Position(5385.980469f, -3598.187500f, 1560.459229f, 6.214468f));
    _pvpLocations.push_back(Position(5418.805664f, -3622.416748f, 1561.296997f, 5.645056f));
    _pvpLocations.push_back(Position(5466.047363f, -3615.710938f, 1568.767212f, 0.304346f));
}

Position getRandomPosition(Player* player)
{
    if (_pvpLocations.size() < 1)
        initLocations();

    if (_pvpLocations.size() < 1)
        return player->GetPosition();

    return _pvpLocations[urand(0, _pvpLocations.size() - 1)];
}

bool isPVPZone(uint32 zoneId)
{
    return zoneId == 616;
}

bool isInPVPZone(Player* player)
{
    return isPVPZone(player->GetZoneId());
}

void teleportToRandomLocation(Player* player)
{
    auto pos = getRandomPosition(player);
    player->TeleportTo(1, pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ(), player->GetOrientation());
}

void announceCount(bool force = false)
{
    uint32 count = _players.size();

    if (force || lastReportTime != GameTime::GetGameTime() || lastReportCount != count)
    {
        sWorld->SendWorldText(LANG_PVP_ZONE_PLAYER_COUNT_ANNOUNCE, count);

        lastReportTime = GameTime::GetGameTime();
        lastReportCount = count;
    }
}

void removePlayer(Player* player, bool announce = true)
{
    for (auto itr = _players.begin(); itr != _players.end(); itr++)
    {
        if (itr->second == player->GetGUID())
        {
            _players.erase(itr);

            if (announce)
                announceCount();

            break;
        }
    }
}

void addPlayer(Player* player)
{
	player->RemoveFromGroup(GROUP_REMOVEMETHOD_LEAVE);
	
    auto itr = _players.find(player->GetSession()->GetRemoteAddress());
    if (itr != _players.end() && itr->second != player->GetGUID())
    {
        if (auto plr = ObjectAccessor::FindPlayer(itr->second))
            plr->GetSession()->KickPlayer();

        _players[player->GetSession()->GetRemoteAddress()] = player->GetGUID();
    }
    else if (itr == _players.end())
    {
        _players[player->GetSession()->GetRemoteAddress()] = player->GetGUID();
        announceCount();
    }
}

class PVPZone_Creature : public CreatureScript
{
public:
    PVPZone_Creature() : CreatureScript("pvpzone_npc") { }
    struct PVPZone_CreatureAI : public ScriptedAI
    {
        PVPZone_CreatureAI(Creature* creature) : ScriptedAI(creature) {}

        bool GossipHello(Player* player) override
        {
            AddGossipItemFor(player, GOSSIP_ICON_BATTLE, player->GetSession()->GetTrinityString(LANG_PVP_ZONE_TELEPORT_ME), GOSSIP_SENDER_MAIN, 1000);
            AddGossipItemFor(player, GOSSIP_ICON_BATTLE, player->GetSession()->GetTrinityString(LANT_PVP_ZONE_CLOSE_MENU), GOSSIP_SENDER_MAIN, 1001);
            SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, me->GetGUID());

            return true;
        }

        bool GossipSelect(Player* player, uint32 /*menuId*/, uint32 gossipListId) override
        {
            uint32 const action = player->PlayerTalkClass->GetGossipOptionAction(gossipListId);

            CloseGossipMenuFor(player);

            if (action == 1000)
                teleportToRandomLocation(player);

            return true;
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new PVPZone_CreatureAI(creature);
    }
};

class PVPZone_PlayerScript : public PlayerScript
{
private:
    void AddItem(Player* player, uint32 itemId, uint32 count)
    {
        uint32 noSpaceForCount = 0;
        ItemPosCountVec dest;
        InventoryResult msg = player->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, itemId, count, &noSpaceForCount);
        if (msg != EQUIP_ERR_OK)                               // convert to possible store amount
            count -= noSpaceForCount;

        if (count == 0 || dest.empty())                         // can't add any
            return;

        Item* item = player->StoreNewItem(dest, itemId, true);
        if (count > 0 && item)
            player->SendNewItem(item, count, false, false);
    }

public:
    PVPZone_PlayerScript() : PlayerScript("pvpzone_playerscript") { }

    void OnPVPKill(Player* killer, Player* killed) override
    {
        if (!isInPVPZone(killer) || killer->GetGUID() == killed->GetGUID())
            return;

        uint32 tokenId = sWorld->getIntConfig(CONFIG_PVP_ZONE_TOKEN_ID);
        uint32 tokenCount = sWorld->getIntConfig(CONFIG_PVP_ZONE_TOKEN_COUNT);

        if (tokenId && tokenCount)
            AddItem(killer, tokenId, tokenCount);
    }

    void OnUpdateZone(Player* player, uint32 newZone, uint32 newArea) override
    {
        if (isPVPZone(newZone))
            addPlayer(player);
        else
            removePlayer(player);
    }

    void OnLogin(Player* player, bool) override
    {
        if (isInPVPZone(player))
            addPlayer(player);
    }

    void OnLogout(Player* player) override
    {
        removePlayer(player);
    }

    void OnPlayerRepop(Player* player) override
    {
        if (!isInPVPZone(player))
            return;

        player->ResurrectPlayer(1.0f);
        ResetCooldowns(player);
        teleportToRandomLocation(player);
    }

    static void ResetCooldowns(Player* player)
    {
        player->GetSpellHistory()->ResetCooldowns([](SpellHistory::CooldownStorageType::iterator itr) -> bool
        {
            SpellInfo const* spellInfo = sSpellMgr->AssertSpellInfo(itr->first);
            return spellInfo->RecoveryTime < 10 * MINUTE * IN_MILLISECONDS
                && spellInfo->CategoryRecoveryTime < 10 * MINUTE * IN_MILLISECONDS
                && !itr->second.OnHold;
        }, true);

        if (Pet* pet = player->GetPet())
            pet->GetSpellHistory()->ResetAllCooldowns();
    }
};

class PVPZone_WorldScript : public WorldScript
{
public:
    PVPZone_WorldScript() : WorldScript("pvpzone_worldScript") { }

    void OnStartup() override
    {
        _events.ScheduleEvent(1, 5 * MINUTE * IN_MILLISECONDS);
    }

    void OnUpdate(uint32 diff) override
    {
        _events.Update(diff);

        while (uint32 event = _events.ExecuteEvent())
        {
            if (event == 1)
            {
                _events.ScheduleEvent(1, 5 * MINUTE * IN_MILLISECONDS);

                announceCount();
            }
        }
    }

private:
    EventMap _events;
};

#ifdef _DEBUG

class PVPZone_CommandScript : public CommandScript
{
public:
    PVPZone_CommandScript() : CommandScript("pvpzone_commandScript") { }

    std::vector<ChatCommand> GetCommands() const override
    {
        static std::vector<ChatCommand> commandTable =
        {
            { "addpvpzone", rbac::RBAC_PERM_RA_COMMANDS,  false, &HandleAddPVPZoneCommand, "" },
            { "removepvpzone", rbac::RBAC_PERM_RA_COMMANDS,  false, &HandleRemovePVPZoneCommand, "" },
            { "listpvpzones", rbac::RBAC_PERM_RA_COMMANDS,  true, &HandleListPvpZoneCommand, "" },
        };

        return commandTable;
    }

    static bool HandleAddPVPZoneCommand(ChatHandler* handler, char const* /*args*/)
    {
        if (auto session = handler->GetSession())
        {
            if (auto player = session->GetPlayer())
            {
                _pvpLocations.push_back(player->GetPosition());

                handler->PSendSysMessage("Location added. Total locations: %u", _pvpLocations.size());
            }
        }

        return true;
    }

    static bool HandleRemovePVPZoneCommand(ChatHandler* handler, char const* /*args*/)
    {
        if (_pvpLocations.size() > 0)
        {
            auto itr = _pvpLocations.rbegin();
            auto pos = *itr;

            _pvpLocations.erase(std::next(itr).base());

            handler->PSendSysMessage("Location %ff, %ff, %ff, %ff removed", pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ(), pos.GetOrientation());
        }
        else
            handler->SendSysMessage("No PVPZones in list");

        return true;
    }

    static bool HandleListPvpZoneCommand(ChatHandler* handler, char const* /*args*/)
    {
        if (_pvpLocations.size() > 0)
        {
            for (auto itr = _pvpLocations.begin(); itr != _pvpLocations.end(); itr++)
            {
                auto pos = *itr;
                handler->PSendSysMessage("_pvpLocations.push_back(Position(%f, %f, %f, %f));", pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ(), pos.GetOrientation());
            }
        }
        else
            handler->SendSysMessage("No PVPZones in list");

        return true;
    }
};

#endif // _DEBUG


void AddSC_PVPZone()
{
    new PVPZone_Creature();
    new PVPZone_PlayerScript();
    new PVPZone_WorldScript();
#ifdef _DEBUG
    new PVPZone_CommandScript();
#endif // _DEBUG

}
