/*
** Made by Rochet2(Eluna)
** Rewritten by mthsena(C++)
*/

#include "ScriptPCH.h"
#include "ScriptedGossip.h"
#include "ItemTemplate.h"
#include "Chat.h"
#include "ItemDefines.h"
#include "Item.h"

using namespace std;

#define DEFAULT_MESSAGE 907

struct VisualData
{
    uint32 Menu;
    uint32 Submenu;
    uint32 Icon;
    uint32 Id;
    string Name;
};

VisualData vData[] =
{
    { 1, 0, GOSSIP_ICON_BATTLE, 3789, "|TInterface\\icons\\Spell_chargepositive:20:20:-18:0|tБерсерк" },
    { 1, 0, GOSSIP_ICON_BATTLE, 3854, "|TInterface\\icons\\Spell_chargepositive:20:20:-18:0|tСила заклинания" },
    { 1, 0, GOSSIP_ICON_BATTLE, 3273, "|TInterface\\icons\\Spell_chargepositive:20:20:-18:0|tСмертной стужи" },
    { 1, 0, GOSSIP_ICON_BATTLE, 3225, "|TInterface\\icons\\Spell_chargepositive:20:20:-18:0|tПалач" },
    { 1, 0, GOSSIP_ICON_BATTLE, 3870, "|TInterface\\icons\\Spell_chargepositive:20:20:-18:0|tКровь Слив" },
    { 1, 0, GOSSIP_ICON_BATTLE, 1899, "|TInterface\\icons\\Spell_chargepositive:20:20:-18:0|tНечестивая оружие" },
    { 1, 0, GOSSIP_ICON_BATTLE, 2674, "|TInterface\\icons\\Spell_chargepositive:20:20:-18:0|tВсплеск" },
    { 1, 0, GOSSIP_ICON_BATTLE, 2675, "|TInterface\\icons\\Spell_chargepositive:20:20:-18:0|tВоеначальник" },
    { 1, 0, GOSSIP_ICON_BATTLE, 2671, "|TInterface\\icons\\Spell_chargepositive:20:20:-18:0|tТайное и Огонь заклинаний" },
    { 1, 0, GOSSIP_ICON_BATTLE, 2672, "|TInterface\\icons\\Spell_chargepositive:20:20:-18:0|tТень и Мороз к силе заклинаний" },
    { 1, 0, GOSSIP_ICON_BATTLE, 3365, "|TInterface\\icons\\Spell_chargepositive:20:20:-18:0|tРуна" },
    { 1, 0, GOSSIP_ICON_BATTLE, 2673, "|TInterface\\icons\\Spell_chargepositive:20:20:-18:0|tМангуста" },
    { 1, 0, GOSSIP_ICON_BATTLE, 2343, "|TInterface\\icons\\Spell_chargepositive:20:20:-18:0|tСила заклинания 2" },
    { 1, 2, GOSSIP_ICON_TALK, 0, "|TInterface\\icons\\Spell_chargepositive:20:20:-18:0|tСлед страница" },

    { 2, 0, GOSSIP_ICON_BATTLE, 425, "|TInterface\\icons\\Spell_chargepositive:20:20:-18:0|tЧерный храм пустышки" },
    { 2, 0, GOSSIP_ICON_BATTLE, 3855, "|TInterface\\icons\\Spell_chargepositive:20:20:-18:0|tСила заклинания III" },
    { 2, 0, GOSSIP_ICON_BATTLE, 1894, "|TInterface\\icons\\Spell_chargepositive:20:20:-18:0|tЛедяное оружие" },
    { 2, 0, GOSSIP_ICON_BATTLE, 1103, "|TInterface\\icons\\Spell_chargepositive:20:20:-18:0|tПроворство" },
    { 2, 0, GOSSIP_ICON_BATTLE, 1898, "|TInterface\\icons\\Spell_chargepositive:20:20:-18:0|tЖизнь воровать" },
    { 2, 0, GOSSIP_ICON_BATTLE, 3345, "|TInterface\\icons\\Spell_chargepositive:20:20:-18:0|tЖизнь Земли I" },
    { 2, 0, GOSSIP_ICON_BATTLE, 1743, "|TInterface\\icons\\Spell_chargepositive:20:20:-18:0|tРозовый свет" },
    { 2, 0, GOSSIP_ICON_BATTLE, 3093, "|TInterface\\icons\\Spell_chargepositive:20:20:-18:0|tСила Атаки" },
    { 2, 0, GOSSIP_ICON_BATTLE, 1900, "|TInterface\\icons\\Spell_chargepositive:20:20:-18:0|tКрестоносец" },
    { 2, 0, GOSSIP_ICON_BATTLE, 3846, "|TInterface\\icons\\Spell_chargepositive:20:20:-18:0|tСила заклинания II" },
    { 2, 0, GOSSIP_ICON_BATTLE, 1606, "|TInterface\\icons\\Spell_chargepositive:20:20:-18:0|tСила Атаки 2" },
    { 2, 0, GOSSIP_ICON_BATTLE, 283, "|TInterface\\icons\\Spell_chargepositive:20:20:-18:0|tВетер бешенство I" },
    { 2, 0, GOSSIP_ICON_BATTLE, 1, "|TInterface\\icons\\Spell_chargepositive:20:20:-18:0|tРок кусающееся III" },
    { 2, 3, GOSSIP_ICON_TALK, 0, "|TInterface\\icons\\Spell_chargepositive:20:20:-18:0|tСлед страница" },
    { 2, 1, GOSSIP_ICON_TALK, 0, "|TInterface\\icons\\Spell_chargenegative:20:20:-18:0|tНазад" },

    { 3, 0, GOSSIP_ICON_BATTLE, 3265, "|TInterface\\icons\\Spell_chargepositive:20:20:-18:0|tБлаженный Оружие Покрытие" },
    { 3, 0, GOSSIP_ICON_BATTLE, 2, "|TInterface\\icons\\Spell_chargepositive:20:20:-18:0|tБренд Льда I" },
    { 3, 0, GOSSIP_ICON_BATTLE, 3, "|TInterface\\icons\\Spell_chargepositive:20:20:-18:0|tПламя язык III" },
    { 3, 0, GOSSIP_ICON_BATTLE, 3266, "|TInterface\\icons\\Spell_chargepositive:20:20:-18:0|tПраведный оружие Покрытие" },
    { 3, 0, GOSSIP_ICON_BATTLE, 1903, "|TInterface\\icons\\Spell_chargepositive:20:20:-18:0|tДух" },
    { 3, 0, GOSSIP_ICON_BATTLE, 13, "|TInterface\\icons\\Spell_chargepositive:20:20:-18:0|tЗаостренный" },
    { 3, 0, GOSSIP_ICON_BATTLE, 26, "|TInterface\\icons\\Spell_chargepositive:20:20:-18:0|tЛёд" },
    { 3, 0, GOSSIP_ICON_BATTLE, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-18:0|tСмертельный яд" },
    { 3, 0, GOSSIP_ICON_BATTLE, 803, "|TInterface\\icons\\Spell_chargepositive:20:20:-18:0|tОгненное оружие" },
    { 3, 0, GOSSIP_ICON_BATTLE, 1896, "|TInterface\\icons\\Spell_chargepositive:20:20:-18:0|tОружие Урон" },
    { 3, 0, GOSSIP_ICON_BATTLE, 2666, "|TInterface\\icons\\Spell_chargepositive:20:20:-18:0|tИнтеллект" },
    { 3, 0, GOSSIP_ICON_BATTLE, 25, "|TInterface\\icons\\Spell_chargepositive:20:20:-18:0|tТень Нефть" },
    { 3, 2, GOSSIP_ICON_TALK, 0, "|TInterface\\icons\\Spell_chargenegative:20:20:-18:0|tНазад" },
};

class NPC_VisualWeapon : public CreatureScript
{
public:
    NPC_VisualWeapon() : CreatureScript("VisualWeapon") { }

    struct NPC_VisualWeaponAI : public ScriptedAI
    {
        NPC_VisualWeaponAI(Creature* creature) : ScriptedAI(creature) { }

        bool MainHand;

        void SetVisual(Player* player, uint32 visual)
        {
            uint8 slot = MainHand ? EQUIPMENT_SLOT_MAINHAND : EQUIPMENT_SLOT_OFFHAND;

            Item* item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, slot);

            if (!item)
            {
                ChatHandler(player->GetSession()).PSendSysMessage("Нету оружия для этого слота.");
                return;
            }

            const ItemTemplate* itemTemplate = item->GetTemplate();

            if (itemTemplate->SubClass == ITEM_SUBCLASS_ARMOR_SHIELD ||
                itemTemplate->SubClass == ITEM_SUBCLASS_ARMOR_BUCKLER ||
                itemTemplate->SubClass == ITEM_SUBCLASS_WEAPON_SPEAR ||
                itemTemplate->SubClass == ITEM_SUBCLASS_WEAPON_BOW ||
                itemTemplate->SubClass == ITEM_SUBCLASS_WEAPON_GUN ||
                itemTemplate->SubClass == ITEM_SUBCLASS_WEAPON_EXOTIC ||
                itemTemplate->SubClass == ITEM_SUBCLASS_WEAPON_EXOTIC2 ||
                itemTemplate->SubClass == ITEM_SUBCLASS_WEAPON_THROWN ||
                itemTemplate->SubClass == ITEM_SUBCLASS_WEAPON_CROSSBOW ||
                itemTemplate->SubClass == ITEM_SUBCLASS_WEAPON_WAND ||
                itemTemplate->SubClass == ITEM_SUBCLASS_WEAPON_FISHING_POLE)
                return;

            item->SetEnchantVisual(visual);
            player->SetVisibleItemSlot(slot, item);
        }

        void GetMenu(Player* player, Creature* creature, uint32 menuId)
        {
            for (uint8 i = 0; i < (sizeof(vData) / sizeof(*vData)); i++)
            {
                if (vData[i].Menu == menuId)
                    AddGossipItemFor(player, vData[i].Icon, vData[i].Name, GOSSIP_SENDER_MAIN, i);
            }

            SendGossipMenuFor(player, DEFAULT_MESSAGE, creature->GetGUID());
        }

        bool GossipHello(Player* player) override
        {
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, "|TInterface/PaperDoll/UI-PaperDoll-Slot-MainHand:32:32:-15:0|tПравая Рука", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, "|TInterface/PaperDoll/UI-PaperDoll-Slot-SecondaryHand:32:32:-15:0|tЛевая Рука", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, "|TInterface/PaperDollInfoFrame/UI-GearManager-Undo:32:32:-15:0|tВыйти", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
            SendGossipMenuFor(player, DEFAULT_MESSAGE, me->GetGUID());
            return true;
        }

        bool GossipSelect(Player* player, uint32 /*menu_id*/, uint32 gossipListId) override
        {
            uint32 action = player->PlayerTalkClass->GetGossipOptionAction(gossipListId);
            player->PlayerTalkClass->ClearMenus();

            switch (action)
            {
            case GOSSIP_ACTION_INFO_DEF + 1:
                MainHand = true;
                GetMenu(player, me, 1);
                return false;

            case GOSSIP_ACTION_INFO_DEF + 2:
                MainHand = false;
                GetMenu(player, me, 1);
                return false;

            case GOSSIP_ACTION_INFO_DEF + 3:
                CloseGossipMenuFor(player);
                return false;
            }

            uint32 menuData = vData[action].Submenu;

            if (menuData == 0)
            {
                SetVisual(player, vData[action].Id);
                menuData = vData[action].Menu;
            }

            GetMenu(player, me, menuData);
            return true;
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new NPC_VisualWeaponAI(creature);
    }
};

class onlogin_enchant_visual : public PlayerScript
{
public:
    onlogin_enchant_visual() : PlayerScript("onlogin_enchant_visual") {}

   void OnLogin(Player* player, bool /*firstLogin*/)
   {
      for (uint8 slot = EQUIPMENT_SLOT_MAINHAND; slot <= EQUIPMENT_SLOT_OFFHAND; slot++)
      {
         if (Item* item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, slot))
             player->SetUInt16Value(PLAYER_VISIBLE_ITEM_1_ENCHANTMENT + (slot * 2), 0, (item->GetEnchantVisual() ? item->GetEnchantVisual() : item->GetEnchantmentId(PERM_ENCHANTMENT_SLOT)));
      }
   }
};

void AddSC_NPC_VisualWeapon()
{
    new NPC_VisualWeapon;
    new onlogin_enchant_visual();
}