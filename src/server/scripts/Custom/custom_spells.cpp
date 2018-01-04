#include "ScriptMgr.h"
#include "SpellMgr.h"
#include "SpellScript.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "Player.h"

/* 30988 - Place draenei banner */

#define SPELL_PLACE_DRAENEI_BANNER 30988

class place_darenei_banner : public SpellScriptLoader
{
public:
    place_darenei_banner() : SpellScriptLoader("spell_place_darenei_banner") { }

    class place_darenei_banner_SpellScript : public SpellScript
    {
        PrepareSpellScript(place_darenei_banner_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_PLACE_DRAENEI_BANNER))
                return false;

            return true;
        }

        SpellCastResult CheckRequirement()
        {
            if (auto npc = GetCaster()->FindNearestCreature(17701, 5, false))
            {
                if (!npc->IsAlive())
                    return SPELL_CAST_OK;
            }

            return SPELL_FAILED_NO_VALID_TARGETS;
        }

        void Register() override
        {
            OnCheckCast += SpellCheckCastFn(place_darenei_banner_SpellScript::CheckRequirement);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new place_darenei_banner_SpellScript();
    }
};

/* 42169 - Reflective dust */

#define SPELL_REFLECTIVE_DUST 42169

class reflective_dust_spellScriptLoader : public SpellScriptLoader
{
public:
    reflective_dust_spellScriptLoader() : SpellScriptLoader("spell_reflective_dust_42169") { }

    class reflective_dust_AuraScript : public AuraScript
    {
        PrepareAuraScript(reflective_dust_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_REFLECTIVE_DUST))
                return false;

            return true;
        }

        void OnApply(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
        {
            if (Unit* target = GetTarget())
                target->SetPhaseMask(target->GetPhaseMask() | 2, true);
        }

        void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Unit* target = GetTarget())
                target->SetPhaseMask(target->GetPhaseMask() & ~2, true);
        }

        void Register() override
        {
            OnEffectApply += AuraEffectApplyFn(reflective_dust_AuraScript::OnApply, EFFECT_1, SPELL_AURA_MOD_INVISIBILITY_DETECT, AURA_EFFECT_HANDLE_REAL);
            AfterEffectRemove += AuraEffectRemoveFn(reflective_dust_AuraScript::OnRemove, EFFECT_1, SPELL_AURA_MOD_INVISIBILITY_DETECT, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new reflective_dust_AuraScript();
    }
};

/* 42287 - Salvage Kit */

#define SPELL_SALVAGE_KIT 42287

class salvage_kit_spellScriptLoader : public SpellScriptLoader
{
public:
    salvage_kit_spellScriptLoader() : SpellScriptLoader("spell_salvage_kit_42287") { }

    class salvage_kit_SpellScript : public SpellScript
    {
        PrepareSpellScript(salvage_kit_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_SALVAGE_KIT))
                return false;

            return true;
        }

        void HandleScript(SpellEffIndex)
        {
            if (auto go = GetHitGObj())
            {
                go->SetRespawnTime(300);
                go->UpdateObjectVisibility(true);
            }
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(salvage_kit_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new salvage_kit_SpellScript();
    }
};

class spell_selfstun_SpellScriptLoader : SpellScriptLoader
{
public:
    spell_selfstun_SpellScriptLoader() : SpellScriptLoader("spell_selfstun") { }

    class spell_selfStun_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_selfStun_AuraScript);

        void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (auto target = GetTarget())
            {
                if (auto npc = target->ToCreature())
                    npc->Respawn(true);
            }
        }

        void Register() override
        {
            AfterEffectRemove += AuraEffectRemoveFn(spell_selfStun_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_MOD_STUN, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_selfStun_AuraScript();
    }
};

/* Deep freeze proc: 71757 */
class spell_deep_freeze_proc_SpellScriptLoader : SpellScriptLoader
{
public:
    spell_deep_freeze_proc_SpellScriptLoader() : SpellScriptLoader("spell_deep_freeze_proc") { }

    class spell_deep_freeze_SpellScript : public SpellScript
    {
    public:
        PrepareSpellScript(spell_deep_freeze_SpellScript);

        bool Validate(SpellInfo const*) override
        {
            return sSpellMgr->GetSpellInfo(71757) != nullptr;
        }

        void FilterTargets(WorldObject*& target)
        {
            if (target)
            {
                if (auto creature = target->ToCreature())
                {
                    if (creature->GetCreatureTemplate()->MechanicImmuneMask & 2048)
                        return;
                }
            }

            target = nullptr;
        }

        void Register() override
        {
            OnObjectTargetSelect += SpellObjectTargetSelectFn(spell_deep_freeze_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_TARGET_ENEMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_deep_freeze_SpellScript();
    }
};

/* Hand of Reckoning: 62124 */
class spell_hand_of_reckoning_spellScriptLoader : public SpellScriptLoader
{
public:
    spell_hand_of_reckoning_spellScriptLoader() : SpellScriptLoader("spell_hand_of_reckoning") { }

    class spell_hand_of_reckoning_SpellScript : public SpellScript
    {
    public:
        PrepareSpellScript(spell_hand_of_reckoning_SpellScript);

        bool Validate(SpellInfo const*) override
        {
            return sSpellMgr->GetSpellInfo(62124) != nullptr;
        }

        void StartCombat(SpellEffIndex)
        {
            if (auto unit = GetSpell()->m_targets.GetUnitTarget())
            {
                if (auto player = unit->ToPlayer())
                    GetCaster()->CombatStart(player, GetSpellInfo()->HasInitialAggro());
            }
        }

        void Register() override
        {
            OnEffectHit += SpellEffectFn(spell_hand_of_reckoning_SpellScript::StartCombat, EFFECT_0, SPELL_EFFECT_ATTACK_ME);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_hand_of_reckoning_SpellScript();
    }
};

void AddSC_Custom_SpellScripts()
{
    new place_darenei_banner();
    new reflective_dust_spellScriptLoader();
    new salvage_kit_spellScriptLoader();
    new spell_selfstun_SpellScriptLoader();
    new spell_deep_freeze_proc_SpellScriptLoader();
	new spell_hand_of_reckoning_spellScriptLoader();
}
