/*
 * Copyright (C) 2008-2017 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2006-2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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

/* ScriptData
SDName: Isle_of_Queldanas
SD%Complete: 100
SDComment: Quest support: 11541,24535,24563
SDCategory: Isle Of Quel'Danas
EndScriptData */

/* ContentData
npc_greengill_slave
npc_thalorien_dawnseeker
EndContentData */

#include "GameObject.h"
#include "GameObjectAI.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "Player.h"
#include "SpellInfo.h"

/*######
## npc_greengill_slave
######*/

enum GreengillSlave
{
    NPC_DARKSPINE_MYRIDON       = 25060,
    QUEST_GREENGILL_COAST       = 11541,
    SPELL_ENRAGE                = 45111,
    SPELL_ORB_MURLOC_CONTROL    = 45109,
    SPELL_GREENGILL_SLAVE_FREED = 45110
};

class npc_greengill_slave : public CreatureScript
{
public:
    npc_greengill_slave() : CreatureScript("npc_greengill_slave") { }

    struct npc_greengill_slaveAI : public ScriptedAI
    {
        npc_greengill_slaveAI(Creature* creature) : ScriptedAI(creature) { }

        void SpellHit(Unit* caster, SpellInfo const* spellInfo) override
        {
            Player* player = caster->ToPlayer();

            if (!player)
                return;

            if (spellInfo->Id == SPELL_ORB_MURLOC_CONTROL && !me->HasAura(SPELL_ENRAGE))
            {
                if (player->GetQuestStatus(QUEST_GREENGILL_COAST) == QUEST_STATUS_INCOMPLETE)
                    DoCast(player, SPELL_GREENGILL_SLAVE_FREED, true);

                DoCast(me, SPELL_ENRAGE);

                if (Creature* Myrmidon = me->FindNearestCreature(NPC_DARKSPINE_MYRIDON, 70))
                {
                    AddThreat(Myrmidon, 100000.0f);
                    AttackStart(Myrmidon);
                }
            }
        }

        void UpdateAI(uint32 /*diff*/) override
        {
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_greengill_slaveAI(creature);
    }
};

/*######
## npc_thalorien_dawnseeker
######*/

enum ThalorienDawnseekerSays
{
    SAY_THALORIEN_1 = 0,
    SAY_THALORIEN_2 = 1,
    SAY_THALORIEN_3 = 2,
    SAY_THALORIEN_4 = 3,
    SAY_THALORIEN_5 = 4,
    SAY_THALORIEN_6 = 5,
    SAY_THALORIEN_7 = 6,
    SAY_THALORIEN_8 = 7,
    SAY_THALORIEN_9 = 8,
    SAY_THALORIEN_10 = 9
};

enum MorlenSays
{
    SAY_MORLEN_1 = 0,
    SAY_MORLEN_2 = 1,
    SAY_MORLEN_3 = 2,
    SAY_MORLEN_4 = 3,
    SAY_MORLEN_5 = 4
};

enum ThalorienDawnseekerCreatures
{
    NPC_THALORIEN_DAWNSEEKER    = 37205,
    NPC_MORLEN_GOLDGRIP         = 37542,
    NPC_QUEST_CREDIT            = 37601,
    NPC_SUNWELL_DEFENDER        = 37211,
    NPC_SCOURGE_ZOMBIE          = 37538,
    NPC_GHOUL_INVADER           = 37539,
    NPC_CRYPT_RAIDER            = 37541
};

enum ThalorienDawnseekerActions
{
    ACTION_START_QUEST = 1
};

enum ThalorienDawnseekerEvents
{
    EVENT_STEP_1    = 1,
    EVENT_STEP_2    = 2,
    EVENT_STEP_3    = 3,
    EVENT_STEP_4    = 4,
    EVENT_STEP_5    = 5,
    EVENT_STEP_6    = 6,
    EVENT_STEP_7    = 7,
    EVENT_STEP_8    = 8,
    EVENT_STEP_9    = 9,
    EVENT_STEP_10   = 10,
    EVENT_STEP_11   = 11,
    EVENT_STEP_12   = 12,
    EVENT_STEP_13   = 13,
    EVENT_STEP_14   = 14,
    EVENT_STEP_15   = 15,
    EVENT_STEP_16   = 16,
    EVENT_STEP_17   = 17,
    EVENT_STEP_18   = 18,
    EVENT_STEP_19   = 19
};

enum ThalorienDawnseekerMisc
{
    OPTION_ID_REMAINS       = 0,
    DISPLAY_MOUNT           = 25678,
    QUEST_THALORIEN_A       = 24535,
    QUEST_THALORIEN_H       = 24563,
    GOSSIP_MENU_ID_REMAINS  = 37552,
    SPELL_BLOOD_PRESENCE    = 50689
};

Position const ZombieLoc[5] =
{
    { 11768.7f, -7065.83f, 24.0971f, 0.125877f },
    { 11769.5f, -7063.83f, 24.1399f, 6.06035f },
    { 11769.8f, -7061.41f, 24.2536f, 6.06035f },
    { 11769.4f, -7057.86f, 24.4624f, 0.00335493f },
    { 11769.4f, -7054.56f, 24.6869f, 0.00335493f }
};

Position const GuardLoc[4] =
{
    { 11807.0f, -7070.37f, 25.372f, 3.218f },
    { 11805.7f, -7073.64f, 25.5694f, 3.218f },
    { 11805.6f, -7077.0f, 25.9643f, 3.218f },
    { 11806.0f, -7079.71f, 26.2067f, 3.218f }
};

class npc_thalorien_dawnseeker : public CreatureScript
{
public:
    npc_thalorien_dawnseeker() : CreatureScript("npc_thalorien_dawnseeker") { }

