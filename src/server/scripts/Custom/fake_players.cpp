#include "ScriptMgr.h"
#include "QueryCallback.h"
#include "fake_players.h"
#include "FakePlayerManager.h"
#include "Config.h"
#include "RBAC.h"
#include "WorldSession.h"
#include "Chat.h"

class FakePlayers_WorldScript : WorldScript
{
public:
    FakePlayers_WorldScript() : WorldScript("fake_players_world_script") { }

    void OnUpdate(uint32 diff) override
    {
        sFakePlayerMgr->Update(diff);
    }

    void OnStartup() override
    {
        sFakePlayerMgr->LoadNames();
        sFakePlayerMgr->LoadZones();
        sFakePlayerMgr->LoadPlayers();
    }

    void OnConfigLoad(bool reload) override
    {
        std::vector<uint32> values;
        values.resize(24);

        std::string str = sConfigMgr->GetStringDefault("FakePlayers.Times", "");
        auto ptr = str.c_str();

        for (uint8 i = 0; i < 24; i++)
        {
            uint32 value = 100;

            if (ptr != nullptr)
            {
                auto ptr2 = strtok(i == 0 ? (char*)ptr : nullptr, ",");

                if (ptr2 && *ptr2)
                    value = atol(ptr2);
            }

            if (value < 1)
                value = 1;
            else if (value > 1000)
                value = 1000;

            values[i] = value;
        }

        sFakePlayerMgr->SetHourValues(values);
    }
};

class fake_players_PlayerScript : PlayerScript
{
public:
    fake_players_PlayerScript() : PlayerScript("fake_players_playerscript") { }

    void OnLogin(Player* player, bool) override
    {
        if (player->GetSession()->IsFake())
        {
            sFakePlayerMgr->OnLogin(player);
        }
    }

    void OnLogout(Player* player) override
    {
        sFakePlayerMgr->OnLogout(player);
    }
};

class fake_players_commandscript : public CommandScript
{
public:
    fake_players_commandscript() : CommandScript("fake_players_commandscript") { }

    std::vector<ChatCommand> GetCommands() const override
    {
        static std::vector<ChatCommand> fakeCommandTable =
        {
            { "create", rbac::RBAC_PERM_COMMAND_ACHIEVEMENT_ADD, true, &HandleFakeCreateCommand , "" },
        };

        static std::vector<ChatCommand> commandTable =
        {
            { "fake", rbac::RBAC_PERM_COMMAND_ACHIEVEMENT,  true, NULL, "", fakeCommandTable },
        };

        return commandTable;
    }

    static bool HandleFakeCreateCommand(ChatHandler* handler, char const* args)
    {
        return false;

        /*std::string name = sFakePlayerMgr->CreateName();
        uint8 race = sFakePlayerMgr->GetRandomRace();
        uint8 Class = sFakePlayerMgr->GetRandomClass(race);
        uint8 gender = urand(0, 1);

        if (*args)
        {
            const char* nameStr = strtok((char*)args, " ");

            if (nameStr)
            {
                name = nameStr;

                const char* raceStr = strtok((char*)args, " ");

                if (raceStr)
                {
                    race = atoi(raceStr);

                    const char* classStr = strtok((char*)args, " ");

                    if (classStr)
                    {
                        Class = atoi(classStr);

                        const char* genderStr = strtok((char*)args, " ");

                        if (genderStr)
                        {
                            gender = atoi(genderStr);

                            if (gender)
                                gender = 1;
                        }
                    }
                }
            }
        }

        if (!sObjectMgr->GetPlayerInfo(race, Class))
        {
            handler->SendSysMessage("Invalid race/class pair");
            return false;
        }

        bool result = sFakePlayerMgr->Create(name, race, Class, gender, [handler](WorldSession* session)
        {
            TC_LOG_INFO("server.loading", session ? "Char create success" : "Char create failed on step 2");

            if (session)
            {
                if (!sFakePlayerMgr->Login(session))
                    TC_LOG_ERROR("server.loading", "Failed login fake player");
            }
        });

        if (!result)
            handler->SendSysMessage("Char create failed on step 1");

        return result;*/
    }
};

void AddSC_FakePlayers()
{
    new FakePlayers_WorldScript();
    new fake_players_PlayerScript();
    new fake_players_commandscript();
    /*new FakePlayers_PlayerScript();*/
}
