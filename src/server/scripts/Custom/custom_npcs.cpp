#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "CreatureAI.h"
#include "CreatureTextMgr.h"
#include "Player.h"
#include "ReputationMgr.h"
#include "ObjectMgr.h"
#include "DBCStores.h"

bool CompleteQuest(Player* player, uint32 quest_id)
{
    Quest const* quest = sObjectMgr->GetQuestTemplate(quest_id);

    // If player doesn't have the quest
    if (!quest || player->GetQuestStatus(quest_id) == QUEST_STATUS_NONE)
        return false;

    // Add quest items for quests that require items
    for (uint8 x = 0; x < QUEST_ITEM_OBJECTIVES_COUNT; ++x)
    {
        uint32 id = quest->RequiredItemId[x];
        uint32 count = quest->RequiredItemCount[x];
        if (!id || !count)
            continue;

        uint32 curItemCount = player->GetItemCount(id, true);

        ItemPosCountVec dest;
        uint8 msg = player->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, id, count - curItemCount);
        if (msg == EQUIP_ERR_OK)
        {
            Item* item = player->StoreNewItem(dest, id, true);
            player->SendNewItem(item, count - curItemCount, true, false);
        }
    }

    // All creature/GO slain/cast (not required, but otherwise it will display "Creature slain 0/10")
    for (uint8 i = 0; i < QUEST_OBJECTIVES_COUNT; ++i)
    {
        int32 creature = quest->RequiredNpcOrGo[i];
        uint32 creatureCount = quest->RequiredNpcOrGoCount[i];

        if (creature > 0)
        {
            if (CreatureTemplate const* creatureInfo = sObjectMgr->GetCreatureTemplate(creature))
                for (uint16 z = 0; z < creatureCount; ++z)
                    player->KilledMonster(creatureInfo, ObjectGuid::Empty);
        }
        else if (creature < 0)
            for (uint16 z = 0; z < creatureCount; ++z)
                player->KillCreditGO(creature);
    }

    // If the quest requires reputation to complete
    if (uint32 repFaction = quest->GetRepObjectiveFaction())
    {
        uint32 repValue = quest->GetRepObjectiveValue();
        uint32 curRep = player->GetReputationMgr().GetReputation(repFaction);
        if (curRep < repValue)
            if (FactionEntry const* factionEntry = sFactionStore.LookupEntry(repFaction))
                player->GetReputationMgr().SetReputation(factionEntry, repValue);
    }

    // If the quest requires a SECOND reputation to complete
    if (uint32 repFaction = quest->GetRepObjectiveFaction2())
    {
        uint32 repValue2 = quest->GetRepObjectiveValue2();
        uint32 curRep = player->GetReputationMgr().GetReputation(repFaction);
        if (curRep < repValue2)
            if (FactionEntry const* factionEntry = sFactionStore.LookupEntry(repFaction))
                player->GetReputationMgr().SetReputation(factionEntry, repValue2);
    }

    // If the quest requires money
    int32 ReqOrRewMoney = quest->GetRewOrReqMoney();
    if (ReqOrRewMoney < 0)
        player->ModifyMoney(-ReqOrRewMoney);

    return true;
}

/* Quest #11134 - The End of the Deserters */
/* NPC #23941 - Gavis greyshield */

#define QUEST_END_OF_DESERTERS 11134

enum GavisGreyshieldEvents
{
    TALK_1 = 1,
    TALK_2,
    DESPAWN
};

class GavisGreyshield_CreatureScript : CreatureScript
{
public:
    GavisGreyshield_CreatureScript() : CreatureScript("npc_gavis_greyshield") { }

    struct GavisGreyshield_ScriptedAI : ScriptedAI
    {
    public:
        GavisGreyshield_ScriptedAI(Creature* me) : ScriptedAI(me) { }

        void Reset() override
        {
            if (CreatureTemplate const* ci = sObjectMgr->GetCreatureTemplate(me->GetEntry()))
                me->SetFaction(ci->faction);

            _events.Reset();
        }

        void DamageTaken(Unit* /*attacker*/, uint32& amount) override
        {
            if (me->GetHealth() <= amount)
            {
                amount = 0;

                if (_events.Empty())
                {
                    auto &targets = me->GetThreatManager().getThreatList();

                    for (auto itr = targets.begin(); itr != targets.end(); itr++)
                    {
                        if (auto player = (*itr)->getTarget()->ToPlayer())
                            if (player->GetQuestStatus(QUEST_END_OF_DESERTERS) == QUEST_STATUS_INCOMPLETE)
                                CompleteQuest(player, QUEST_END_OF_DESERTERS);
                    }

                    me->SetHealth(1);
                    me->SetFaction(35);
                    me->AttackStop();
                    _events.ScheduleEvent(TALK_1, 1);
                }
            }
        }