    struct npc_thalorien_dawnseekerAI : public ScriptedAI
    {
        npc_thalorien_dawnseekerAI(Creature* creature) : ScriptedAI(creature) { }

        bool GossipHello(Player* player) override
        {
            player->PrepareGossipMenu(me, 0);

            if ((player->GetQuestStatus(QUEST_THALORIEN_A) == QUEST_STATUS_INCOMPLETE) || (player->GetQuestStatus(QUEST_THALORIEN_H) == QUEST_STATUS_INCOMPLETE))
                AddGossipItemFor(player, GOSSIP_MENU_ID_REMAINS, OPTION_ID_REMAINS, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->SendPreparedGossip(me);
            return true;
        }

        bool GossipSelect(Player* player, uint32 /*uiSender*/, uint32 gossipListId) override
        {
            uint32 const action = player->PlayerTalkClass->GetGossipOptionAction(gossipListId);
            ClearGossipMenuFor(player);

            switch (action)
            {
            case GOSSIP_ACTION_INFO_DEF + 1:
                CloseGossipMenuFor(player);
                me->AI()->SetGUID(player->GetGUID());
                me->SetPhaseMask(2, true);
                player->SetPhaseMask(2, true); // Better handling if we find the correct Phasing-Spell
                me->AI()->DoAction(ACTION_START_QUEST);
                break;
            default:
                return false;
            }
            return true;
        }

        void Reset()
        {
            if (Player* player = ObjectAccessor::GetPlayer(*me, uiPlayer))
                player->SetPhaseMask(1, true);
            if (Creature* Morlen = ObjectAccessor::GetCreature(*me, uiMorlen))
                Morlen->DisappearAndDie();
            me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            me->SetPhaseMask(1, true);
            events.Reset();
        }

        void UpdateAI(uint32 diff) override
        {
            events.Update(diff);

            switch (events.ExecuteEvent())
            {
            case EVENT_STEP_1:
                if (Creature* Thalorien = me->SummonCreature(NPC_THALORIEN_DAWNSEEKER, 11797.0f, -7074.06f, 26.379f, 0.242908f, TEMPSUMMON_CORPSE_DESPAWN, 0))
                {
                    Thalorien->SetPhaseMask(2, true);
                    uiThalorien = Thalorien->GetGUID();
                }

                for (int i = 0; i < 4; ++i)
                    if (Creature* Guard = me->SummonCreature(NPC_SUNWELL_DEFENDER, GuardLoc[i], TEMPSUMMON_TIMED_DESPAWN, 30000))
                        Guard->SetPhaseMask(2, true);

                if (Creature* Morlen = me->SummonCreature(NPC_MORLEN_GOLDGRIP, 11776.8f, -7050.72f, 25.2394f, 5.13752f, TEMPSUMMON_CORPSE_DESPAWN, 0))
                {
                    Morlen->Mount(DISPLAY_MOUNT);
                    Morlen->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    Morlen->SetPhaseMask(2, true);
                    Morlen->SetReactState(REACT_PASSIVE);
                    uiMorlen = Morlen->GetGUID();
                }
                events.ScheduleEvent(EVENT_STEP_2, 0.1 * IN_MILLISECONDS);
                break;
            case EVENT_STEP_2:
                if (Creature* Thalorien = ObjectAccessor::GetCreature(*me, uiThalorien))
                    Thalorien->AI()->Talk(SAY_THALORIEN_1);
                events.ScheduleEvent(EVENT_STEP_3, 5 * IN_MILLISECONDS);
                break;
            case EVENT_STEP_3:
                if (Creature* Thalorien = ObjectAccessor::GetCreature(*me, uiThalorien))
                    Thalorien->AI()->Talk(SAY_THALORIEN_2);
                events.ScheduleEvent(EVENT_STEP_4, 5 * IN_MILLISECONDS);
                break;
            case EVENT_STEP_4:
                if (Creature* Thalorien = ObjectAccessor::GetCreature(*me, uiThalorien))
                    Thalorien->AI()->Talk(SAY_THALORIEN_3);
                events.ScheduleEvent(EVENT_STEP_5, 10 * IN_MILLISECONDS);
                break;
            case EVENT_STEP_5:
                if (Creature* Thalorien = ObjectAccessor::GetCreature(*me, uiThalorien))
                    Thalorien->AI()->Talk(SAY_THALORIEN_4);
                events.ScheduleEvent(EVENT_STEP_6, 6 * IN_MILLISECONDS);
                break;
            case EVENT_STEP_6:
                if (Creature* Thalorien = ObjectAccessor::GetCreature(*me, uiThalorien))
                    Thalorien->GetMotionMaster()->MovePoint(0, 11787.3f, -7064.11f, 25.8395f);
                events.ScheduleEvent(EVENT_STEP_7, 6 * IN_MILLISECONDS);
                break;
            case EVENT_STEP_7:
                if (Creature* Thalorien = ObjectAccessor::GetCreature(*me, uiThalorien))
                    Thalorien->AI()->Talk(SAY_THALORIEN_5);
                events.ScheduleEvent(EVENT_STEP_8, 9 * IN_MILLISECONDS);
                break;
            case EVENT_STEP_8:
                if (Creature* Thalorien = ObjectAccessor::GetCreature(*me, uiThalorien))
                    Thalorien->AI()->Talk(SAY_THALORIEN_6);
                if (Creature* Morlen = ObjectAccessor::GetCreature(*me, uiMorlen))
                    Morlen->CastSpell(Morlen, SPELL_BLOOD_PRESENCE, true);
                events.ScheduleEvent(EVENT_STEP_9, 9 * IN_MILLISECONDS);
                break;
            case EVENT_STEP_9:
                if (Creature* Morlen = ObjectAccessor::GetCreature(*me, uiMorlen))
                    Morlen->AI()->Talk(SAY_MORLEN_1);
                events.ScheduleEvent(EVENT_STEP_10, 9 * IN_MILLISECONDS);
                break;
                // Intro 
            case EVENT_STEP_10:
                if (Creature* Morlen = ObjectAccessor::GetCreature(*me, uiMorlen))
                    Morlen->AI()->Talk(SAY_MORLEN_2);
                if (Creature* Thalorien = ObjectAccessor::GetCreature(*me, uiThalorien))
                    Thalorien->SetHomePosition(Thalorien->GetPositionX(), Thalorien->GetPositionY(), Thalorien->GetPositionZ(), Thalorien->GetOrientation());
                SummonWave(NPC_SCOURGE_ZOMBIE);
                events.ScheduleEvent(EVENT_STEP_11, 30 * IN_MILLISECONDS);
                break;
            case EVENT_STEP_11:
                if (Creature* Morlen = ObjectAccessor::GetCreature(*me, uiMorlen))
                    Morlen->AI()->Talk(SAY_MORLEN_3);
                SummonWave(NPC_GHOUL_INVADER);
                events.ScheduleEvent(EVENT_STEP_12, 30 * IN_MILLISECONDS);
                break;
            case EVENT_STEP_12:
                if (Creature* Morlen = ObjectAccessor::GetCreature(*me, uiMorlen))
                    Morlen->AI()->Talk(SAY_MORLEN_4);
                SummonWave(NPC_CRYPT_RAIDER);
                events.ScheduleEvent(EVENT_STEP_13, 30 * IN_MILLISECONDS);
                break;
            case EVENT_STEP_13:
                if (Creature* Morlen = ObjectAccessor::GetCreature(*me, uiMorlen))
                    Morlen->AI()->Talk(SAY_MORLEN_5);
                events.ScheduleEvent(EVENT_STEP_14, 1 * IN_MILLISECONDS);
                break;
            case EVENT_STEP_14:
                if (Creature* Thalorien = ObjectAccessor::GetCreature(*me, uiThalorien))
                {
                    if (Creature* Morlen = ObjectAccessor::GetCreature(*me, uiMorlen))
                    {
                        Morlen->SetReactState(REACT_AGGRESSIVE);
                        AddThreat(Thalorien, 100, Morlen);
                        Morlen->AI()->AttackStart(Thalorien);
                        Morlen->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    }
                }
                break;
                // Outro
            case EVENT_STEP_15:
                if (Creature* Thalorien = ObjectAccessor::GetCreature(*me, uiThalorien))
                    Thalorien->AI()->Talk(SAY_THALORIEN_7);
                events.ScheduleEvent(EVENT_STEP_16, 5 * IN_MILLISECONDS);
                break;
            case EVENT_STEP_16:
                if (Creature* Thalorien = ObjectAccessor::GetCreature(*me, uiThalorien))
                    Thalorien->AI()->Talk(SAY_THALORIEN_8);
                events.ScheduleEvent(EVENT_STEP_17, 5 * IN_MILLISECONDS);
                break;
            case EVENT_STEP_17:
                if (Creature* Thalorien = ObjectAccessor::GetCreature(*me, uiThalorien))
                    Thalorien->AI()->Talk(SAY_THALORIEN_9);
                events.ScheduleEvent(EVENT_STEP_18, 6 * IN_MILLISECONDS);
                break;
            case EVENT_STEP_18:
                if (Creature* Thalorien = ObjectAccessor::GetCreature(*me, uiThalorien))
                    Thalorien->AI()->Talk(SAY_THALORIEN_10);
                events.ScheduleEvent(EVENT_STEP_19, 12 * IN_MILLISECONDS);
                break;
            case EVENT_STEP_19:
                if (Creature* Thalorien = ObjectAccessor::GetCreature(*me, uiThalorien))
                    Thalorien->DisappearAndDie();
                if (Player* player = ObjectAccessor::GetPlayer(*me, uiPlayer))
                    player->KilledMonsterCredit(NPC_QUEST_CREDIT, player->GetGUID());
                Reset();
                break;
            default:
                break;
            }

            DoMeleeAttackIfReady();
        }

        void SummonWave(uint32 entry)
        {
            if (Creature* Thalorien = ObjectAccessor::GetCreature(*me, uiThalorien))
            {
                for (int i = 0; i < 5; ++i)
                    if (Creature* Zombie = me->SummonCreature(entry, ZombieLoc[i], TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000))
                    {
                        Zombie->SetPhaseMask(2, true);
                        AddThreat(Thalorien, 100.0f, Zombie);
                        Zombie->AI()->AttackStart(Thalorien);
                    }
            }
        }

        void SummonedCreatureDespawn(Creature* summon)
        {
            if (summon->GetEntry() == NPC_THALORIEN_DAWNSEEKER)
            {
                Reset();
                return;
            }

            if (summon->GetEntry() == NPC_MORLEN_GOLDGRIP)
                events.ScheduleEvent(EVENT_STEP_15, 3 * IN_MILLISECONDS);
        }

        void SetGUID(ObjectGuid guid, int32 /*id*/) override
        {
            uiPlayer = guid;
        }

        void DoAction(int32 action) override
        {
            switch (action)
            {
            case ACTION_START_QUEST:
                me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                events.ScheduleEvent(EVENT_STEP_1, 0);
                break;
            }
        }
    private:
        EventMap events;
        ObjectGuid uiThalorien;
        ObjectGuid uiPlayer;
        ObjectGuid uiMorlen;
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_thalorien_dawnseekerAI(creature);
    }
};

class npc_sunwell_warder : public CreatureScript
{
public:
    npc_sunwell_warder() : CreatureScript("npc_sunwell_warder") { }

