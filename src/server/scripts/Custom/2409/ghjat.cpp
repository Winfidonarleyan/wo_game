#include "ScriptPCH.h"
#include "Language.h"
#include "World.h"
#include "ScriptedGossip.h"
#include "Chat.h"
#include "Log.h"
#include "SpellMgr.h"
#include "DBCStores.h"

class Professions_NPC : public CreatureScript
{
public:
    Professions_NPC() : CreatureScript("Professions_NPC") {}

    struct Professions_NPCAI : public ScriptedAI
    {
        Professions_NPCAI(Creature* creature) : ScriptedAI(creature) { }

        void CreatureWhisperBasedOnBool(const char *text, Creature *_creature, Player *pPlayer, bool value)
        {
            if (value)
                _creature->Whisper(text, LANG_UNIVERSAL, pPlayer);
        }

        uint32 PlayerMaxLevel() const
        {
            return sWorld->getIntConfig(CONFIG_MAX_PLAYER_LEVEL);
        }

        bool PlayerHasItemOrSpell(const Player *plr, uint32 itemId, uint32 spellId) const
        {
            return plr->HasItemCount(itemId, 1, true) || plr->HasSpell(spellId);
        }

        bool GossipHello(Player *pPlayer) override
        {
            AddGossipItemFor(pPlayer, 9, "[Professions] ->", GOSSIP_SENDER_MAIN, 196);
            pPlayer->PlayerTalkClass->SendGossipMenu(907, me->GetGUID());
            return true;
        }

        bool PlayerAlreadyHasTwoProfessions(const Player *pPlayer) const
        {
            uint32 skillCount = 0;

            if (pPlayer->HasSkill(SKILL_MINING))
                skillCount++;
            if (pPlayer->HasSkill(SKILL_SKINNING))
                skillCount++;
            if (pPlayer->HasSkill(SKILL_HERBALISM))
                skillCount++;

            if (skillCount >= sWorld->getIntConfig(CONFIG_MAX_PRIMARY_TRADE_SKILL))
                return true;

            for (uint32 i = 1; i < sSkillLineStore.GetNumRows(); ++i)
            {
                SkillLineEntry const *SkillInfo = sSkillLineStore.LookupEntry(i);
                if (!SkillInfo)
                    continue;

                if (SkillInfo->categoryId == SKILL_CATEGORY_SECONDARY)
                    continue;

                if ((SkillInfo->categoryId != SKILL_CATEGORY_PROFESSION) || !SkillInfo->canLink)
                    continue;

                const uint32 skillID = SkillInfo->id;
                if (pPlayer->HasSkill(skillID))
                    skillCount++;

                if (skillCount >= sWorld->getIntConfig(CONFIG_MAX_PRIMARY_TRADE_SKILL))
                    return true;
            }
            return false;
        }

        bool LearnAllRecipesInProfession(Player *pPlayer, SkillType skill)
        {
            ChatHandler handler(pPlayer->GetSession());
            char* skill_name;

            SkillLineEntry const *SkillInfo = sSkillLineStore.LookupEntry(skill);
            skill_name = SkillInfo->name[handler.GetSessionDbcLocale()];

            if (!SkillInfo)
                TC_LOG_ERROR("server.ghjat", "Profession NPC: received non-valid skill ID (LearnAllRecipesInProfession)");

            LearnSkillRecipesHelper(pPlayer, SkillInfo->id);

            pPlayer->SetSkill(SkillInfo->id, pPlayer->GetSkillStep(SkillInfo->id), 450, 450);
            handler.PSendSysMessage(LANG_COMMAND_LEARN_ALL_RECIPES, skill_name);

            return true;
        }

        void LearnSkillRecipesHelper(Player *player, uint32 skill_id)
        {
            uint32 classmask = player->getClassMask();

            for (uint32 j = 0; j < sSkillLineAbilityStore.GetNumRows(); ++j)
            {
                SkillLineAbilityEntry const *skillLine = sSkillLineAbilityStore.LookupEntry(j);
                if (!skillLine)
                    continue;

                // wrong skill
                if (skillLine->skillId != skill_id)
                    continue;

                // not high rank
                if (skillLine->forward_spellid)
                    continue;

                // skip racial skills
                if (skillLine->racemask != 0)
                    continue;

                // skip wrong class skills
                if (skillLine->classmask && (skillLine->classmask & classmask) == 0)
                    continue;

                SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(skillLine->spellId);
                if (!spellInfo || !SpellMgr::IsSpellValid(spellInfo, nullptr, false))
                    continue;

                player->LearnSpell(skillLine->spellId, false);
            }
        }

        bool IsSecondarySkill(SkillType skill) const
        {
            return skill == SKILL_COOKING || skill == SKILL_FIRST_AID;
        }