        void UpdateAI(uint32 diff) override
        {
            if (_events.Empty() && !me->IsInCombat())
                return;

            _events.Update(diff);

            while (uint32 eventId = _events.ExecuteEvent())
            {
                if (eventId == TALK_1)
                {
                    sCreatureTextMgr->SendChat(me, 0);

                    _events.ScheduleEvent(TALK_2, 3000);
                }
                else if (eventId == TALK_2)
                {
                    sCreatureTextMgr->SendChat(me, 1);

                    _events.ScheduleEvent(DESPAWN, 10000);
                }
                else if (eventId == DESPAWN)
                {
                    me->DespawnOrUnsummon(0);
                }
            }

            if (me->GetFaction() != 35)
                DoMeleeAttackIfReady();
        }

    private:
        EventMap _events;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new GavisGreyshield_ScriptedAI(creature);
    }
};

/* Quest #1920 */

class npc_rift_spawn : CreatureScript
{
public:
    npc_rift_spawn() : CreatureScript("npc_fift_spawn_q1920") { }

    struct npc_rift_spawnAI : ScriptedAI
    {
    public:
        npc_rift_spawnAI(Creature* creature) : ScriptedAI(creature) { }

        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim())
                return;

            DoMeleeAttackIfReady();
        }

        void Reset() override
        {
            me->RemoveAllAuras();
            me->CastSpell(me, 34426, true);
        }

        void JustDied(Unit* killer) override
        {
            me->Respawn(true);
        }

        void DamageTaken(Unit*, uint32& amount) override
        {
            if (amount >= me->GetHealth())
            {
                amount = 0;
                me->SetHealth(1);

                if (!me->HasAura(9032))
                    me->CastSpell(me, 9032, true);
            }
        }

        void SpellHit(Unit* caster, SpellInfo const* spell) override
        {
            if (spell->Id == 9095)
            {
                if (me->IsInCombat())
                    return;

                me->RemoveAllAuras();
                //me->CastSpell(me, 9096, true);
                AttackStart(caster);
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_rift_spawnAI(creature);
    }
};

enum CustomBossSpells
{
    SPELL_BONE_SPIKE_GRAVEYARD = 69057,
    SPELL_FROSTBOLT_VOLLEY = 72905,
    SPELL_FROST_BOMB = 69845,
    SPELL_FROST_AURA = 70084,
    SPELL_NECROTIC_PLAGUE = 73914,
    SPELL_INFEST = 70541,
    SPELL_CHAIN_LIGHTNING = 28167,
    SPELL_LIFE_DRAIN = 28542,
};

enum CastTargetType
{
    RANDOM,
    VICTIM,
    SELF,
    NEAREST_AGGRO,
    FARTHEST_AGGRO,
    AOE
};

class npc_custom_worldboss : public CreatureScript
{
public:
    npc_custom_worldboss() : CreatureScript("npc_custom_worldboss") { }

    class CastInfo
    {
    public:
        CastInfo(uint32 id, uint32 min = 0, uint32 max = 0, CastTargetType t = RANDOM, uint32 ph = 0)
        {
            spellId = id;
            minTime = min * IN_MILLISECONDS;
            maxTime = max * IN_MILLISECONDS;
            target = t;
            phase = ph;
        }

        uint32 spellId;
        uint32 minTime;
        uint32 maxTime;
        CastTargetType target;
        uint32 phase;
    };

    struct npc_custom_worldbossAI : public CreatureAI
    {
    public:
        npc_custom_worldbossAI(Creature* c) : CreatureAI(c)
        {
            _spells.push_back(CastInfo(SPELL_BONE_SPIKE_GRAVEYARD, 5, 10, SELF));
            _spells.push_back(CastInfo(SPELL_FROSTBOLT_VOLLEY, 10, 20, AOE));
            _spells.push_back(CastInfo(SPELL_FROST_BOMB, 10, 20, AOE));
            _spells.push_back(CastInfo(SPELL_FROST_AURA, 0, 0, SELF));
            _spells.push_back(CastInfo(SPELL_NECROTIC_PLAGUE, 10, 15));
            _spells.push_back(CastInfo(SPELL_INFEST, 5, 10, SELF));
            _spells.push_back(CastInfo(SPELL_CHAIN_LIGHTNING, 3, 5));
            _spells.push_back(CastInfo(SPELL_LIFE_DRAIN, 3, 5));
        }