    struct npc_sunwell_warderAI : public ScriptedAI
    {
        npc_sunwell_warderAI(Creature* creature) : ScriptedAI(creature) { }

        bool GossipHello(Player* player) override
        {
            player->PrepareGossipMenu(me, 0);
            if (player->HasItemCount(49879, 1) || player->HasItemCount(49889, 1))
                AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Take me to the Sunwell", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->SendPreparedGossip(me);
            return true;
        }

        bool GossipSelect(Player* player, uint32 /*uiSender*/, uint32 gossipListId) override
        {
            uint32 const action = player->PlayerTalkClass->GetGossipOptionAction(gossipListId);
            player->PlayerTalkClass->ClearMenus();
            switch (action)
            {
            case GOSSIP_ACTION_INFO_DEF + 1:
                CloseGossipMenuFor(player);
                player->SetGameMaster(true);
                player->TeleportTo(580, 1728.5f, 709.219f, 71.1905f, 2.78676f);
                player->SetGameMaster(false);
                player->SetPhaseMask(2, true);
                break;
            default:
                return false; // nothing defined -> trinity core handling
            }
            return true; // no default handling -> prevent trinity core handling
        }

    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_sunwell_warderAI(creature);
    }
};

enum Says
{
    SAY_QUELDELAR_1 = 11,  // Damas y Caballeros, les presento a $N, portador de Quel'Delar.
    SAY_QUELDELAR_2 = 0,  // Is it true that this is Quel\'Delar?
    SAY_QUELDELAR_3 = 1,  // We will see.
    SAY_QUELDELAR_4 = 2,  // Look Lor\'themar! It is Quel\'Delar without a doubt.
    SAY_QUELDELAR_5 = 3,  // So be it. You have my thanks, $N, for returning Quel\'Delar to its rightful owners
    SAY_QUELDELAR_6 = 4,  // What means this treason?
    SAY_QUELDELAR_7 = 5,  // Drop the weapon and surrender, traitor.
    SAY_QUELDELAR_8 = 6,  // This is not my fault, Rommath. It is not a treason.
    SAY_QUELDELAR_9 = 7,  // Remove your men. The stupidity of Lor\'themar himself caused his wounds. Quel\'Delar is not chosen, it chooses it\'s master.
    SAY_QUELDELAR_10 = 8,  // Guards, return to your posts
    SAY_QUELDELAR_11 = 9,  // You will have what you seek, $N. Take the sword and leave. And your Auric, be careful what you say in this sacred place.
    SAY_QUELDELAR_12 = 10,  // Take the sword through this portal into Dalaran, $N. You have done what many quel\'dorei have dreamed for years. At last Quel\'Delar is restored.
};

enum QuelDelarEvents
{
    EVENT_QUEST_STEP_1 = 1,
    EVENT_QUEST_STEP_2 = 2,
    EVENT_QUEST_STEP_3 = 3,
    EVENT_QUEST_STEP_4 = 4,
    EVENT_QUEST_STEP_5 = 5,
    EVENT_QUEST_STEP_6 = 6,
    EVENT_QUEST_STEP_7 = 7,
    EVENT_QUEST_STEP_8 = 8,
    EVENT_QUEST_STEP_9 = 9,
    EVENT_QUEST_STEP_10 = 10,
    EVENT_QUEST_STEP_11 = 11,
    EVENT_QUEST_STEP_12 = 12,
    EVENT_QUEST_STEP_13 = 13,
    EVENT_QUEST_STEP_14 = 14,
    EVENT_QUEST_STEP_15 = 15,
    EVENT_QUEST_STEP_16 = 16
};

enum QuelDelarActions
{
    ACTION_START_EVENT = 1
};

enum QuelDelarCreatures
{
    NPC_ROMMATH = 37763,
    NPC_THERON = 37764,
    NPC_AURIC = 37765,
    NPC_QUEL_GUARD = 37781,
    NPC_CASTER_BUNNY = 37746
};

enum QuelDelarGameobjects
{
    GO_QUEL_DANAR = 201794
};
enum QuelDelarMisc
{
    ITEM_TAINTED_QUELDANAR_1 = 49879,
    ITEM_TAINTED_QUELDANAR_2 = 49889,
    SPELL_WRATH_QUEL_DANAR = 70493,
    SPELL_ICY_PRISON = 70540
};

/*######
## npc_queldelar_sunwell_plateau
######*/
int swordkj;
class item_tainted_queldelar : public ItemScript
{
public:
    item_tainted_queldelar() : ItemScript("item_tainted_queldelar") { }
    bool OnUse(Player* player, Item* item, SpellCastTargets const& /*targets*/)
    {
        InstanceScript *instance = player->GetInstanceScript();
        if (instance && player->FindNearestCreature(NPC_CASTER_BUNNY, 20.0f, true))
        {

            if (Creature *introducer = player->FindNearestCreature(NPC_CASTER_BUNNY, 20.0f, true))
            {
                introducer->Kill(player->FindNearestCreature(NPC_CASTER_BUNNY, 20.0f, true));
            }
            return true;
        }

        else
            return false;
    }
};

class npc_queldelar_sunwell_plateau : public CreatureScript
{
public:
    npc_queldelar_sunwell_plateau() : CreatureScript("npc_queldelar_sunwell_plateau") { }

