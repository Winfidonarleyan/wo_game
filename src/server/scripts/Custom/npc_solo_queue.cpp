/*
* Copyright (C) 2008-2016 TrinityCore <http://www.trinitycore.org/>
* Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 2 of the License, or (at your
* option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
* more details.
*
* You should have received a copy of the GNU General Public License along
* with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "ScriptMgr.h"
#include "ArenaTeamMgr.h"
#include "Common.h"
#include "DisableMgr.h"
#include "BattlegroundMgr.h"
#include "Battleground.h"
#include "ArenaTeam.h"
#include "Language.h"
#include "npc_solo_queue.h"
#include "BattlegroundQueue.h"
#include "Player.h"
#include "World.h"
#include "GossipDef.h"
#include "Battleground.h"
#include "ScriptedGossip.h"
#include "WorldSession.h"
#include "DBCStores.h"
#include "PlayerAI.h"

#define GTS session->GetTrinityString

class npc_solo_queue : public CreatureScript
{
public:
    npc_solo_queue() : CreatureScript("npc_solo_queue") {}

    struct npc_solo_queueAI : public ScriptedAI
    {
        npc_solo_queueAI(Creature* creature) : ScriptedAI(creature)
        {
            for (int i = 0; i < MAX_TALENT_CAT; ++i)
                cache3v3Queue[i] = 0;

            lastFetchQueueList = 0;
        }

        bool GossipHello(Player* player) override
        {
            if (!player || !me)
                return true;

            if (sWorld->getBoolConfig(CONFIG_SOLO_3V3_ENABLE) == false)
            {
                ChatHandler(player->GetSession()).SendSysMessage(LANG_SOLO_ARENA_DISABLED);
                return true;
            }

            fetchQueueList();
            std::stringstream infoQueue;
            infoQueue << "Solo 3vs3 Arena\n";
            infoQueue << "|TInterface\\icons\\Inv_misc_note_05:25:25:-15:0|tИгроков в очереди: " << (cache3v3Queue[MELEE] + cache3v3Queue[RANGE] + cache3v3Queue[HEALER]);

            if (sWorld->getBoolConfig(CONFIG_SOLO_3V3_FILTER_TALENTS))
            {
                infoQueue << "\n\n";
                infoQueue << "В очереди мили: " << cache3v3Queue[MELEE] << " (Дольше ожидание!)\n";
                infoQueue << "В очереди кастеров: " << cache3v3Queue[RANGE] << " (Дольше ожидание!)\n";
                infoQueue << "В очереди хилов: " << cache3v3Queue[HEALER] << " (Бонусные награды!)\n";
            }

            AddGossipItemFor(player, GOSSIP_ICON_BATTLE, "|TInterface\\icons\\Achievement_arena_2v2_7:25:25:-15:0|tВстать в очередь 3х3 арены\n", GOSSIP_SENDER_MAIN, 2);
            AddGossipItemFor(player, GOSSIP_ICON_BATTLE, "|TInterface\\icons\\Achievement_arena_2v2_7:25:25:-15:0|tВстать в очередь 1х1 арены\n", GOSSIP_SENDER_MAIN, 20);
            //AddGossipItemFor(player, GOSSIP_ICON_BATTLE, "Queue up for 1vs1 Wargame\n", GOSSIP_SENDER_MAIN, 20);

            AddGossipItemFor(player, GOSSIP_ICON_CHAT, infoQueue.str().c_str(), GOSSIP_SENDER_MAIN, 0);
            //AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Script Info", GOSSIP_SENDER_MAIN, 8);
            SendGossipMenuFor(player, 70, me->GetGUID());
            return true;
        }

        bool GossipSelect(Player* player, uint32 /*menuId*/, uint32 gossipListId) override
        {
            if (!player || !me)
                return true;
            uint32 const action = player->PlayerTalkClass->GetGossipOptionAction(gossipListId);

            player->PlayerTalkClass->ClearMenus();

            switch (action)
            {
            case 2: // 3v3 Join Queue Arena (rated)
            {
                int8 slot = ARENA_TEAM_3v3;
                if (player->InBattlegroundQueue())
                {
                    ChatHandler(player->GetSession()).PSendSysMessage(LANG_SOLO_ARENA_ERROR);
                    CloseGossipMenuFor(player);
                    return true;
                }
                if (sWorld->getIntConfig(CONFIG_SOLO_3V3_MIN_LEVEL) <= player->getLevel())
                {
                    if (player->GetMoney() >= sWorld->getIntConfig(CONFIG_SOLO_3V3_COSTS) && CreateArenateam2(player, me))
                        player->ModifyMoney(sWorld->getIntConfig(CONFIG_SOLO_3V3_COSTS) * -1);
                }
                else if (!player->GetArenaTeamId(slot))
                {
                    ChatHandler(player->GetSession()).PSendSysMessage(LANG_SOLO_ARENA_LVL_ERROR, sWorld->getIntConfig(CONFIG_SOLO_3V3_MIN_LEVEL));
                    CloseGossipMenuFor(player);
                    return true;
                }

                // check Deserter debuff
                if (player->HasAura(26013) &&
                    (sWorld->getBoolConfig(CONFIG_SOLO_3V3_CAST_DESERTER_ON_AFK) || sWorld->getBoolConfig(CONFIG_SOLO_3V3_CAST_DESERTER_ON_LEAVE)))
                {
                    WorldPacket data;
                    sBattlegroundMgr->BuildGroupJoinedBattlegroundPacket(&data, ERR_GROUP_JOIN_BATTLEGROUND_DESERTERS);
                    player->GetSession()->SendPacket(&data);
                }
                else
                    if (ArenaCheckFullEquipAndTalents(player)
                        && JoinQueueArena2(player, me, true, ARENA_TYPE_3v3) == false)
                        ChatHandler(player->GetSession()).SendSysMessage(LANG_SOLO_ARENA_ERROR_JOIN);

                CloseGossipMenuFor(player);
                return true;
            }

            case 20: // 1v1 Join Queue Arena (rated)
            {
                int8 slot = ARENA_TEAM_5v5;
                if (player->InBattlegroundQueue())
                {
                    ChatHandler(player->GetSession()).PSendSysMessage(LANG_SOLO_ARENA_ERROR);
                    CloseGossipMenuFor(player);
                    return true;
                }

                if (sWorld->getBoolConfig(CONFIG_ARENA_1V1_BLOCK_FORBIDDEN_TALENTS) && PlayerAI::IsPlayerHealer(player))
                {
                    ChatHandler(player->GetSession()).SendSysMessage("Вы не можете встать в очередь на арену 1х1 будучи целителем");
                    return false;
                }

                CreateArenateam(player, me);

                if (ArenaCheckFullEquipAndTalents(player)
                    && JoinQueueArena(player, me, true, ARENA_TYPE_5v5) == false)
                    ChatHandler(player->GetSession()).SendSysMessage(LANG_SOLO_ARENA_ERROR_JOIN);

                CloseGossipMenuFor(player);
                return true;
            }
            }

            GossipHello(player);
            return true;
        }

    private:
        int16 cache3v3Queue[MAX_TALENT_CAT];
        uint32 lastFetchQueueList;

        bool ArenaCheckFullEquipAndTalents(Player* player)
        {
            if (!player)
                return false;

            if (sWorld->getBoolConfig(CONFIG_SOLO_3V3_EQUIP_AND_TALENTS) == false)
                return true;

            std::stringstream err;

            if (player->GetFreeTalentPoints() > 0)
                err << LANG_SOLO_ARENA_CURRENTLY_TALENT << player->GetFreeTalentPoints() << LANG_SOLO_ARENA_CURRENTLY_POINT;

            Item* newItem = NULL;
            for (uint8 slot = EQUIPMENT_SLOT_START; slot < EQUIPMENT_SLOT_END; ++slot)
            {
                if (slot == EQUIPMENT_SLOT_OFFHAND || slot == EQUIPMENT_SLOT_RANGED || slot == EQUIPMENT_SLOT_TABARD || slot == EQUIPMENT_SLOT_BODY)
                    continue;

                newItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, slot);
                if (newItem == NULL)
                {
                    err << player->GetSession()->GetTrinityString(LANG_SOLO_ARENA_CHAR_EQUIP);
                    break;
                }
            }

            if (err.str().length() > 0)
            {
                ChatHandler(player->GetSession()).SendSysMessage(err.str().c_str());
                return false;
            }
            return true;
        }


        bool JoinQueueArena(Player* player, Creature* me, bool isRated, uint8 arenatype)
        {
            if (!player || !me)
                return false;

            if (sWorld->getIntConfig(CONFIG_SOLO_3V3_MIN_LEVEL) > player->getLevel())
                return false;

            uint8 arenaslot = ArenaTeam::GetSlotByType(ARENA_TEAM_5v5);
            uint32 arenaRating = 0;
            uint32 matchmakerRating = 0;
            WorldSession* session = player->GetSession();

            // ignore if we already in BG or BG queue
            if (player->InBattleground() || player->InBattlegroundQueue())
                return false;

            //check existance
            Battleground* bg = sBattlegroundMgr->GetBattlegroundTemplate(BATTLEGROUND_AA);
            if (!bg)
                return false;

            if (DisableMgr::IsDisabledFor(DISABLE_TYPE_BATTLEGROUND, BATTLEGROUND_AA, NULL))
            {
                ChatHandler(player->GetSession()).PSendSysMessage(LANG_ARENA_DISABLED);
                return false;
            }

            BattlegroundTypeId bgTypeId = bg->GetTypeID();
            BattlegroundQueueTypeId bgQueueTypeId = BattlegroundMgr::BGQueueTypeId(bgTypeId, arenatype);
            PvPDifficultyEntry const* bracketEntry = GetBattlegroundBracketByLevel(bg->GetMapId(), player->getLevel());
            if (!bracketEntry)
                return false;

            // check if already in queue
            if (player->GetBattlegroundQueueIndex(bgQueueTypeId) < PLAYER_MAX_BATTLEGROUND_QUEUES)
                //player is already in this queue
                return false;
            // check if has free queue slots
            if (!player->HasFreeBattlegroundQueueId())
                return false;

            if (sWorld->getBoolConfig(CONFIG_SOLO_3V3_TIME_CHECK))
            {
                uint32 reqPlayedTime = 15 * MINUTE; // 15 minutes

                if (player->GetTotalPlayedTime() <= reqPlayedTime)
                {
                    uint32 remainingTime = ((15 * MINUTE) - player->GetTotalPlayedTime()) / MINUTE;
                    uint32 currentTime = 15 - remainingTime;

                    session->SendAreaTriggerMessage(GTS(LANG_SOLO_ARENA_PLAYED_TIME));
                    session->SendAreaTriggerMessage(GTS(LANG_SOLO_ARENA_PLAYED_TIMER), currentTime);
                    ChatHandler(player->GetSession()).SendSysMessage(LANG_SOLO_ARENA_PLAYED_TIME);
                    ChatHandler(player->GetSession()).PSendSysMessage(LANG_SOLO_ARENA_PLAYED_TIMER, currentTime);
                    return false;
                }
            }

            uint32 ateamId = 0;

            if (isRated)
            {
                ateamId = player->GetArenaTeamId(arenaslot);
                ArenaTeam* at = sArenaTeamMgr->GetArenaTeamById(ateamId);
                if (!at)
                {
                    player->GetSession()->SendNotInArenaTeamPacket(arenatype);
                    return false;
                }

                // get the team rating for queueing
                arenaRating = at->GetRating();
                matchmakerRating = at->GetAverageMMR();
                // the arenateam id must match for everyone in the group

                if (arenaRating <= 0)
                    arenaRating = 1;
            }

            BattlegroundQueue &bgQueue = sBattlegroundMgr->GetBattlegroundQueue(bgQueueTypeId);

            if (sWorld->getBoolConfig(CONFIG_SOLO_3V3_IP_CHECK))
            {
                if (bgQueue.IPExistsInQueue(player, NULL, bracketEntry, ARENA_TYPE_5v5, isRated, false))
                {
                    session->SendAreaTriggerMessage(GTS(LANG_SOLO_ARENA_CHECK_IP));
                    ChatHandler(player->GetSession()).SendSysMessage(LANG_SOLO_ARENA_CHECK_IP);
                    return false;
                }

            }

            bg->SetRated(isRated);

            GroupQueueInfo* ginfo = bgQueue.AddGroup(player, NULL, bgTypeId, bracketEntry, arenatype, isRated, false, arenaRating, matchmakerRating, ateamId);
            uint32 avgTime = bgQueue.GetAverageQueueWaitTime(ginfo, bracketEntry->GetBracketId());
            uint32 queueSlot = player->AddBattlegroundQueueId(bgQueueTypeId);

            WorldPacket data;
            // send status packet (in queue)
            sBattlegroundMgr->BuildBattlegroundStatusPacket(&data, bg, queueSlot, STATUS_WAIT_QUEUE, avgTime, 0, arenatype, 0);
            player->GetSession()->SendPacket(&data);

            sBattlegroundMgr->ScheduleQueueUpdate(matchmakerRating, arenatype, bgQueueTypeId, bgTypeId, bracketEntry->GetBracketId());

            return true;
        }


        bool CreateArenateam(Player* player, Creature* me)
        {
            if (!player || !me)
                return false;

            uint8 slot = ArenaTeam::GetSlotByType(ARENA_TEAM_5v5);
            if (slot >= MAX_ARENA_SLOT)
                return false;

            // Check if player is already in an arena team
            if (player->GetArenaTeamId(slot))
                return false;

            // Teamname = playername
            // if team name exist, we have to choose another name (playername + number)
            int i = 1;
            std::stringstream teamName;
            teamName << "[Solo Queue 1v1]: " + player->GetName();
            do
            {
                if (sArenaTeamMgr->GetArenaTeamByName(teamName.str()) != NULL) // teamname exist, so choose another name
                {
                    teamName.str(std::string());
                    teamName << player->GetName() << i;
                }
                else
                    break;
            } while (i < 100); // should never happen

            // Create arena team
            ArenaTeam* arenaTeam = new ArenaTeam();

            if (!arenaTeam->Create(player->GetGUID(), ARENA_TEAM_5v5, teamName.str(), 4283124816, 45, 4294242303, 5, 4294705149))
            {
                delete arenaTeam;
                return false;
            }

            // Register arena team
            sArenaTeamMgr->AddArenaTeam(arenaTeam);
            arenaTeam->AddMember(player->GetGUID());

            ChatHandler(player->GetSession()).SendSysMessage(LANG_SOLO_ARENA_TEAM_CREATED);

            return true;
        }

        bool JoinQueueArena2(Player* player, Creature* me, bool isRated, uint8 arenatype)
        {
            if (!player || !me)
                return false;

            if (sWorld->getIntConfig(CONFIG_SOLO_3V3_MIN_LEVEL) > player->getLevel())
                return false;

            uint8 arenaslot = ArenaTeam::GetSlotByType(ARENA_TEAM_3v3);
            uint32 arenaRating = 0;
            uint32 matchmakerRating = 0;
            WorldSession* session = player->GetSession();

            // ignore if we already in BG or BG queue
            if (player->InBattleground() || player->InBattlegroundQueue())
                return false;

            //check existance
            Battleground* bg = sBattlegroundMgr->GetBattlegroundTemplate(BATTLEGROUND_AA);
            if (!bg)
                return false;

            if (DisableMgr::IsDisabledFor(DISABLE_TYPE_BATTLEGROUND, BATTLEGROUND_AA, NULL))
            {
                ChatHandler(player->GetSession()).PSendSysMessage(LANG_ARENA_DISABLED);
                return false;
            }

            BattlegroundTypeId bgTypeId = bg->GetTypeID();
            BattlegroundQueueTypeId bgQueueTypeId = BattlegroundMgr::BGQueueTypeId(bgTypeId, arenatype);
            PvPDifficultyEntry const* bracketEntry = GetBattlegroundBracketByLevel(bg->GetMapId(), player->getLevel());
            if (!bracketEntry)
                return false;

            // check if already in queue
            if (player->GetBattlegroundQueueIndex(bgQueueTypeId) < PLAYER_MAX_BATTLEGROUND_QUEUES)
                //player is already in this queue
                return false;
            // check if has free queue slots
            if (!player->HasFreeBattlegroundQueueId())
                return false;

            if (sWorld->getBoolConfig(CONFIG_SOLO_3V3_TIME_CHECK))
            {
                uint32 reqPlayedTime = 15 * MINUTE; // 15 minutes

                if (player->GetTotalPlayedTime() <= reqPlayedTime)
                {
                    uint32 remainingTime = ((15 * MINUTE) - player->GetTotalPlayedTime()) / MINUTE;
                    uint32 currentTime = 15 - remainingTime;

                    session->SendAreaTriggerMessage(GTS(LANG_SOLO_ARENA_PLAYED_TIME));
                    session->SendAreaTriggerMessage(GTS(LANG_SOLO_ARENA_PLAYED_TIMER), currentTime);
                    ChatHandler(player->GetSession()).SendSysMessage(LANG_SOLO_ARENA_PLAYED_TIME);
                    ChatHandler(player->GetSession()).PSendSysMessage(LANG_SOLO_ARENA_PLAYED_TIMER, currentTime);
                    return false;
                }
            }

            uint32 ateamId = 0;

            if (isRated)
            {
                ateamId = player->GetArenaTeamId(arenaslot);
                ArenaTeam* at = sArenaTeamMgr->GetArenaTeamById(ateamId);
                if (!at)
                {
                    player->GetSession()->SendNotInArenaTeamPacket(arenatype);
                    return false;
                }

                // get the team rating for queueing
                arenaRating = at->GetRating();
                matchmakerRating = at->GetAverageMMR();
                // the arenateam id must match for everyone in the group

                if (arenaRating <= 0)
                    arenaRating = 1;
            }

            BattlegroundQueue &bgQueue = sBattlegroundMgr->GetBattlegroundQueue(bgQueueTypeId);

            if (sWorld->getBoolConfig(CONFIG_SOLO_3V3_IP_CHECK))
            {
                if (bgQueue.IPExistsInQueue(player, NULL, bracketEntry, ARENA_TYPE_3v3, isRated, false))
                {
                    session->SendAreaTriggerMessage(GTS(LANG_SOLO_ARENA_CHECK_IP));
                    ChatHandler(player->GetSession()).SendSysMessage(LANG_SOLO_ARENA_CHECK_IP);
                    return false;
                }

                if (bgQueue.IPExistsInQueue(player, NULL, bracketEntry, ARENA_TYPE_3v3, isRated, false))
                {
                    session->SendAreaTriggerMessage(GTS(LANG_SOLO_ARENA_CHECK_IP));
                    ChatHandler(player->GetSession()).SendSysMessage(LANG_SOLO_ARENA_CHECK_IP);
                    return false;
                }

            }

            bg->SetRated(isRated);

            GroupQueueInfo* ginfo = bgQueue.AddGroup(player, NULL, bgTypeId, bracketEntry, arenatype, isRated, false, arenaRating, matchmakerRating, ateamId);
            uint32 avgTime = bgQueue.GetAverageQueueWaitTime(ginfo, bracketEntry->GetBracketId());
            uint32 queueSlot = player->AddBattlegroundQueueId(bgQueueTypeId);

            WorldPacket data;
            // send status packet (in queue)
            sBattlegroundMgr->BuildBattlegroundStatusPacket(&data, bg, queueSlot, STATUS_WAIT_QUEUE, avgTime, 0, arenatype, 0);
            player->GetSession()->SendPacket(&data);

            sBattlegroundMgr->ScheduleQueueUpdate(matchmakerRating, arenatype, bgQueueTypeId, bgTypeId, bracketEntry->GetBracketId());

            return true;
        }

        bool CreateArenateam2(Player* player, Creature* me)
        {
            if (!player || !me)
                return false;

            uint8 slot = ArenaTeam::GetSlotByType(ARENA_TEAM_3v3);
            if (slot >= MAX_ARENA_SLOT)
                return false;

            // Check if player is already in an arena team
            if (player->GetArenaTeamId(slot))
                return false;

            // Teamname = playername
            // if team name exist, we have to choose another name (playername + number)
            int i = 1;
            std::stringstream teamName;
            teamName << player->GetName();
            do
            {
                if (sArenaTeamMgr->GetArenaTeamByName(teamName.str()) != NULL) // teamname exist, so choose another name
                {
                    teamName.str(std::string());
                    teamName << player->GetName() << i;
                }
                else
                    break;
            } while (i < 100); // should never happen
            
			// Create arena team
            ArenaTeam* arenaTeam = new ArenaTeam();

            if (!arenaTeam->Create(player->GetGUID(), ARENA_TEAM_3v3, teamName.str(), 4283124816, 45, 4294242303, 5, 4294705149))
            {
                delete arenaTeam;
                return false;
            }

            // Register arena team
            sArenaTeamMgr->AddArenaTeam(arenaTeam);
            arenaTeam->AddMember(player->GetGUID());

            ChatHandler(player->GetSession()).SendSysMessage(LANG_SOLO_ARENA_TEAM_CREATED);

            return true;
        }

        void fetchQueueList()
        {
            if (GetMSTimeDiffToNow(lastFetchQueueList) < 1000)
                return; // prevent spamming
            lastFetchQueueList = getMSTime();

            BattlegroundQueue* queue = &sBattlegroundMgr->GetBattlegroundQueue(BATTLEGROUND_QUEUE_3v3);

            for (int i = 0; i < MAX_TALENT_CAT; ++i)
                cache3v3Queue[i] = 0;
            for (int i = BG_BRACKET_ID_FIRST; i <= BG_BRACKET_ID_LAST; ++i)
                for (int j = 0; j < 2; ++j)
                {
                    for (BattlegroundQueue::GroupsQueueType::iterator itr = queue->m_QueuedGroups[i][j].begin(); itr != queue->m_QueuedGroups[i][j].end(); ++itr)
                    {
                        if ((*itr)->IsInvitedToBGInstanceGUID) // Skip when invited
                            continue;

                        std::map<ObjectGuid, PlayerQueueInfo*> *grpPlr = &(*itr)->Players;
                        for (std::map<ObjectGuid, PlayerQueueInfo*>::iterator grpPlrItr = grpPlr->begin(); grpPlrItr != grpPlr->end(); ++grpPlrItr)
                        {
                            Player* plr = ObjectAccessor::FindPlayer(grpPlrItr->first);
                            if (!plr)
                                continue;

                            Solo3v3TalentCat plrCat = GetTalentCatForSolo3v3(plr); // get talent cat
                            cache3v3Queue[plrCat]++;
                        }
                    }
                }
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_solo_queueAI(creature);
    }
};


void AddSC_npc_solo_queue()
{
	new npc_solo_queue();
}