        void Reset() override
        {
            _events.Reset();
            me->RemoveAllAuras();

            for (uint32 i = 0; i < _spells.size(); i++)
                ScheludeEvent(i + 1);

            me->SetFloatValue(UNIT_MOD_CAST_SPEED, 0.1f);
        }

        void EnterCombat(Unit* victim) override
        {
            for (uint32 i = 0; i < _spells.size(); i++)
                ExecuteEvent(i + 1, true);
        }

        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim())
                return;

            _events.Update(diff);

            if (uint32 id = _events.ExecuteEvent())
                ExecuteEvent(id);

            DoMeleeAttackIfReady();
        }

    private:
        void ScheludeEvent(uint32 id)
        {
            if (id > _spells.size())
                return;

            auto &info = _spells[id - 1];

            if (info.maxTime)
                _events.ScheduleEvent(id, urand(info.minTime, info.maxTime));
        }

        SelectAggroTarget GetAggroTarget(CastTargetType type)
        {
            if (type == NEAREST_AGGRO)
                return SELECT_TARGET_MINDISTANCE;
            else if (type == FARTHEST_AGGRO)
                return SELECT_TARGET_MAXDISTANCE;

            return SELECT_TARGET_RANDOM;
        }

        void ExecuteEvent(uint32 id, bool onCombatStart = false)
        {
            if (id > _spells.size())
                return;

            auto &info = _spells[id - 1];

            if (!info.maxTime && !onCombatStart)
                return;

            if (info.target == AOE)
                DoCastAOE(info.spellId);
            else if (info.target == VICTIM)
                DoCastVictim(info.spellId);
            else
            {
                Unit* target;

                if (info.target == SELF)
                    target = me;
                else
                    target = SelectTarget(GetAggroTarget(info.target), 0, 50.0f, true);

                if (target)
                    DoCast(target, info.spellId);
            }

            ScheludeEvent(id);
        }

        std::vector<CastInfo> _spells;
        EventMap _events;
    };

    CreatureAI* GetAI(Creature* c) const override
    {
        return new npc_custom_worldbossAI(c);
    }
};

enum ImmortalSoulsSpells
{
    SPELL_SHADOW_BOLT          = 47610, 
    SPELL_SOUL_FLAY            = 45442, 
    SPELL_SHADOW_WORD_PAIN     = 72319, 
    SPELL_FINGER_OF_DEATH      = 31984, 
    SPELL_BLACK_PLAGUE         = 64153, 
    SPELL_HARVEST_SOUL         = 74325, 


    SPELL_SHROUD_OF_THE_OCCULT = 70768,  
    SPELL_PAIN_AND_SUFFERING   = 73790, 
    SPELL_SOUL_SICKNESS        = 69131, 
    SPELL_WAIL_OF_SOULS        = 70210, 
    SPELL_SOUL_SHRIEK          = 73802, 
    SPELL_STOP_TIME            = 60077, 

    SPELL_DARK_VOLLEY          = 63038, 
    SPELL_DEADEN               = 41410, 
    SPELL_SHADOW_BLAST         = 70866, 
    SPELL_SHADOW_SPIKE         = 46589, 
    SPELL_AURA_OF_DESIRE       = 41350, 
    SPELL_CLONE_PLAYER         = 57507, 
    SPELL_CLONE_WEAPON         = 41054, 

    SPELL_DRAIN_LIFE           = 70213, 
    SPELL_RUNE_SHIELD          = 41431, 
    SPELL_SHADOW_LANCE         = 72806, 
    SPELL_AURA_OF_SUFFERING    = 42017,
    SPELL_SINFUL_BEAM          = 40827, 
    SPELL_FEAR                 = 65809, 
    SPELL_SOULSTORM            = 68872,  
    SPELL_SOULSTORM_CHANNEL    = 69008,
    SPELL_SOULSTORM_VISUAL     = 68870,

    SPELL_SHADOWFORM           = 16592, 
    SPELL_SHADOW_CRASH         = 62660, 
    SPELL_REFLECTION_FIELD     = 10831, 
    SPELL_SHADOW_FISSURE       = 59127, 
    SPELL_SOUL_FRAGMENT        = 71905, 
    SPELL_SOUL_FRAGMENT_TIMER  = 6000, 
    SPELL_MENDING              = 2147, 
    SPELL_ARCANE_BEAM_PERIODIC = 51019,
    SPELL_PERIODIC_MANA_BURN   = 812, 
    SPELL_DEATH_AND_DECAY      = 71001, 
    SPELL_MORTAL_STRIKE        = 65926, 
    SPELL_VISUAL_1             = 72523, 
    SPELL_VISUAL_2             = 46228, 
    SPELL_VISUAL_3             = 60484
};

