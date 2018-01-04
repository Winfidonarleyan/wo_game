#include "ScriptMgr.h"
#include "ObjectMgr.h"

class LevelupSpellLearn_PlayerScript : PlayerScript
{
public:
    LevelupSpellLearn_PlayerScript() : PlayerScript("levelup_spell_learn") { }

    void LearnSpells(Player* player)
    {
        static std::vector<uint32> npcTrainers = 
        {
            0, // None
            985, // Warrior
            927, // Paladin
            987, // Hunter
            917, // Rogue
            376, // Priest
            28472, // Death knight
            986, // Shaman
            328, // Mage
            906, // Warlock
            0, // Monk (5.x)
            3033, // Druid
            0 // Demon hunter
        };

        auto plrClass = player->getClass();
        if (plrClass >= npcTrainers.size())
            return;

        if (auto spells = sObjectMgr->GetNpcTrainerSpells(npcTrainers[plrClass]))
        {
            for (auto itr = spells->spellList.begin(); itr != spells->spellList.end(); itr++)
            {
                auto spell = &itr->second;

                if (spell->SpellID > 0 && player->GetTrainerSpellState(spell) == TRAINER_SPELL_GREEN && !player->HasSpell(spell->SpellID))
                {
                    if (spell->IsCastable())
                        player->CastSpell(player, spell->SpellID, true);
                    else
                        player->LearnSpell(spell->SpellID, false);
                }
            }
        }
    }

    void OnLogin(Player* player, bool) override
    {
        LearnSpells(player);
    }

    void OnLevelChanged(Player* player, uint8 oldLevel) override
    {
        LearnSpells(player);
    }

    void OnFreeTalentPointsChanged(Player* player, uint32 points)
    {
        if (points < 1)
            LearnSpells(player);
    }
};

void AddSC_LevelSpellLearn()
{
    new LevelupSpellLearn_PlayerScript();
}