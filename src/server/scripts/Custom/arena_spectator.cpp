#include "Common.h"
#include "Config.h"
#include "Creature.h"
#include "Define.h"
#include "Field.h"
#include "GossipDef.h"
#include "Player.h"
#include "ObjectGuid.h"
#include "ObjectMgr.h"
#include "ScriptedGossip.h"
#include "ScriptMgr.h"
#include "SharedDefines.h"
#include "WorldSession.h"
#include "BattlegroundMgr.h"
#include "Battleground.h"
#include "ArenaTeamMgr.h"
#include "ArenaTeam.h"
#include "Chat.h"
#include "Pet.h"

enum SPECTATOR_LANG
{
    LANG_GAMES_WITH_LOW_RATING = 11500,
    LANG_GAMES_WITH_HIGH_RATING,
    LANG_GAME_INFO,
    LANG_UPDATE_GAMES
};

#define ACTION_GAMES_WITH_LOW_RATING 1001
#define ACTION_GAMES_WITH_HIGH_RATING 1002
#define ACTION_VIEW 1010

#define GTS player->GetSession()->GetTrinityString

class ArenaSpecator_CreatureScript : public CreatureScript
{
public:
    ArenaSpecator_CreatureScript() : CreatureScript("arena_spectator") { }

    struct ArenaSpecator_CreatureScriptAI : public ScriptedAI
    {
        ArenaSpecator_CreatureScriptAI(Creature* creature) : ScriptedAI(creature) { }

        bool Spectrate(Battleground* bg, Player* player)
        {
            if (bg->GetStatus() != STATUS_IN_PROGRESS)
                return false;

            BattlegroundQueueTypeId bgQueueTypeId = sBattlegroundMgr->BGQueueTypeId(bg->GetTypeID(), bg->GetArenaType());

            if (player->InBattlegroundQueueForBattlegroundQueueType(bgQueueTypeId) || !player->HasFreeBattlegroundQueueId())
                return false;

            Player* target = NULL;
            auto &players = bg->GetPlayers();

            for (auto itr = players.begin(); itr != players.end(); itr++)
            {
                if (itr->second.Team == 35)
                    continue;

                if (auto plr = ObjectAccessor::FindPlayer(itr->first))
                {
                    if (plr->IsInWorld() && plr->IsAlive() && plr->GetMap()->IsBattleArena())
                    {
                        target = plr;
                        break;
                    }
                }
            }

            if (!target)
                return false;

            if (auto pet = ObjectAccessor::GetCreature(*player, player->GetPetGUID()))
            {
                pet->DespawnOrUnsummon();
            }

            if (auto pet2 = player->GetPet())
            {
                pet2->DespawnOrUnsummon();
            }

            player->AddBattlegroundQueueId(bgQueueTypeId);

            WorldPacket status;
            uint32 queueSlot = player->GetBattlegroundQueueIndex(bgQueueTypeId);

            sBattlegroundMgr->BuildBattlegroundStatusPacket(&status, bg, queueSlot, STATUS_IN_PROGRESS, 0, bg->GetStartTime(), bg->GetArenaType(), 0);
            player->SendDirectMessage(&status);

            player->SetBattlegroundId(target->GetBattlegroundId(), target->GetBattlegroundTypeId());
            player->SetBattlegroundEntryPoint();
            float x, y, z;
            target->GetContactPoint(player, x, y, z);

            player->TeleportTo(target->GetMapId(), x, y, z, player->GetAngle(target), TELE_TO_GM_MODE);
            player->SetPhaseMask(target->GetPhaseMask(), true);

            bg->AddSpectator(player);
            return true;
        }

        bool GossipHello(Player* player) override
        {
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS(LANG_GAMES_WITH_LOW_RATING), GOSSIP_SENDER_MAIN, ACTION_GAMES_WITH_LOW_RATING);
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS(LANG_GAMES_WITH_HIGH_RATING), GOSSIP_SENDER_MAIN, ACTION_GAMES_WITH_HIGH_RATING);
            player->PlayerTalkClass->SendGossipMenu(DEFAULT_GOSSIP_MESSAGE, me->GetGUID());
            return true;
        }

        bool GossipSelect(Player* player, uint32 /*menu_id*/, uint32 gossipListId) override
        {
            //uint32 sender = player->PlayerTalkClass->GetGossipOptionSender(gossipListId);
            uint32 action = player->PlayerTalkClass->GetGossipOptionAction(gossipListId);

            if (player->IsInCombat())
                return false;

            player->PlayerTalkClass->ClearMenus();

            if (action == ACTION_GAMES_WITH_LOW_RATING || action == ACTION_GAMES_WITH_HIGH_RATING)
            {
                AddGossipItemFor(player, GOSSIP_ICON_CHAT, GTS(LANG_UPDATE_GAMES), GOSSIP_SENDER_MAIN, action);
                uint32 minRating = action == ACTION_GAMES_WITH_LOW_RATING ? 0 : 1800;
                uint32 maxRating = action == ACTION_GAMES_WITH_LOW_RATING ? 1799 : 100000;

                auto const &bgs = sBattlegroundMgr->GetBattlegrounds();

                for (auto itr = bgs.begin(); itr != bgs.end(); itr++)
                {
                    auto bg = *itr;

                    if (!bg->isArena() || !bg->isRated() || bg->GetStatus() != STATUS_IN_PROGRESS)
                        continue;

                    auto teamId1 = bg->GetArenaTeamIdByIndex(0);
                    auto teamId2 = bg->GetArenaTeamIdByIndex(1);

                    if (!teamId1 || !teamId2)
                        continue;

                    auto team1 = sArenaTeamMgr->GetArenaTeamById(teamId1);
                    auto team2 = sArenaTeamMgr->GetArenaTeamById(teamId2);

                    if (!team1 || !team2)
                        continue;

                    if ((team1->GetRating() < minRating && team2->GetRating() < minRating) ||
                        (team2->GetRating() > maxRating && team2->GetRating() > maxRating))
                        continue;

                    auto type = team1->GetType() == ARENA_TEAM_5v5 ? "[Solo 1x1" : team1->GetType() == ARENA_TEAM_2v2 ? "2x2" : team1->GetType() == ARENA_TEAM_3v3 ? "[Solo 3x3" : "";
                    char buffer[2048];
                    sprintf(buffer, GTS(LANG_GAME_INFO), type, team1->GetName().c_str(), team1->GetRating(), team2->GetName().c_str(), team2->GetRating());

                    AddGossipItemFor(player, GOSSIP_ICON_BATTLE, buffer, GOSSIP_SENDER_MAIN, ACTION_VIEW + bg->GetInstanceID());
                }

                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, me->GetGUID());
            }
            else if (action > ACTION_VIEW)
            {
                auto instanceId = action - ACTION_VIEW;
                auto bg = sBattlegroundMgr->GetBattleground(instanceId, BATTLEGROUND_TYPE_NONE);

                if (!bg || !Spectrate(bg, player))
                    return GossipHello(player);

                CloseGossipMenuFor(player);
            }
            return true;
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new ArenaSpecator_CreatureScriptAI(creature);
    }
};

void AddSC_arena_spectator()
{
    new ArenaSpecator_CreatureScript();
}