enum Events
{
    EVENT_SHADOW_BOLT = 1,
    EVENT_SOUL_FLAY = 2,
    EVENT_SHADOW_WORD_PAIN = 3,
    EVENT_FINGER_OF_DEATH = 4,
    EVENT_BLACK_PLAGUE = 5,
    EVENT_HARVEST_SOUL = 6,
    EVENT_SHROUD_OF_THE_OCCULT = 7,
    EVENT_PAIN_AND_SUFFERING = 8,
    EVENT_SOUL_SICKNESS = 9,
    EVENT_WAIL_OF_SOULS = 10,
    EVENT_SOUL_SHRIEK = 11,
    EVENT_STOP_TIME = 12,
    EVENT_DARK_VOLLEY = 13,
    EVENT_DEADEN = 14,
    EVENT_SHADOW_BLAST = 15,
    EVENT_SHADOW_SPIKE = 16,
    EVENT_AURA_OF_DESIRE = 17,
    EVENT_SEVERED_ESSENCE = 18,
    EVENT_DRAIN_LIFE = 19,
    EVENT_RUNE_SHIELD = 20,
    EVENT_SHADOW_LANCE = 21,
    EVENT_AURA_OF_SUFFERING = 22,
    EVENT_SINFUL_BEAM = 23,
    EVENT_FEAR = 24,
    EVENT_SOULSTORM = 25,
    EVENT_REFLECTION_FIELD = 26,
    EVENT_SHADOW_FISSURE = 27,

    EVENT_SOUL_FRAGMENT = 28,
    EVENT_GHOSTLY = 29,

    EVENT_SADNESS = 32,
    EVENT_ANGUISH = 33,
    EVENT_AFFLICTION = 34,
    EVENT_SORROW = 35,
    EVENT_DEATH = 36,
    EVENT_DEATH_AND = 37,
    EVENT_DEATH_AND_DECAY = 38,
    EVENT_MORTAL_STRIKE = 39,
    EVENT_MORTAL = 40,
    EVENT_MORTAL_STRIK = 41,
    EVENT_CHECK_RESET  = 42
};

#define TEXT_AGGRO_SADNESS "Вам конец,глупцы!!!"
#define TEXT_ON_SLAY "Умрите,ничтожества!"

Creature *severed_essence;

enum Phases
{
    SADNESS_PHASE = 1,
    ANGUISH_PHASE = 2,
    AFFLICTION_PHASE = 3,
    SORROW_PHASE = 4
};

uint32 MORPH_WOWGAME;

class boss_wowgame : public CreatureScript
{
public:
    boss_wowgame() : CreatureScript("boss_wowgame") { }


    CreatureAI* GetAI(Creature* creature) const override
    {
        return new boss_wowgameAI(creature);
    }

    struct boss_wowgameAI : public ScriptedAI
    {
        boss_wowgameAI(Creature* creature) : ScriptedAI(creature) { }

        void SetMorph(uint32 morph)
        {
            if (morph == MORPH_WOWGAME)
                return;
            MORPH_WOWGAME = morph;
            me->SetDisplayId(morph);
            return;
        }

        EventMap events;

        bool _sadness;
        bool _anguish;
        bool _affliction;
        bool _sorrow;
        bool _soulstormed;

        void Reset() override
        {
            events.Reset();
            _sadness = true;
            _anguish = false;
            _affliction = false;
            _sorrow = false;
            _soulstormed = false;
            events.SetPhase(SADNESS_PHASE);
            me->SetFloatValue(OBJECT_FIELD_SCALE_X, 1.8f);
            SetEquipmentSlots(false, 49623);
            me->RemoveAllAuras();
            me->SetReactState(REACT_AGGRESSIVE);

        }

        void EnterCombat(Unit* who) override
        {
            me->SetWalk(true);
            events.CancelEvent(EVENT_GHOSTLY);
            me->AddAura(SPELL_SHADOWFORM, me);
            events.SetPhase(SADNESS_PHASE);
            events.ScheduleEvent(EVENT_SOUL_FRAGMENT, 100);
            events.ScheduleEvent(EVENT_SADNESS, 100);
        }

