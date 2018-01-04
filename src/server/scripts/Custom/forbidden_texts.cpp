#include "ScriptMgr.h"
#include "Common.h"
#include "Config.h"
#include "Define.h"
#include "Field.h"
#include "Player.h"
#include "ObjectGuid.h"
#include "ObjectMgr.h"
#include "SharedDefines.h"
#include "Chat.h"
#include "AccountMgr.h"
#include "ArenaTeamMgr.h"
#include "CellImpl.h"
#include "CharacterCache.h"
#include "Chat.h"
#include "DatabaseEnv.h"
#include "DisableMgr.h"
#include "GridNotifiers.h"
#include "Group.h"
#include "GroupMgr.h"
#include "InstanceSaveMgr.h"
#include "Item.h"
#include "Language.h"
#include "LFG.h"
#include "Log.h"
#include "MMapFactory.h"
#include "MovementGenerator.h"
#include "ObjectAccessor.h"
#include "Opcodes.h"
#include "Pet.h"
#include "Player.h"
#include "Realm.h"
#include "SpellAuras.h"
#include "SpellHistory.h"
#include "SpellMgr.h"
#include "TargetedMovementGenerator.h"
#include "Transport.h"
#include "Weather.h"
#include "WeatherMgr.h"
#include "World.h"
#include "WorldSession.h"
#include <boost/algorithm/string.hpp>

namespace
{
    class ForbiddenTexts_PlayerScript : PlayerScript 
    {
    public:
        ForbiddenTexts_PlayerScript() : PlayerScript("forbidden_texts") { }

        void OnChat(Player* player, std::string& msg)
        {
            auto const &forbiddenTexts = sObjectMgr->GetForbiddenTexts();

            for (auto itr = forbiddenTexts.begin(); itr != forbiddenTexts.end(); itr++)
            {
                auto& forbiddenText = *itr;

                if (boost::icontains(msg, forbiddenText))
                {
					uint32 notSpeakTime = 3;
					std::string muteReasonStr("Запрещенное слово");
                    msg = "Запрещенное слово!";
					PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_UPD_MUTE_TIME);
					std::string muteBy = "";
					muteBy = ChatHandler(player->GetSession()).GetTrinityString(LANG_CONSOLE);
		
					if (player)
					{
						// Target is online, mute will be in effect right away.
						int64 muteTime = time(NULL) + notSpeakTime * MINUTE;
						player->GetSession()->m_muteTime = muteTime;
						stmt->setInt64(0, muteTime);
						std::string nameLink =  ChatHandler(player->GetSession()).playerLink( player->GetSession()->GetPlayerName());

						if (sWorld->getBoolConfig(CONFIG_SHOW_MUTE_IN_WORLD))
							sWorld->SendWorldText(LANG_COMMAND_MUTEMESSAGE_WORLD, muteBy.c_str(), nameLink.c_str(), notSpeakTime, muteReasonStr.c_str());

						ChatHandler(player->GetSession()).PSendSysMessage(LANG_YOUR_CHAT_DISABLED, notSpeakTime, muteBy.c_str(), muteReasonStr.c_str());
					}
					else
					{
						// Target is offline, mute will be in effect starting from the next login.
						int32 muteTime = -int32(notSpeakTime * MINUTE);
						stmt->setInt64(0, muteTime);
					}

					stmt->setString(1, muteReasonStr.c_str());
					stmt->setString(2, muteBy.c_str());
					stmt->setUInt32(3, player->GetSession()->GetAccountId());
					LoginDatabase.Execute(stmt);
					stmt = LoginDatabase.GetPreparedStatement(LOGIN_INS_ACCOUNT_MUTE);
					stmt->setUInt32(0, player->GetSession()->GetAccountId());
					stmt->setUInt32(1, notSpeakTime);
					stmt->setString(2, muteBy.c_str());
					stmt->setString(3, muteReasonStr.c_str());
					LoginDatabase.Execute(stmt);
					std::string nameLink = ChatHandler(player->GetSession()).playerLink(player->GetSession()->GetPlayerName());

					if (sWorld->getBoolConfig(CONFIG_SHOW_MUTE_IN_WORLD))
						sWorld->SendWorldText(LANG_COMMAND_MUTEMESSAGE_WORLD, muteBy.c_str(), nameLink.c_str(), notSpeakTime, muteReasonStr.c_str());
					
					ChatHandler(player->GetSession()).PSendSysMessage(player ? LANG_YOU_DISABLE_CHAT : LANG_COMMAND_DISABLE_CHAT_DELAYED, nameLink.c_str(), notSpeakTime, muteReasonStr.c_str());

                    return;
                }
            }
        }

        void OnChat(Player* player, uint32 /*type*/, uint32 /*lang*/, std::string& msg) override 
        {
            OnChat(player, msg);
        }

        void OnChat(Player* player, uint32 /*type*/, uint32 /*lang*/, std::string& msg, Player* /*receiver*/) override 
        {
            OnChat(player, msg);
        }

        void OnChat(Player* player, uint32 /*type*/, uint32 /*lang*/, std::string& msg, Group* /*group*/) override
        {
            OnChat(player, msg);
        }

        void OnChat(Player* player, uint32 /*type*/, uint32 /*lang*/, std::string& msg, Guild* /*guild*/) override
        {
            OnChat(player, msg);
        }

        void OnChat(Player* player, uint32 /*type*/, uint32 /*lang*/, std::string& msg, Channel* /*channel*/) override
        {
            OnChat(player, msg);
        }
    };
}

void AddSC_forbidden_texts()
{
    new ForbiddenTexts_PlayerScript();
}