    struct npc_queldelar_sunwell_plateauAI : public ScriptedAI
    {
        npc_queldelar_sunwell_plateauAI(Creature* creature) : ScriptedAI(creature) { }
        int i = 0;
        void MoveInLineOfSight(Unit * who) override
        {
            if (!who)
                return;

            if (who && me->IsWithinDistInMap(who, 150.0f) && me->FindNearestCreature(NPC_CASTER_BUNNY, 200.0f, false) && i == 0)
            {
                DoAction(ACTION_START_EVENT);
                i = 1;
                return;
            }
        }

        void Reset() override
        {
            me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            events.Reset();
        }

        void DoAction(int32 action) override
        {
            switch (action)
            {
            case ACTION_START_EVENT:
                me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                events.ScheduleEvent(EVENT_QUEST_STEP_1, 0);
                break;
            }
        }

        void UpdateAI(uint32 diff) override
        {
            events.Update(diff);

            switch (events.ExecuteEvent())
            {
            case EVENT_QUEST_STEP_1:
                me->Yell(SAY_QUELDELAR_1);
                if (Creature* rommath = me->FindNearestCreature(NPC_ROMMATH, 100.0f, true))  // Rommath
                    uiRommath = rommath->GetGUID();

                if (Creature* theron = me->FindNearestCreature(NPC_THERON, 100.0f, true))    // Lor'Themar Theron
                    uiTheron = theron->GetGUID();

                if (Creature* auric = me->FindNearestCreature(NPC_AURIC, 100.0f, true))      // Auric
                    uiAuric = auric->GetGUID();

                if (GameObject* quelDelar = me->SummonGameObject(GO_QUEL_DANAR, 1683.99f, 620.231f, 29.3599f, 0.410932f, QuaternionData(), 0))
                {
                    uiQuelDelar = quelDelar->GetGUID();
                    quelDelar->SetFlag(GAMEOBJECT_FLAGS, 5);
                }

                if (Player* player = me->SelectNearestPlayer(200.0f))
                {
                    player->DestroyItemCount(ITEM_TAINTED_QUELDANAR_1, 1, true);
                    player->DestroyItemCount(ITEM_TAINTED_QUELDANAR_2, 1, true);
                }
                events.ScheduleEvent(EVENT_QUEST_STEP_2, 2 * IN_MILLISECONDS);
                break;
            case EVENT_QUEST_STEP_2:
                if (Creature* guard = me->FindNearestCreature(NPC_QUEL_GUARD, 100.0f, true))
                    guard->AI()->Talk(SAY_QUELDELAR_2);
                events.ScheduleEvent(EVENT_QUEST_STEP_3, 1 * IN_MILLISECONDS);
                break;
            case EVENT_QUEST_STEP_3:
                if (Creature* theron = ObjectAccessor::GetCreature(*me, uiTheron))
                    theron->AI()->Talk(SAY_QUELDELAR_3);
                events.ScheduleEvent(EVENT_QUEST_STEP_4, 4 * IN_MILLISECONDS);
                break;
            case EVENT_QUEST_STEP_4:
                if (Creature* rommath = ObjectAccessor::GetCreature(*me, uiRommath))
                    rommath->GetMotionMaster()->MovePoint(1, 1675.8f, 617.19f, 28.0504f);
                if (Creature*auric = ObjectAccessor::GetCreature(*me, uiAuric))
                    auric->GetMotionMaster()->MovePoint(1, 1681.77f, 612.084f, 28.4409f);
                events.ScheduleEvent(EVENT_QUEST_STEP_5, 6 * IN_MILLISECONDS);
                break;
            case EVENT_QUEST_STEP_5:
                if (Creature* rommath = ObjectAccessor::GetCreature(*me, uiRommath))
                {
                    rommath->SetOrientation(0.3308f);
                    rommath->AI()->Talk(SAY_QUELDELAR_4);
                }
                if (Creature* auric = ObjectAccessor::GetCreature(*me, uiAuric))
                    auric->SetOrientation(1.29057f);
                if (Creature* theron = ObjectAccessor::GetCreature(*me, uiTheron))
                    theron->GetMotionMaster()->MovePoint(1, 1677.07f, 613.122f, 28.0504f);
                events.ScheduleEvent(EVENT_QUEST_STEP_6, 10 * IN_MILLISECONDS);
                break;
            case EVENT_QUEST_STEP_6:
                if (Creature* theron = ObjectAccessor::GetCreature(*me, uiTheron))
                {
                    theron->AI()->Talk(SAY_QUELDELAR_5);
                    theron->GetMotionMaster()->MovePoint(1, 1682.3f, 618.459f, 27.9581f);
                }
                events.ScheduleEvent(EVENT_QUEST_STEP_7, 4 * IN_MILLISECONDS);
                break;
            case EVENT_QUEST_STEP_7:
                if (Creature* theron = ObjectAccessor::GetCreature(*me, uiTheron))
                    theron->HandleEmoteCommand(EMOTE_ONESHOT_POINT);
                events.ScheduleEvent(EVENT_QUEST_STEP_8, 0.8 * IN_MILLISECONDS);
                break;
            case EVENT_QUEST_STEP_8:
                if (Creature* theron = ObjectAccessor::GetCreature(*me, uiTheron))
                    theron->CastSpell(theron, SPELL_WRATH_QUEL_DANAR, true);
                events.ScheduleEvent(EVENT_QUEST_STEP_9, 1 * IN_MILLISECONDS);
                break;
            case EVENT_QUEST_STEP_9:
                if (Creature* rommath = ObjectAccessor::GetCreature(*me, uiRommath))
                {
                    if (Player* player = me->SelectNearestPlayer(200.0f))
                        rommath->AddAura(SPELL_ICY_PRISON, player);
                    rommath->AI()->Talk(SAY_QUELDELAR_6);
                }
                if (Creature* guard = me->FindNearestCreature(NPC_QUEL_GUARD, 200.0f))
                {
                    guard->GetMotionMaster()->MovePoint(0, 1681.1f, 614.955f, 28.4983f);
                    guard->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_READY1H);
                }
                events.ScheduleEvent(EVENT_QUEST_STEP_10, 3 * IN_MILLISECONDS);
                break;
            case EVENT_QUEST_STEP_10:
                if (Creature* guard = me->FindNearestCreature(NPC_QUEL_GUARD, 200.0f))
                    guard->AI()->Talk(SAY_QUELDELAR_7);
                events.ScheduleEvent(EVENT_QUEST_STEP_11, 2 * IN_MILLISECONDS);
                break;
            case EVENT_QUEST_STEP_11:
                if (Creature* auric = ObjectAccessor::GetCreature(*me, uiAuric))
                    auric->AI()->Talk(SAY_QUELDELAR_8);
                events.ScheduleEvent(EVENT_QUEST_STEP_12, 6 * IN_MILLISECONDS);
                break;
            case EVENT_QUEST_STEP_12:
                if (Creature* auric = ObjectAccessor::GetCreature(*me, uiAuric))
                    auric->AI()->Talk(SAY_QUELDELAR_9);
                events.ScheduleEvent(EVENT_QUEST_STEP_13, 5 * IN_MILLISECONDS);
                break;
            case EVENT_QUEST_STEP_13:
                if (Creature* rommath = ObjectAccessor::GetCreature(*me, uiRommath))
                    rommath->AI()->Talk(SAY_QUELDELAR_10);
                events.ScheduleEvent(EVENT_QUEST_STEP_14, 2 * IN_MILLISECONDS);
                break;
            case EVENT_QUEST_STEP_14:
                if (Creature* guard = me->FindNearestCreature(NPC_QUEL_GUARD, 200.0f))
                {
                    guard->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_STAND);
                    guard->GetMotionMaster()->MovePoint(0, guard->GetHomePosition());
                }
                if (Creature* rommath = ObjectAccessor::GetCreature(*me, uiRommath))
                {
                    rommath->HandleEmoteCommand(EMOTE_ONESHOT_POINT);
                    rommath->AI()->Talk(SAY_QUELDELAR_11);
                }
                events.ScheduleEvent(EVENT_QUEST_STEP_15, 7 * IN_MILLISECONDS);
                break;
            case EVENT_QUEST_STEP_15:
                if (Creature* auric = ObjectAccessor::GetCreature(*me, uiAuric))
                {
                    auric->HandleEmoteCommand(EMOTE_ONESHOT_POINT);
                    auric->AI()->Talk(SAY_QUELDELAR_12);
                    if (GameObject* quelDelar = me->FindNearestGameObject(GO_QUEL_DANAR, 100.0f))
                        quelDelar->RemoveFlag(GAMEOBJECT_FLAGS, 5);
                }
                events.ScheduleEvent(EVENT_QUEST_STEP_16, 2 * IN_MILLISECONDS);
                break;
            case EVENT_QUEST_STEP_16:
                if (Creature* auric = ObjectAccessor::GetCreature(*me, uiAuric))
                    auric->GetMotionMaster()->MovePoint(0, auric->GetHomePosition());
                if (Creature* rommath = ObjectAccessor::GetCreature(*me, uiRommath))
                    rommath->GetMotionMaster()->MovePoint(0, rommath->GetHomePosition());
                if (Creature* theron = ObjectAccessor::GetCreature(*me, uiTheron))
                    theron->DespawnOrUnsummon(5 * IN_MILLISECONDS);
                break;
            default:
                break;
            }
        }
    private:
        EventMap events;
        ObjectGuid uiRommath;
        ObjectGuid uiTheron;
        ObjectGuid uiAuric;
        ObjectGuid uiQuelDelar;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_queldelar_sunwell_plateauAI(creature);
    }
};

class go_dalaran_portal_sunwell : public GameObjectScript
{
public:
    go_dalaran_portal_sunwell() : GameObjectScript("go_dalaran_portal_sunwell") {}

    struct go_dalaran_portal_sunwellAI : public GameObjectAI
    {
        go_dalaran_portal_sunwellAI(GameObject* go) : GameObjectAI(go) { }

        bool GossipHello(Player* player) override
        {
            player->PrepareGossipMenu(me, 0);
            player->TeleportTo(571, 5804.149902f, 624.770996f, 647.767029f, 1.640000f);
            player->SetPhaseMask(1, true);
            return true;
        }
    };

    GameObjectAI* GetAI(GameObject* go) const override
    {
        return new go_dalaran_portal_sunwellAI(go);
    }
};

void AddSC_isle_of_queldanas()
{
    new go_dalaran_portal_sunwell();
    new npc_queldelar_sunwell_plateau();
    new item_tainted_queldelar();
    new npc_sunwell_warder();
    new npc_greengill_slave();
    new npc_thalorien_dawnseeker();
}