        void CompleteLearnProfession(Player *pPlayer, Creature *pCreature, SkillType skill)
        {
            if (PlayerAlreadyHasTwoProfessions(pPlayer) && !IsSecondarySkill(skill))
                pCreature->Whisper("You already know two professions!", LANG_UNIVERSAL, pPlayer);
            else
            {
                if (!LearnAllRecipesInProfession(pPlayer, skill))
                    pCreature->Whisper("Internal error occured!", LANG_UNIVERSAL, pPlayer);
            }
        }

        bool GossipSelect(Player* pPlayer, uint32 /*menu_id*/, uint32 gossipListId) override
        {
            uint32 uiAction = pPlayer->PlayerTalkClass->GetGossipOptionAction(gossipListId);
            pPlayer->PlayerTalkClass->ClearMenus();

                switch (uiAction)
                {
                case 196:
                    AddGossipItemFor(pPlayer, GOSSIP_ICON_INTERACT_2, "|TInterface\\icons\\trade_alchemy:30|t Алхимия.", GOSSIP_SENDER_MAIN, 1);
                    AddGossipItemFor(pPlayer, GOSSIP_ICON_INTERACT_2, "|TInterface\\icons\\INV_Ingot_05:30|t Кузнечное дело.", GOSSIP_SENDER_MAIN, 2);
                    AddGossipItemFor(pPlayer, GOSSIP_ICON_INTERACT_2, "|TInterface\\icons\\INV_Misc_LeatherScrap_02:30|t Кожевничество.", GOSSIP_SENDER_MAIN, 3);
                    AddGossipItemFor(pPlayer, GOSSIP_ICON_INTERACT_2, "|TInterface\\icons\\INV_Fabric_Felcloth_Ebon:30|t Портняжное дело.", GOSSIP_SENDER_MAIN, 4);
                    AddGossipItemFor(pPlayer, GOSSIP_ICON_INTERACT_2, "|TInterface\\icons\\inv_misc_wrench_01:30|t Инженерное дело.", GOSSIP_SENDER_MAIN, 5);
                    AddGossipItemFor(pPlayer, GOSSIP_ICON_INTERACT_2, "|TInterface\\icons\\trade_engraving:30|t Наложение чар.", GOSSIP_SENDER_MAIN, 6);
                    AddGossipItemFor(pPlayer, GOSSIP_ICON_INTERACT_2, "|TInterface\\icons\\inv_misc_gem_01:30|t Ювелирное дело.", GOSSIP_SENDER_MAIN, 7);
                    AddGossipItemFor(pPlayer, GOSSIP_ICON_INTERACT_2, "|TInterface\\icons\\INV_Scroll_08:30|t Начертание.", GOSSIP_SENDER_MAIN, 8);
                    AddGossipItemFor(pPlayer, GOSSIP_ICON_INTERACT_2, "|TInterface\\icons\\INV_Misc_Herb_07:30|t Травничество.", GOSSIP_SENDER_MAIN, 9);
                    AddGossipItemFor(pPlayer, GOSSIP_ICON_INTERACT_2, "|TInterface\\icons\\inv_misc_pelt_wolf_01:30|t Снятие шкур.", GOSSIP_SENDER_MAIN, 10);
                    AddGossipItemFor(pPlayer, GOSSIP_ICON_INTERACT_2, "|TInterface\\icons\\trade_mining:30|t Горное дело.", GOSSIP_SENDER_MAIN, 11);
                    AddGossipItemFor(pPlayer, GOSSIP_ICON_INTERACT_2, "|TInterface\\icons\\trade_fishing:30|t Рыбалка.", GOSSIP_SENDER_MAIN, 13);
                    AddGossipItemFor(pPlayer, GOSSIP_ICON_INTERACT_2, "|TInterface\\icons\\Spell_holy_sealofsacrifice:30|t Первая помощь.", GOSSIP_SENDER_MAIN, 14);
                    AddGossipItemFor(pPlayer, GOSSIP_ICON_INTERACT_2, "|TInterface\\icons\\Inv_misc_food_50:30|t Кулинария.", GOSSIP_SENDER_MAIN, 15);
                    pPlayer->PlayerTalkClass->SendGossipMenu(1, me->GetGUID());
                    break;
                case 1:
                    if (pPlayer->HasSkill(SKILL_ALCHEMY))
                    {
                        pPlayer->PlayerTalkClass->SendCloseGossip();
                        break;
                    }

                    CompleteLearnProfession(pPlayer, me, SKILL_ALCHEMY);

                    pPlayer->PlayerTalkClass->SendCloseGossip();
                    break;
                case 2:
                    if (pPlayer->HasSkill(SKILL_BLACKSMITHING))
                    {
                        pPlayer->PlayerTalkClass->SendCloseGossip();
                        break;
                    }
                    CompleteLearnProfession(pPlayer, me, SKILL_BLACKSMITHING);

                    pPlayer->PlayerTalkClass->SendCloseGossip();
                    break;
                case 3:
                    if (pPlayer->HasSkill(SKILL_LEATHERWORKING))
                    {
                        pPlayer->PlayerTalkClass->SendCloseGossip();
                        break;
                    }
                    CompleteLearnProfession(pPlayer, me, SKILL_LEATHERWORKING);

                    pPlayer->PlayerTalkClass->SendCloseGossip();
                    break;
                case 4:
                    if (pPlayer->HasSkill(SKILL_TAILORING))
                    {
                        pPlayer->PlayerTalkClass->SendCloseGossip();
                        break;
                    }
                    CompleteLearnProfession(pPlayer, me, SKILL_TAILORING);

                    pPlayer->PlayerTalkClass->SendCloseGossip();
                    break;
                case 5:
                    if (pPlayer->HasSkill(SKILL_ENGINEERING))
                    {
                        pPlayer->PlayerTalkClass->SendCloseGossip();
                        break;
                    }
                    CompleteLearnProfession(pPlayer, me, SKILL_ENGINEERING);

                    pPlayer->PlayerTalkClass->SendCloseGossip();
                    break;
                case 6:
                    if (pPlayer->HasSkill(SKILL_ENCHANTING))
                    {
                        pPlayer->PlayerTalkClass->SendCloseGossip();
                        break;
                    }
                    CompleteLearnProfession(pPlayer, me, SKILL_ENCHANTING);

                    pPlayer->PlayerTalkClass->SendCloseGossip();
                    break;
                case 7:
                    if (pPlayer->HasSkill(SKILL_JEWELCRAFTING))
                    {
                        pPlayer->PlayerTalkClass->SendCloseGossip();
                        break;
                    }
                    CompleteLearnProfession(pPlayer, me, SKILL_JEWELCRAFTING);

                    pPlayer->PlayerTalkClass->SendCloseGossip();
                    break;
                case 8:
                    if (pPlayer->HasSkill(SKILL_INSCRIPTION))
                    {
                        pPlayer->PlayerTalkClass->SendCloseGossip();
                        break;
                    }
                    CompleteLearnProfession(pPlayer, me, SKILL_INSCRIPTION);

                    pPlayer->PlayerTalkClass->SendCloseGossip();
                    break;
                case 9:
                    if (pPlayer->HasSkill(SKILL_HERBALISM))
                    {
                        pPlayer->PlayerTalkClass->SendCloseGossip();
                        break;
                    }

                    CompleteLearnProfession(pPlayer, me, SKILL_HERBALISM);
                    pPlayer->PlayerTalkClass->SendCloseGossip();
                    break;
                case 10:
                    if (pPlayer->HasSkill(SKILL_SKINNING))
                    {
                        pPlayer->PlayerTalkClass->SendCloseGossip();
                        break;
                    }

                    CompleteLearnProfession(pPlayer, me, SKILL_SKINNING);
                    pPlayer->PlayerTalkClass->SendCloseGossip();
                    break;
                case 11:
                    if (pPlayer->HasSkill(SKILL_MINING))
                    {
                        pPlayer->PlayerTalkClass->SendCloseGossip();
                        break;
                    }

                    CompleteLearnProfession(pPlayer, me, SKILL_MINING);
                    pPlayer->PlayerTalkClass->SendCloseGossip();
                    break;
                case 12:
                    pPlayer->PlayerTalkClass->SendCloseGossip();
                    break;

                case 13:
                    if (pPlayer->HasSkill(SKILL_FISHING))
                    {
                        pPlayer->PlayerTalkClass->SendCloseGossip();
                        break;
                    }

                    CompleteLearnProfession(pPlayer, me, SKILL_FISHING);
                    pPlayer->PlayerTalkClass->SendCloseGossip();
                    break;

                case 14:
                    if (pPlayer->HasSkill(SKILL_FIRST_AID))
                    {
                        pPlayer->PlayerTalkClass->SendCloseGossip();
                        break;
                    }

                    CompleteLearnProfession(pPlayer, me, SKILL_FIRST_AID);
                    pPlayer->PlayerTalkClass->SendCloseGossip();
                    break;

                case 15:
                    if (pPlayer->HasSkill(SKILL_COOKING))
                    {
                        pPlayer->PlayerTalkClass->SendCloseGossip();
                        break;
                    }

                    CompleteLearnProfession(pPlayer, me, SKILL_COOKING);
                    pPlayer->PlayerTalkClass->SendCloseGossip();
                    break;
                }
            return true;
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new Professions_NPCAI(creature);
    }
};


void AddSC_Professions_NPC()
{
    new Professions_NPC();
}