        void DamageTaken(Unit* attacker, uint32& damage) override
        {
            if (!_anguish && !_affliction && !_sorrow && !HealthAbovePct(75))
            {
                _anguish = true;
                events.ScheduleEvent(EVENT_ANGUISH, 100);
            }

            if (!_affliction && !_sorrow && !HealthAbovePct(50))
            {
                _affliction = true;
                events.ScheduleEvent(EVENT_AFFLICTION, 100);
            }

            if (!_sorrow && !HealthAbovePct(25))
            {
                events.ScheduleEvent(EVENT_SORROW, 100);
                _sorrow = true;
            }

            if (!_soulstormed && !HealthAbovePct(15))
            {
                _soulstormed = true;
                events.ScheduleEvent(EVENT_SOULSTORM, 100);
            }
        }

        void JustSummoned(Creature* summon) override
        {
            switch (summon->GetEntry())
            {
            case 100021:
            {
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 40.0f, true))
                {
                    summon->CastSpell(target, 69091, false);
                }
                else
                    summon->DespawnOrUnsummon();
                break;
            }
            default:
                break;
            }
        }

        void KilledUnit(Unit* /*who*/) override
        {
            me->Yell(TEXT_ON_SLAY, LANG_UNIVERSAL);
        }

        void JustDied(Unit* /*killer*/) override
        {
            events.Reset();
            me->RemoveAllAuras();
        }

        void UpdateAI(uint32 diff) override
        {
            bool live = me && me->IsAlive();
            if (!live)
            {
                events.Reset();
                return;
            }

            if (me->GetAuraCount(SPELL_SOULSTORM_CHANNEL) < 1)
            {
                if (!UpdateVictim())
                    me->CastSpell(me, SPELL_SOULSTORM_CHANNEL, live);
            }

            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_SOUL_FRAGMENT:
                    if (me->GetAuraCount(SPELL_SOUL_FRAGMENT) < 9)
                    {
                        events.CancelEvent(EVENT_SOUL_FRAGMENT);
                        DoCast(me, SPELL_SOUL_FRAGMENT);
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                        DoCast(target, 69145);
                    }
                    events.ScheduleEvent(EVENT_SOUL_FRAGMENT, SPELL_SOUL_FRAGMENT_TIMER);
                    break;

                case EVENT_SHADOW_BOLT: 
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                        DoCast(target, SPELL_SHADOW_BOLT, true);
                    events.ScheduleEvent(EVENT_SHADOW_BOLT, urand(15400, 16100), 0, SADNESS_PHASE);
                    break;

                case EVENT_DEATH:
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                        DoCast(target, SPELL_DEATH_AND_DECAY, true);
                    events.ScheduleEvent(EVENT_DEATH, urand(200, 54100), 0, SADNESS_PHASE);
                    break;

                case EVENT_MORTAL:
                    DoCast(me->GetVictim(), SPELL_MORTAL_STRIKE, true);
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                    DoCast(target, 69145);
                    events.ScheduleEvent(EVENT_MORTAL, urand(20, 22900), 0, SADNESS_PHASE);
                    break;

                case EVENT_SOUL_FLAY:
                    DoCast(me->GetVictim(), SPELL_SOUL_FLAY, true);
                    events.ScheduleEvent(EVENT_SOUL_FLAY, urand(22000, 22900), 0, SADNESS_PHASE);
                    break;

                case EVENT_SHADOW_WORD_PAIN: 
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                        DoCast(target, SPELL_SHADOW_WORD_PAIN, true);
                    events.ScheduleEvent(EVENT_SHADOW_WORD_PAIN, urand(13400, 14200), 0, SADNESS_PHASE);
                    break;

                case EVENT_FINGER_OF_DEATH:
                    DoCast(me->GetVictim(), SPELL_FINGER_OF_DEATH);
                    events.ScheduleEvent(EVENT_FINGER_OF_DEATH, urand(17000, 17700), 0, SADNESS_PHASE);
                    break;

                case EVENT_BLACK_PLAGUE: 
                    DoCastAOE(SPELL_BLACK_PLAGUE);
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                        DoCast(target, 69145);
                    events.ScheduleEvent(EVENT_BLACK_PLAGUE, urand(35400, 36300), 0, SADNESS_PHASE);
                    break;

                case EVENT_HARVEST_SOUL:
                    DoCast(me->GetVictim(), SPELL_HARVEST_SOUL);
                    events.ScheduleEvent(EVENT_HARVEST_SOUL, urand(41200, 43100), 0, SADNESS_PHASE);
                    break;

                case EVENT_SHROUD_OF_THE_OCCULT:
                    me->AddAura(SPELL_SHROUD_OF_THE_OCCULT, me);
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                    DoCast(target, 62696);
                    events.ScheduleEvent(EVENT_SHROUD_OF_THE_OCCULT, 80000, 0, ANGUISH_PHASE);
                    break;

                case EVENT_PAIN_AND_SUFFERING: 
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                        DoCast(target, SPELL_PAIN_AND_SUFFERING);
                    events.ScheduleEvent(EVENT_PAIN_AND_SUFFERING, urand(15600, 16200), 0, ANGUISH_PHASE);
                    break;

                case EVENT_SOUL_SICKNESS:
                    DoCast(me->GetVictim(), SPELL_SOUL_SICKNESS);
                    events.ScheduleEvent(EVENT_SOUL_SICKNESS, urand(6200, 6800), 0, ANGUISH_PHASE);
                    break;

                case EVENT_WAIL_OF_SOULS: 
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                        DoCast(target, SPELL_WAIL_OF_SOULS);
                    events.ScheduleEvent(EVENT_WAIL_OF_SOULS, urand(11400, 12300), 0, ANGUISH_PHASE);
                    break;

                case EVENT_SOUL_SHRIEK: 
                    DoCastAOE(SPELL_SOUL_SHRIEK);
                    events.ScheduleEvent(EVENT_SOUL_SHRIEK, urand(22400, 23300), 0, ANGUISH_PHASE);
                    break;

                case EVENT_STOP_TIME: 
                    DoCastAOE(SPELL_STOP_TIME);
                    events.ScheduleEvent(EVENT_STOP_TIME, urand(48200, 54100), 0, ANGUISH_PHASE);
                    break;

                case EVENT_DEATH_AND:
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                        DoCast(target, SPELL_DEATH_AND_DECAY, true);
                    events.ScheduleEvent(EVENT_DEATH_AND, urand(200, 54100), 0, ANGUISH_PHASE);
                    break;

                case EVENT_DARK_VOLLEY:
                    DoCast(me->GetVictim(), SPELL_DARK_VOLLEY);
                    events.ScheduleEvent(EVENT_DARK_VOLLEY, urand(16100, 16900), 0, AFFLICTION_PHASE);
                    break;

                case EVENT_DEADEN:
                    DoCast(me->GetVictim(), SPELL_DEADEN);
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                        DoCast(target, 62696);
                    events.ScheduleEvent(EVENT_DEADEN, urand(9200, 9800), 0, AFFLICTION_PHASE);
                    break;

                case EVENT_SHADOW_BLAST:
                    DoCastAOE(SPELL_SHADOW_BLAST);
                    events.ScheduleEvent(EVENT_SHADOW_BLAST, urand(49200, 55100), 0, AFFLICTION_PHASE);
                    break;

                case EVENT_SHADOW_SPIKE: 
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                        DoCast(target, SPELL_SHADOW_SPIKE);
                    events.ScheduleEvent(EVENT_SHADOW_SPIKE, urand(25400, 27300), 0, AFFLICTION_PHASE);
                    break;

                case EVENT_MORTAL_STRIK:
                    DoCast(me->GetVictim(), SPELL_MORTAL_STRIKE, true);
                    events.ScheduleEvent(EVENT_MORTAL_STRIK, urand(20, 22900), 0, AFFLICTION_PHASE);
                    break;

                case EVENT_SEVERED_ESSENCE:
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 20.0f, true))
                    {
                        severed_essence = me->SummonCreature(100021, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), target->GetOrientation(), TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 120000);
                        target->CastSpell(severed_essence, SPELL_CLONE_PLAYER, true); // 63793
                        target->CastSpell(severed_essence, SPELL_CLONE_WEAPON, true);
                        me->CastSpell(severed_essence, 70499, true); // Visual
                    }
                    events.ScheduleEvent(EVENT_SEVERED_ESSENCE, 42000, 0, AFFLICTION_PHASE);
                    break;

                case EVENT_MORTAL_STRIKE:
                    DoCast(me->GetVictim(), SPELL_MORTAL_STRIKE);
                    events.ScheduleEvent(EVENT_MORTAL_STRIKE, urand(100, 13800), 0, SORROW_PHASE);
                    break;

                case EVENT_DRAIN_LIFE:
                    DoCast(me->GetVictim(), SPELL_DRAIN_LIFE);
                    events.ScheduleEvent(EVENT_DRAIN_LIFE, urand(12200, 13800), 0, SORROW_PHASE);
                    break;

                case EVENT_RUNE_SHIELD:
                    me->AddAura(SPELL_RUNE_SHIELD, me);
                    events.ScheduleEvent(EVENT_RUNE_SHIELD, 70000, 0, SORROW_PHASE);
                    break;

                case EVENT_SHADOW_LANCE: 
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                        DoCast(target, SPELL_SHADOW_LANCE);
                    events.ScheduleEvent(EVENT_SHADOW_LANCE, urand(22400, 23300), 0, SORROW_PHASE);
                    break;

                case EVENT_DEATH_AND_DECAY:
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                        DoCast(target, SPELL_DEATH_AND_DECAY, true);
                    events.ScheduleEvent(EVENT_DEATH_AND_DECAY, urand(200, 54100), 0, SORROW_PHASE);
                    break;

                case EVENT_SINFUL_BEAM: 
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                        DoCast(target, SPELL_SINFUL_BEAM);
                    events.ScheduleEvent(EVENT_SINFUL_BEAM, urand(14400, 15300), 0, SORROW_PHASE);
                    break;

                case EVENT_FEAR: 
                    DoCastAOE(SPELL_FEAR);
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                        DoCast(target, 62696);
                    events.ScheduleEvent(EVENT_FEAR, urand(49200, 55100), 0, SORROW_PHASE);
                    break;

                case EVENT_SOULSTORM:
                    me->GetMotionMaster()->MovePoint(0, Position(2932.348633f, -4794.806641f, 234.688721f));
                    me->RemoveAllAuras();
                    DoCastAOE(SPELL_SHADOW_CRASH);
                    me->CastSpell(me, SPELL_SOULSTORM_VISUAL, true);
                    me->CastSpell(me, SPELL_SOULSTORM, false);
                    me->CastSpell(me, SPELL_ARCANE_BEAM_PERIODIC, true);
                    me->CastSpell(me, SPELL_PERIODIC_MANA_BURN, true);
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                        DoCast(target, 62696);
                    events.ScheduleEvent(EVENT_REFLECTION_FIELD, urand(10100, 10700), 0, SORROW_PHASE);
                    events.ScheduleEvent(EVENT_SHADOW_FISSURE, urand(52400, 63300), 0, SORROW_PHASE);
                    break;

                case EVENT_REFLECTION_FIELD:
                    me->AddAura(SPELL_REFLECTION_FIELD, me);
                    events.ScheduleEvent(EVENT_REFLECTION_FIELD, urand(10100, 10700), 0, SORROW_PHASE);
                    break;

                case EVENT_SHADOW_FISSURE: 
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                        DoCast(target, SPELL_SHADOW_FISSURE);
                    events.ScheduleEvent(EVENT_SHADOW_FISSURE, urand(52400, 63300), 0, SORROW_PHASE);
                    break;

                case EVENT_SADNESS:
                    events.SetPhase(SADNESS_PHASE);
                    me->Yell(TEXT_AGGRO_SADNESS, LANG_UNIVERSAL);
                    me->CastSpell(me, SPELL_MENDING, live);
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                        DoCast(target, 62696);
                    events.ScheduleEvent(EVENT_MORTAL, urand(20, 22900), 0, SADNESS_PHASE);
                    events.ScheduleEvent(EVENT_DEATH, urand(200, 54100), 0, SADNESS_PHASE);
                    events.ScheduleEvent(EVENT_SHADOW_BOLT, urand(15400, 16100), 0, SADNESS_PHASE);
                    events.ScheduleEvent(EVENT_SOUL_FLAY, urand(22000, 22900), 0, SADNESS_PHASE);
                    events.ScheduleEvent(EVENT_SHADOW_WORD_PAIN, urand(13400, 14200), 0, SADNESS_PHASE);
                    events.ScheduleEvent(EVENT_FINGER_OF_DEATH, urand(17000, 17700), 0, SADNESS_PHASE);
                    events.ScheduleEvent(EVENT_BLACK_PLAGUE, urand(35400, 36300), 0, SADNESS_PHASE);
                    events.ScheduleEvent(EVENT_HARVEST_SOUL, urand(41200, 43100), 0, SADNESS_PHASE);
                    break;

                case EVENT_ANGUISH:
                    events.SetPhase(ANGUISH_PHASE);
                    me->RemoveAura(SPELL_MENDING);
                    me->CastSpell(me, SPELL_VISUAL_3, live);
                    SetMorph(30242);
                    me->SetFloatValue(OBJECT_FIELD_SCALE_X, 1.2f);
                    SetEquipmentSlots(false, 49709);
                    events.ScheduleEvent(EVENT_SHROUD_OF_THE_OCCULT, 60000, 0, ANGUISH_PHASE);
                    events.ScheduleEvent(EVENT_DEATH_AND, urand(200, 54100), 0, ANGUISH_PHASE);
                    events.ScheduleEvent(EVENT_PAIN_AND_SUFFERING, urand(15600, 16200), 0, ANGUISH_PHASE);
                    events.ScheduleEvent(EVENT_SOUL_SICKNESS, urand(6200, 6800), 0, ANGUISH_PHASE);
                    events.ScheduleEvent(EVENT_WAIL_OF_SOULS, urand(11400, 12300), 0, ANGUISH_PHASE);
                    events.ScheduleEvent(EVENT_SOUL_SHRIEK, urand(22400, 23300), 0, ANGUISH_PHASE);
                    events.ScheduleEvent(EVENT_STOP_TIME, urand(48200, 54100), 0, ANGUISH_PHASE);
                    break;

                case EVENT_AFFLICTION:
                    events.SetPhase(AFFLICTION_PHASE);
                    me->CastSpell(me, SPELL_AURA_OF_DESIRE, live);
                    me->CastSpell(me, SPELL_VISUAL_3, live);
                    SetMorph(18239);
                    me->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.5f);
                    SetEquipmentSlots(false, 49653);
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                        DoCast(target, 62696);
                    events.ScheduleEvent(EVENT_MORTAL_STRIK, urand(20, 22900), 0, AFFLICTION_PHASE);
                    events.ScheduleEvent(EVENT_DARK_VOLLEY, urand(16100, 16900), 0, AFFLICTION_PHASE);
                    events.ScheduleEvent(EVENT_DEADEN, urand(9200, 9800), 0, AFFLICTION_PHASE);
                    events.ScheduleEvent(EVENT_SHADOW_BLAST, urand(49200, 55100), 0, AFFLICTION_PHASE);
                    events.ScheduleEvent(EVENT_SHADOW_SPIKE, urand(25400, 27300), 0, AFFLICTION_PHASE);
                    events.ScheduleEvent(EVENT_SEVERED_ESSENCE, 42000, 0, AFFLICTION_PHASE);
                    break;
                    // Phase Sorrow
                case EVENT_SORROW:
                    events.SetPhase(SORROW_PHASE);
                    me->RemoveAura(SPELL_AURA_OF_DESIRE);
                    me->CastSpell(me, SPELL_AURA_OF_SUFFERING, live);
                    me->CastSpell(me, SPELL_VISUAL_3, live);
                    SetMorph(30169);
                    me->SetFloatValue(OBJECT_FIELD_SCALE_X, 1.2f);
                    SetEquipmentSlots(false, 49654);
                    events.ScheduleEvent(EVENT_DRAIN_LIFE, urand(12200, 13800), 0, SORROW_PHASE);
                    events.ScheduleEvent(EVENT_MORTAL_STRIKE, urand(100, 13800), 0, SORROW_PHASE);
                    events.ScheduleEvent(EVENT_RUNE_SHIELD, 70000, 0, SORROW_PHASE);
                    events.ScheduleEvent(EVENT_DEATH_AND_DECAY, urand(200, 54100), 0, SORROW_PHASE);
                    events.ScheduleEvent(EVENT_SHADOW_LANCE, urand(22400, 23300), 0, SORROW_PHASE);
                    events.ScheduleEvent(EVENT_SINFUL_BEAM, urand(14400, 15300), 0, SORROW_PHASE);
                    events.ScheduleEvent(EVENT_FEAR, urand(49200, 55100), 0, SORROW_PHASE);
                    break;
                case EVENT_CHECK_RESET:
                    Position const& _homePosition = me->GetHomePosition();
                    if (me->GetDistance2d(_homePosition.GetPositionX(), _homePosition.GetPositionY()) > 30.0f)
                    {
                        EnterEvadeMode();
                        return;
                    }
                    events.ScheduleEvent(EVENT_CHECK_RESET, 50);
                    break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };
};

void AddSC_Custom_Npc_Scripts()
{
    new GavisGreyshield_CreatureScript();
    new npc_rift_spawn();
    new npc_custom_worldboss();
    new boss_wowgame();
}