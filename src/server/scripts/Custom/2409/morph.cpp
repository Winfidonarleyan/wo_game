#include "ScriptPCH.h"
#include "ScriptMgr.h"
#include "ScriptedGossip.h"

#define MSG_GOSSIP_TEXT_GETTING_STARTED	"Приветствуем вас на нашем сервере!"

#define MSG_ERR_INCOMBAT "Вы находитесь в бою. Чтобы использовать данного Npc выйдите из него."

#define MSG_GOSSIP_TEXT_MORTH_GNOME_MALE "[Превратить] Гном, мужчина."
#define MSG_GOSSIP_TEXT_MORTH_GNOME_FEMALE "[Превратить] Гном, женщина."
#define MSG_GOSSIP_TEXT_MORTH_HUMAN_FEMALE "[Превратить] Человек, женщина."
#define MSG_GOSSIP_TEXT_MORTH_HUMAN_MALE "[Превратить] Человек, мужчина."
#define MSG_GOSSIP_TEXT_MORTH_BLOOD_ELF_MALE "[Превратить] Эльф Крови, мужчина."
#define MSG_GOSSIP_TEXT_MORTH_BLOOD_ELF_FEMALE "[Превратить] Эльф Крови, женщина."
#define MSG_GOSSIP_TEXT_MORTH_TAUREN_MALE "[Превратить] Таурен, мужчина."
#define MSG_GOSSIP_TEXT_MORTH_TAUREN_FEMALE "[Превратить] Таурен, женщина."

class npc_morph : public CreatureScript
{
public:
    npc_morph() : CreatureScript("npc_morph") { }

    struct npc_morphAI : public ScriptedAI
    {
        npc_morphAI(Creature* creature) : ScriptedAI(creature) { }

        bool GossipHello(Player *player) override
        {
            me->Whisper(MSG_GOSSIP_TEXT_GETTING_STARTED, LANG_UNIVERSAL, player);
            AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, MSG_GOSSIP_TEXT_MORTH_GNOME_MALE, GOSSIP_SENDER_MAIN, 2);
            AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, MSG_GOSSIP_TEXT_MORTH_GNOME_FEMALE, GOSSIP_SENDER_MAIN, 3);
            AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, MSG_GOSSIP_TEXT_MORTH_HUMAN_MALE, GOSSIP_SENDER_MAIN, 4);
            AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, MSG_GOSSIP_TEXT_MORTH_HUMAN_FEMALE, GOSSIP_SENDER_MAIN, 5);
            AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, MSG_GOSSIP_TEXT_MORTH_BLOOD_ELF_MALE, GOSSIP_SENDER_MAIN, 6);
            AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, MSG_GOSSIP_TEXT_MORTH_BLOOD_ELF_FEMALE, GOSSIP_SENDER_MAIN, 7);
            AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, MSG_GOSSIP_TEXT_MORTH_TAUREN_MALE, GOSSIP_SENDER_MAIN, 8);
            AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, MSG_GOSSIP_TEXT_MORTH_TAUREN_FEMALE, GOSSIP_SENDER_MAIN, 9);
            SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, me->GetGUID());
            return true;
        }

        bool GossipSelect(Player* player, uint32 /*menu_id*/, uint32 gossipListId) override
        {
            uint32 action = player->PlayerTalkClass->GetGossipOptionAction(gossipListId);
            if (!player->getAttackers().empty())
            {
                me->Whisper(MSG_ERR_INCOMBAT, LANG_UNIVERSAL, player);
                CloseGossipMenuFor(player);
                return false;
            }
            switch (action)
            {
            case 2:
                player->CastSpell(player, 37808, true);
                CloseGossipMenuFor(player);
                break;
            case 3:
                player->CastSpell(player, 37809, true);
                CloseGossipMenuFor(player);
                break;
            case 4:
                player->CastSpell(player, 35466, true);
                CloseGossipMenuFor(player);
                break;
            case 5:
                player->CastSpell(player, 37805, true);
                CloseGossipMenuFor(player);
                break;
            case 6:
                player->CastSpell(player, 37807, true);
                CloseGossipMenuFor(player);
                break;
            case 7:
                player->CastSpell(player, 37806, true);
                CloseGossipMenuFor(player);
                break;
            case 8:
                player->CastSpell(player, 37810, true);
                CloseGossipMenuFor(player);
                break;
            case 9:
                player->CastSpell(player, 37811, true);
                CloseGossipMenuFor(player);
                break;
            }
            return true;
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_morphAI(creature);
    }
};

void AddSC_npc_morph()
{
    new npc_morph;
}