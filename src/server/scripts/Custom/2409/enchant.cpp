/************************************
*        Made by Krys               *
*************************************
*/
#include "ScriptPCH.h"
#include "Item.h"
#include "WorldSession.h"
#include "ScriptedGossip.h"

enum Enchants
{
    ENCHANT_WEP_BERSERKING = 3789,
    ENCHANT_WEP_BLADE_WARD = 3869,
    ENCHANT_WEP_BLOOD_DRAINING = 3870,
    ENCHANT_WEP_ACCURACY = 3788,
    ENCHANT_WEP_AGILITY_1H = 1103,
    ENCHANT_WEP_SPIRIT = 3844,
    ENCHANT_WEP_BATTLEMASTER = 2675,
    ENCHANT_WEP_BLACK_MAGIC = 3790,
    ENCHANT_WEP_ICEBREAKER = 3239,
    ENCHANT_WEP_LIFEWARD = 3247,
    ENCHANT_WEP_MIGHTY_SPELL_POWER = 3834, // One-hand
    ENCHANT_WEP_EXECUTIONER = 3225,
    ENCHANT_WEP_STRYJA = 1894,
    ENCHANT_WEP_POTENCY = 3833,
    ENCHANT_WEP_TITANGUARD = 3851,
    ENCHANT_2WEP_MASSACRE = 3827,
    ENCHANT_2WEP_SCOURGEBANE = 3247,
    ENCHANT_2WEP_GIANT_SLAYER = 3257,
    ENCHANT_2WEP_GREATER_SPELL_POWER = 3854,
    ENCHANT_2WEP_AGILITY = 2670,
    ENCHANT_2WEP_MONGOOSE = 2673,

    ENCHANT_SHIELD_DEFENSE = 1952,
    ENCHANT_SHIELD_INTELLECT = 1128,
    ENCHANT_SHIELD_RESILIENCE = 3229,
    ENCHANT_SHIELD_BLOCK = 2655,
    ENCHANT_SHIELD_STAMINA = 1077,
    ENCHANT_SHIELD_TOUGHSHIELD = 2653,
    ENCHANT_SHIELD_TITANIUM_PLATING = 3849,

    ENCHANT_HEAD_BLISSFUL_MENDING = 3819,
    ENCHANT_HEAD_BURNING_MYSTERIES = 3820,
    ENCHANT_HEAD_DOMINANCE = 3796,
    ENCHANT_HEAD_SAVAGE_GLADIATOR = 3842,
    ENCHANT_HEAD_STALWART_PROTECTOR = 3818,
    ENCHANT_HEAD_TORMENT = 3817,
    ENCHANT_HEAD_TRIUMPH = 3795,
    ENCHANT_HEAD_ECLIPSED_MOON = 3815,
    ENCHANT_HEAD_FLAME_SOUL = 3816,
    ENCHANT_HEAD_FLEEING_SHADOW = 3814,
    ENCHANT_HEAD_FROSTY_SOUL = 3812,
    ENCHANT_HEAD_TOXIC_WARDING = 3813,

    ENCHANT_SHOULDER_MASTERS_AXE = 3835,
    ENCHANT_SHOULDER_MASTERS_CRAG = 3836,
    ENCHANT_SHOULDER_KRIT = 3838,
    ENCHANT_SHOULDER_MASTERS_PINNACLE = 3837,
    ENCHANT_SHOULDER_MASTERS_STORM = 3813,
    ENCHANT_SHOULDER_GREATER_AXE = 3808,
    ENCHANT_SHOULDER_GREATER_CRAG = 3809,
    ENCHANT_SHOULDER_GREATER_GLADIATOR = 3852,
    ENCHANT_SHOULDER_GREATER_PINNACLE = 3811,
    ENCHANT_SHOULDER_GREATER_STORM = 3810,
    ENCHANT_SHOULDER_DOMINANCE = 3794,
    ENCHANT_SHOULDER_TRIUMPH = 3793,

    ENCHANT_CLOAK_DARKGLOW_EMBROIDERY = 3728,
    ENCHANT_CLOAK_SWORDGUARD_EMBROIDERY = 3730,
    ENCHANT_CLOAK_LIGHTWEAVE_EMBROIDERY = 3722,
    ENCHANT_CLOAK_SPRINGY_ARACHNOWEAVE = 3859,
    ENCHANT_CLOAK_WISDOM = 3296,
    ENCHANT_CLOAK_TITANWEAVE = 1957,
    ENCHANT_CLOAK_SPELL_PIERCING = 3243,
    ENCHANT_CLOAK_SHADOW_ARMOR = 3256,
    ENCHANT_CLOAK_MIGHTY_ARMOR = 3294,
    ENCHANT_CLOAK_MAJOR_AGILITY = 1099,
    ENCHANT_CLOAK_GREATER_SPEED = 3831,

    ENCHANT_LEG_EARTHEN = 3853,
    ENCHANT_LEG_FROSTHIDE = 3822,
    ENCHANT_LEG_ICESCALE = 3823,
    ENCHANT_LEG_BRILLIANT_SPELLTHREAD = 3719,
    ENCHANT_LEG_SAPPHIRE_SPELLTHREAD = 3721,
    ENCHANT_LEG_DRAGONSCALE = 3331,
    ENCHANT_LEG_WYRMSCALE = 3332,

    ENCHANT_GLOVES_GREATER_BLASTING = 3249,
    ENCHANT_GLOVES_ARMSMAN = 3253,
    ENCHANT_GLOVES_CRUSHER = 1603,
    ENCHANT_GLOVES_AGILITY = 3222,
    ENCHANT_GLOVES_PRECISION = 3234,
    ENCHANT_GLOVES_SPD = 3246,
    ENCHANT_GLOVES_EXPERTISE = 3237,
    ENCHANT_GLOVES_HAST = 3604,
    ENCHANT_GLOVES_RAKETA = 3603,

    ENCHANT_BRACERS_MAJOR_STAMINA = 3850,
    ENCHANT_BRACERS_SUPERIOR_SP = 2332,
    ENCHANT_BRACERS_GREATER_ASSUALT = 3845,
    ENCHANT_BRACERS_MAJOR_SPIRT = 1147,
    ENCHANT_BRACERS_EXPERTISE = 3231,
    ENCHANT_BRACERS_INTELLECT = 1119,
    ENCHANT_BRACERS_FURL_ARCANE = 3763,
    ENCHANT_BRACERS_FURL_FIRE = 3759,
    ENCHANT_BRACERS_FURL_FROST = 3760,
    ENCHANT_BRACERS_FURL_NATURE = 3762,
    ENCHANT_BRACERS_FURL_SHADOW = 3761,
    ENCHANT_BRACERS_FURL_ATTACK = 3756,
    ENCHANT_BRACERS_FURL_STAMINA = 3757,
    ENCHANT_BRACERS_FURL_SPELLPOWER = 3758,

    ENCHANT_CHEST_POWERFUL_STATS = 3832,
    ENCHANT_CHEST_SUPER_HEALTH = 3297,
    ENCHANT_CHEST_GREATER_MAINA_REST = 2387,
    ENCHANT_CHEST_EXCEPTIONAL_RESIL = 3245,
    ENCHANT_CHEST_GREATER_DEFENSE = 1953,

    ENCHANT_BOOTS_GREATER_ASSULT = 1597,
    ENCHANT_BOOTS_TUSKARS_VITLIATY = 3232,
    ENCHANT_BOOTS_SUPERIOR_AGILITY = 983,
    ENCHANT_BOOTS_GREATER_SPIRIT = 1147,
    ENCHANT_BOOTS_GREATER_VITALITY = 3244,
    ENCHANT_BOOTS_ICEWALKER = 3826,
    ENCHANT_BOOTS_GREATER_FORTITUDE = 1075,
    ENCHANT_BOOTS_NITRO_BOOTS = 3606,

    ENCHANT_RING_ASSULT = 3839,
    ENCHANT_RING_GREATER_SP = 3840,
    ENCHANT_RING_STAMINA = 3791,

    ENCHANT_VZRIV = 3599,
    ENCHANT_BOMBA = 3601,
    ENCHANT_WAIST_SOCKET = 3729,
    ENCHANT_KISTIRYK_SOCKET = 3723,
    ENCHANT_ZAPYASTIE_SOCKET = 3717,

    ENCHANT_SPINA = 3859, // Пружинистая паутинка
    ENCHANT_SPINADVA = 3605 // Эластичная подкладка

};

void AdvancedEnchant(Player* player, Item* item, EnchantmentSlot slot, uint32 socketGem)
{
    if (!item)
    {
        player->GetSession()->SendNotification("Вы должны одеть предмет для зачарование!");
        return;
    }

    if (!socketGem)
    {
        player->GetSession()->SendNotification("Something went wrong in the code. It has been logged for developers and will be looked into, sorry for the inconvenience.");
        return;
    }

    player->ApplyEnchantment(item, slot, false);
    item->SetEnchantment(slot, socketGem, 0, 0);
    player->ApplyEnchantment(item, slot, true);
    player->GetSession()->SendNotification("|cff800080%s |cffFF0000 успешно произошло", item->GetTemplate()->Name1.c_str());
}

void Enchant(Player* player, Item* item, uint32 enchantid)
{
    if (!item)
    {
        player->GetSession()->SendNotification("Вы должны одеть предмет для зачарование!");
        return;
    }

    if (!enchantid)
    {
        player->GetSession()->SendNotification("Something went wrong in the code. It has been logged for developers and will be looked into, sorry for the inconvenience.");
        //sLog->("enchant_vendor::Enchant: Enchant NPC 'enchantid' is NULL, something went wrong here!");
        return;
    }

    player->ApplyEnchantment(item, PERM_ENCHANTMENT_SLOT, false);
    item->SetEnchantment(PERM_ENCHANTMENT_SLOT, enchantid, 0, 0);
    player->ApplyEnchantment(item, PERM_ENCHANTMENT_SLOT, true);
    player->GetSession()->SendNotification("|cff800080%s |cffFF0000 успешно произошло!", item->GetTemplate()->Name1.c_str());
}

void RemoveEnchant(Player* player, Item* item)
{
    if (!item)
    {
        player->GetSession()->SendNotification("У вас нету предмета");
        return;
    }

    item->ClearEnchantment(PERM_ENCHANTMENT_SLOT);
    player->GetSession()->SendNotification("|cff800080%s |cffFF0000 успешно снята!", item->GetTemplate()->Name1.c_str());
}

class npc_enchantment : public CreatureScript
{
public:
    npc_enchantment() : CreatureScript("npc_enchantment") { }

    struct npc_enchantmentAI : public ScriptedAI
    {
        npc_enchantmentAI(Creature* creature) : ScriptedAI(creature) { }

        bool GossipHello(Player* player) override
        {
            AddGossipItemFor(player, 7, "|TInterface\\icons\\inv_helmet_164:20:20:-15:0|t[Зачарить: Голову]", GOSSIP_SENDER_MAIN, 4);
            AddGossipItemFor(player, 7, "|TInterface\\icons\\inv_shoulder_126:20:20:-15:0|t[Зачарить: Плечи]", GOSSIP_SENDER_MAIN, 5);
            AddGossipItemFor(player, 7, "|TInterface\\icons\\inv_chest_plate26:20:20:-15:0|t[Зачарить: Грудь]", GOSSIP_SENDER_MAIN, 7);
            AddGossipItemFor(player, 7, "|TInterface\\icons\\inv_pants_plate_37:20:20:-15:0|t[Зачарить: Ноги]", GOSSIP_SENDER_MAIN, 10);
            AddGossipItemFor(player, 7, "|TInterface\\icons\\inv_gauntlets_91:20:20:-15:0|t[Зачарить: Кисти рук]", GOSSIP_SENDER_MAIN, 9);
            AddGossipItemFor(player, 7, "|TInterface\\icons\\inv_bracer_43:20:20:-15:0|t[Зачарить: Запястья]", GOSSIP_SENDER_MAIN, 8);
            AddGossipItemFor(player, 7, "|TInterface\\icons\\inv_belt_48c:20:20:-15:0|t[Зачарить: Пояс]", GOSSIP_SENDER_MAIN, 15);
            AddGossipItemFor(player, 7, "|TInterface\\icons\\inv_boots_plate_13:20:20:-15:0|t[Зачарить: Ступни]", GOSSIP_SENDER_MAIN, 11);
            AddGossipItemFor(player, 7, "|TInterface\\icons\\inv_misc_cape_18:20:20:-15:0|t[Зачарить: Спину]", GOSSIP_SENDER_MAIN, 6);
            AddGossipItemFor(player, 7, "|TInterface\\icons\\inv_axe_113:20:20:-15:0|t[Зачарить: Оружие]", GOSSIP_SENDER_MAIN, 1);
            AddGossipItemFor(player, 7, "|TInterface\\icons\\inv_axe_116:20:20:-15:0|t[Зачарить: Оружие в левой руке]", GOSSIP_SENDER_MAIN, 13);
            AddGossipItemFor(player, 7, "|TInterface\\icons\\inv_axe_115:20:20:-15:0|t[Зачарить: Двуручное оружие]", GOSSIP_SENDER_MAIN, 2);
            AddGossipItemFor(player, 7, "|TInterface\\icons\\inv_shield_75:20:20:-15:0|t[Зачарить: Щит]", GOSSIP_SENDER_MAIN, 3);
            if (player->HasSkill(SKILL_ENCHANTING) && player->GetSkillValue(SKILL_ENCHANTING) == 450)
            {
                AddGossipItemFor(player, 7, "|TInterface\\icons\\inv_jewelry_ring_83:20:20:-15:0|t[Зачарить: Кольца]", GOSSIP_SENDER_MAIN, 12);
            }

            AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargenegative:20:20:-15:0|t[Снять инчант с вещей]", GOSSIP_SENDER_MAIN, 14);
            player->PlayerTalkClass->SendGossipMenu(100007, me->GetGUID());
            return true;
        }

        bool GossipSelect(Player* player, uint32 /*menuId*/, uint32 uiAction) override
        {
            uint32 const action = player->PlayerTalkClass->GetGossipOptionAction(uiAction);
            player->PlayerTalkClass->ClearMenus();
            Item * item;

            switch (action)
            {
            case 1: // Enchant Weapon

                AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|tОтведение удара", GOSSIP_SENDER_MAIN, 102);
                AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|tВытягивание Крови", GOSSIP_SENDER_MAIN, 103);
                AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+26 к ловкости", GOSSIP_SENDER_MAIN, 100);
                AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+45 к духу", GOSSIP_SENDER_MAIN, 101);
                AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|tБоевое исступление", GOSSIP_SENDER_MAIN, 104);
                AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+25 к рейтингу меткости, +25 к рейтингу критического удара", GOSSIP_SENDER_MAIN, 105);
                AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|tЧерная магия", GOSSIP_SENDER_MAIN, 106);
                AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|tВоеначальник", GOSSIP_SENDER_MAIN, 107);
                AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|tОружие Ледолома", GOSSIP_SENDER_MAIN, 108);
                AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|tОберег", GOSSIP_SENDER_MAIN, 109);
                AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+50 к выносливости", GOSSIP_SENDER_MAIN, 110);
                AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+65 к силе атаки", GOSSIP_SENDER_MAIN, 111);
                AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+63 к силе заклинаний", GOSSIP_SENDER_MAIN, 112);
                AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|tМангуст", GOSSIP_SENDER_MAIN, 113);
                AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|tПалач", GOSSIP_SENDER_MAIN, 114);
                AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|tЛеденящая стужа", GOSSIP_SENDER_MAIN, 701);
                AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargenegative:20:20:-15:0|tНазад", GOSSIP_SENDER_MAIN, 300);
                player->PlayerTalkClass->SendGossipMenu(100002, me->GetGUID());
                return true;
                break;
            case 2: // Enchant 2H Weapon
            {
                item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND);
                if (!item)
                {
                    player->GetSession()->SendNotification("Вы должны оборудовать двуручным оружием.");
                    GossipHello(player);
                    return false;
                }
                if (item->GetTemplate()->InventoryType == INVTYPE_2HWEAPON)
                {
                    AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|tБоевое исступление", GOSSIP_SENDER_MAIN, 104);
                    AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+81 к силе заклинаний", GOSSIP_SENDER_MAIN, 115);
                    AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+35 к ловкости", GOSSIP_SENDER_MAIN, 116);
                    AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+110 к силе атаки", GOSSIP_SENDER_MAIN, 117);
                    AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|tМангуст", GOSSIP_SENDER_MAIN, 113);
                    AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|tПалач", GOSSIP_SENDER_MAIN, 114);
                    AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|tЛеденящая стужа", GOSSIP_SENDER_MAIN, 701);
                    AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargenegative:20:20:-15:0|tНазад", GOSSIP_SENDER_MAIN, 300);
                    player->PlayerTalkClass->SendGossipMenu(100003, me->GetGUID());
                    return true;
                }
                else
                {
                    player->GetSession()->SendNotification("Вы должны двуручным оружием оборудованным.");
                    GossipHello(player);
                }
            }
            break;
            case 3: // Enchant Shield
            {
                item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
                if (!item)
                {
                    player->GetSession()->SendNotification("Вы должны оборудовать щитом.");
                    GossipHello(player);
                    return false;
                }
                if (item->GetTemplate()->InventoryType == INVTYPE_SHIELD)
                {
                    AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+20 к рейтингу защиты", GOSSIP_SENDER_MAIN, 118);
                    AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+25 к интелекту", GOSSIP_SENDER_MAIN, 119);
                    AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+12 к рейтингу устойчивости", GOSSIP_SENDER_MAIN, 120);
                    AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|tТитановая обшивка", GOSSIP_SENDER_MAIN, 121);
                    AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+18 к выносливости", GOSSIP_SENDER_MAIN, 122);
                    AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+36 к показателю блокирования", GOSSIP_SENDER_MAIN, 123);
                    AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargenegative:20:20:-15:0|tНазад", GOSSIP_SENDER_MAIN, 300);
                    player->PlayerTalkClass->SendGossipMenu(100004, me->GetGUID());
                    return true;
                }
                else
                {
                    player->GetSession()->SendNotification("У вас нету щита.");
                    GossipHello(player);
                }
            }
            break;
            case 4: // Enchant Head
                AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+30 к силе заклинаний и + 10 ед.маны каждые 5 секунд", GOSSIP_SENDER_MAIN, 124);
                AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+30 к силе заклинаний и 20 к рейтингу критического удара", GOSSIP_SENDER_MAIN, 125);
                AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+29 к силе заклинаний и +20 к рейтингу устойчивости", GOSSIP_SENDER_MAIN, 126);
                AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+30 к выносливости и + 25 к рейтингу устойчивости", GOSSIP_SENDER_MAIN, 127);
                AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+37 к выносливости и + 20 к рейтингу защиты", GOSSIP_SENDER_MAIN, 128);
                AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+50 к силе атаки и + 20 к рейтингу критического удара", GOSSIP_SENDER_MAIN, 129);
                AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+50 к силе атаки и +20 к рейтингу устойчивости", GOSSIP_SENDER_MAIN, 130);
                AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+25 к сопротивлению тайной магии и 30 к выносливости", GOSSIP_SENDER_MAIN, 131);
                AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+25 к сопротивлению огню и 30 к выносливости", GOSSIP_SENDER_MAIN, 132);
                AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+25 к сопротивлению темной магии и 30 к выносливости", GOSSIP_SENDER_MAIN, 133);
                AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+25 к сопротивлению магии льда и 30 к выносливости", GOSSIP_SENDER_MAIN, 134);
                AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+25 к сопротивлению силам природы и 30 к выносливости", GOSSIP_SENDER_MAIN, 135);
                AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargenegative:20:20:-15:0|tНазад", GOSSIP_SENDER_MAIN, 300);
                player->PlayerTalkClass->SendGossipMenu(100005, me->GetGUID());
                return true;
                break;
            case 5: // Enchant Shoulders
                if (player->HasSkill(SKILL_INSCRIPTION) && player->GetSkillValue(SKILL_INSCRIPTION) == 450)
                {
                    AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+120 к силе атаки и + 15 к рейтингу критического эффекта [|cffD80000+|r]", GOSSIP_SENDER_MAIN, 136);
                    AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+70 к силе заклинаний и +8 к мане каждые 5 секунд [|cffD80000+|r]", GOSSIP_SENDER_MAIN, 137);
                    AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+70 к силе заклинаний и +15 к рейтингу критического эффекта [|cffD80000+|r]", GOSSIP_SENDER_MAIN, 411);
                    AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+60 к рейтингу уклонения и + 15 к защите [|cffD80000+|r]", GOSSIP_SENDER_MAIN, 138);
                    // AddGossipItemFor(player,7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+25 к сопротивлению силам природы и 30 к выносливости", GOSSIP_SENDER_MAIN, 139);
                }
                AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+40 к силе атаки и +15 к рейтингу критического удара", GOSSIP_SENDER_MAIN, 140);
                AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+24 к силе заклинаний и +8 ед.маны каждые 5 секунд", GOSSIP_SENDER_MAIN, 141);
                AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+30 к выносливости и +15 к рейтингу устойчивости", GOSSIP_SENDER_MAIN, 142);
                AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+20 к рейтингу уклонения и +15 к рейтингу защиты", GOSSIP_SENDER_MAIN, 143);
                AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+24 к силе заклинаний и +15 к рейтингу критического удара", GOSSIP_SENDER_MAIN, 144);
                AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+23 к силе заклинаний и +15 к рейтингу устойчивости", GOSSIP_SENDER_MAIN, 145);
                AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+40 к силе атаки и + 15 крейтингу устойчивости", GOSSIP_SENDER_MAIN, 146);
                AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargenegative:20:20:-15:0|tНазад", GOSSIP_SENDER_MAIN, 300);
                player->PlayerTalkClass->SendGossipMenu(100006, me->GetGUID());
                return true;
                break;
            case 6: // Enchant Cloak
                if (player->HasSkill(SKILL_TAILORING) && player->GetSkillValue(SKILL_TAILORING) == 450)
                {
                    AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|tПружинистая паутинка [|cffD80000+|r]", GOSSIP_SENDER_MAIN, 147);
                    AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|tВышивка темного сияния [|cffD80000+|r]", GOSSIP_SENDER_MAIN, 149);
                    AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|tСветлотканая вышивка [|cffD80000+|r]", GOSSIP_SENDER_MAIN, 150);
                    AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|tВышивка в виде рукояти меча [|cffD80000+|r]", GOSSIP_SENDER_MAIN, 151);
                }
                if (player->HasSkill(SKILL_ENGINEERING) && player->GetSkillValue(SKILL_ENGINEERING) == 450)
                {
                    AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|tПружинистая паутинка +27 к силе заклинаний + парашут [|cffD80000+|r]", GOSSIP_SENDER_MAIN, 350);
                    AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|tЭластичная подкладка +23 к ловкости + парашут [|cffD80000+|r]", GOSSIP_SENDER_MAIN, 351);
                }
                AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+10 к духу и снижение угрозы на 2%", GOSSIP_SENDER_MAIN, 152);
                AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+16 к рейтингу защиты", GOSSIP_SENDER_MAIN, 153);
                AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+35 к проникающей способности заклинаний", GOSSIP_SENDER_MAIN, 154);
                AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+225 к броне", GOSSIP_SENDER_MAIN, 155);
                AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+22 к ловкости", GOSSIP_SENDER_MAIN, 156);
                AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+23 к рейтингу скорости", GOSSIP_SENDER_MAIN, 157);
                AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|tУлучшение незаметности и +10 к ловкости", GOSSIP_SENDER_MAIN, 148);
                AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargenegative:20:20:-15:0|tНазад", GOSSIP_SENDER_MAIN, 300);
                player->PlayerTalkClass->SendGossipMenu(100007, me->GetGUID());
                return true;
                break;
            case 7: //Enchant chest
                AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+10 ко всем характеристикам", GOSSIP_SENDER_MAIN, 158);
                AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+275 к здоровью", GOSSIP_SENDER_MAIN, 159);
                AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+10 ед.маны каждые 5 секунд", GOSSIP_SENDER_MAIN, 160);
                AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+20 к рейтингу устойчивости", GOSSIP_SENDER_MAIN, 161);
                AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+22 к рейтингу защиты", GOSSIP_SENDER_MAIN, 162);
                AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargenegative:20:20:-15:0|tНазад", GOSSIP_SENDER_MAIN, 300);
                player->PlayerTalkClass->SendGossipMenu(100008, me->GetGUID());
                return true;
                break;
            case 8: //Enchant Bracers            
                if (player->HasSkill(SKILL_LEATHERWORKING) && player->GetSkillValue(SKILL_LEATHERWORKING) == 450)
                {
                    AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+70 к сопротивлению тайной магии [|cffD80000+|r]", GOSSIP_SENDER_MAIN, 170);
                    AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+70 к сопротивлению огню [|cffD80000+|r]", GOSSIP_SENDER_MAIN, 171);
                    AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+70 к сопротивлению магии льда [|cffD80000+|r]", GOSSIP_SENDER_MAIN, 172);
                    AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+70 к сопротивлению силам природы [|cffD80000+|r]", GOSSIP_SENDER_MAIN, 173);
                    AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+70 к сопротивлению темной магии [|cffD80000+|r]", GOSSIP_SENDER_MAIN, 174);
                    AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+130 к силе атаки [|cffD80000+|r]", GOSSIP_SENDER_MAIN, 175);
                    AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+102 к выносливости [|cffD80000+|r]", GOSSIP_SENDER_MAIN, 176);
                    AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+76 к силе заклинаний [|cffD80000+|r]", GOSSIP_SENDER_MAIN, 177);
                }
                if (player->HasSkill(SKILL_BLACKSMITHING) && player->GetSkillValue(SKILL_BLACKSMITHING) == 450)
                {
                    AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|tДобавить место для сокета [|cffD80000+|r]", GOSSIP_SENDER_MAIN, 601);
                }
                AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+40 к выносливости", GOSSIP_SENDER_MAIN, 163);
                AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+30 к силе заклинаний", GOSSIP_SENDER_MAIN, 164);
                AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+50 к силе атаки", GOSSIP_SENDER_MAIN, 165);
                AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+18 к духу", GOSSIP_SENDER_MAIN, 166);
                AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+15 к рейтингу мастерства", GOSSIP_SENDER_MAIN, 167);
                AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+16 к интелекту", GOSSIP_SENDER_MAIN, 169);
                AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargenegative:20:20:-15:0|tНазад", GOSSIP_SENDER_MAIN, 300);
                player->PlayerTalkClass->SendGossipMenu(100009, me->GetGUID());
                return true;
                break;
            case 9: //Enchant Gloves		
                if (player->HasSkill(SKILL_ENGINEERING) && player->GetSkillValue(SKILL_ENGINEERING) == 450)
                {

                    AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|tГиперскоростные ускорители [|cffD80000+|r]", GOSSIP_SENDER_MAIN, 222);
                    AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|tНарукавная зажигательная ракетница [|cffD80000+|r]", GOSSIP_SENDER_MAIN, 223);

                }
                if (player->HasSkill(SKILL_BLACKSMITHING) && player->GetSkillValue(SKILL_BLACKSMITHING) == 450)
                {
                    AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|tДобавить место для сокета [|cffD80000+|r]", GOSSIP_SENDER_MAIN, 600);
                }
                AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+16 к рейтингу критического удара", GOSSIP_SENDER_MAIN, 178);
                AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+2% угрозы и + 10 к рейтингу парирования", GOSSIP_SENDER_MAIN, 179);
                AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+44 к силе атаки", GOSSIP_SENDER_MAIN, 180);
                AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+20 к ловкости", GOSSIP_SENDER_MAIN, 181);
                AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+20 к рейтингу меткости", GOSSIP_SENDER_MAIN, 182);
                AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+28 к силе заклинаний", GOSSIP_SENDER_MAIN, 168);
                AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargenegative:20:20:-15:0|tНазад", GOSSIP_SENDER_MAIN, 300);
                player->PlayerTalkClass->SendGossipMenu(100010, me->GetGUID());
                return true;
                break;
            case 10: //Enchant legs
                AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+40 к устойчивости и + 28 к выносливости", GOSSIP_SENDER_MAIN, 184);
                AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+55 к выносливости и +22 к ловкости", GOSSIP_SENDER_MAIN, 185);
                AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+75 к силе атаки и +22 к рейтингу критического удара", GOSSIP_SENDER_MAIN, 186);
                AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+50 к силе заклинаний и +20 к духу", GOSSIP_SENDER_MAIN, 187);
                AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+50 к силе заклинаний и +30 к выносливости", GOSSIP_SENDER_MAIN, 188);
                // AddGossipItemFor(player,7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+72 к выносливости и +35 к ловкости", GOSSIP_SENDER_MAIN, 189);
                // AddGossipItemFor(player,7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+100 к силе атаки и +36 к рейтингу критического удара", GOSSIP_SENDER_MAIN, 190);
                AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargenegative:20:20:-15:0|tНазад", GOSSIP_SENDER_MAIN, 300);
                player->PlayerTalkClass->SendGossipMenu(100017, me->GetGUID());
                return true;
                break;
            case 11: //Enchant feet
                AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+32 к силе атаки", GOSSIP_SENDER_MAIN, 191);
                AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+15 к выносливости и небольшое увеличение скорости", GOSSIP_SENDER_MAIN, 192);
                AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+16 к ловкости", GOSSIP_SENDER_MAIN, 193);
                AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+18 к духу", GOSSIP_SENDER_MAIN, 194);
                AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+7 ед. здоровья и маны каждые 5 сек", GOSSIP_SENDER_MAIN, 195);
                AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+12 к рейтингу меткости, и + 12 к рейтингу критического удара", GOSSIP_SENDER_MAIN, 196);
                AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+22 к выносливости", GOSSIP_SENDER_MAIN, 197);
                if (player->HasSkill(SKILL_ENGINEERING) && player->GetSkillValue(SKILL_ENGINEERING) == 450)
                {
                    AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|tНитро сапоги [|cffD80000+|r]", GOSSIP_SENDER_MAIN, 198);
                }
                AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargenegative:20:20:-15:0|tНазад", GOSSIP_SENDER_MAIN, 300);
                player->PlayerTalkClass->SendGossipMenu(100012, me->GetGUID());
                return true;
                break;
            case 12: //Enchant rings
                AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+40 к силе атаки [|cffD80000+|r]", GOSSIP_SENDER_MAIN, 202);
                AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+23 к силе заклинаний [|cffD80000+|r]", GOSSIP_SENDER_MAIN, 203);
                AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+30 к выносливости [|cffD80000+|r]", GOSSIP_SENDER_MAIN, 204);
                AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargenegative:20:20:-15:0|tНазад", GOSSIP_SENDER_MAIN, 300);
                player->PlayerTalkClass->SendGossipMenu(100013, me->GetGUID());
                return true;
                break;
            case 13: //Enchant Off-Hand weapons
            {
                item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
                if (!item)
                {
                    player->GetSession()->SendNotification("Это заточки нужен одной рукой оружие оборудованная в левой руке.");
                    GossipHello(player);
                    return false;
                }
                if (item->GetTemplate()->InventoryType == INVTYPE_WEAPON)
                {
                    AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|tОтведение удара", GOSSIP_SENDER_MAIN, 205);
                    AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|tВытягивание Крови", GOSSIP_SENDER_MAIN, 219);
                    AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+26 к ловкости", GOSSIP_SENDER_MAIN, 206);
                    AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+45 к духу", GOSSIP_SENDER_MAIN, 207);
                    AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|tБоевое исступление", GOSSIP_SENDER_MAIN, 208);
                    AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+25 к рейтингу меткости, +25 к рейтингу критического удара", GOSSIP_SENDER_MAIN, 209);
                    AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|tЧерная магия", GOSSIP_SENDER_MAIN, 210);
                    AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|tВоеначальник", GOSSIP_SENDER_MAIN, 211);
                    AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|tОружие Ледолома", GOSSIP_SENDER_MAIN, 212);
                    AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|tОберег", GOSSIP_SENDER_MAIN, 213);
                    AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+50 к выносливости", GOSSIP_SENDER_MAIN, 214);
                    AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+65 к силе атаки", GOSSIP_SENDER_MAIN, 215);
                    AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|t+63 к силе заклинаний", GOSSIP_SENDER_MAIN, 216);
                    AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|tМангуст", GOSSIP_SENDER_MAIN, 217);
                    AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|tПалач", GOSSIP_SENDER_MAIN, 218);
                    AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargenegative:20:20:-15:0|tНазад", GOSSIP_SENDER_MAIN, 300);
                    player->PlayerTalkClass->SendGossipMenu(100002, me->GetGUID());
                    return true;
                }
                else
                {
                    player->GetSession()->SendNotification("В левой руке не является оружием.");
                    GossipHello(player);
                }
            }
            break;
            case 15: //Enchant poyas                        
                AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|tДобавить место для сокета", GOSSIP_SENDER_MAIN, 220);
                if (player->HasSkill(SKILL_ENGINEERING) && player->GetSkillValue(SKILL_ENGINEERING) == 450)
                {
                    AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|tНаременные осколочные гранаты [|cffD80000+|r]", GOSSIP_SENDER_MAIN, 221);
                    AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargepositive:20:20:-15:0|tГенератор ЭМИ [|cffD80000+|r]", GOSSIP_SENDER_MAIN, 299);
                }
                AddGossipItemFor(player, 7, "|TInterface\\icons\\Spell_chargenegative:20:20:-15:0|tНазад", GOSSIP_SENDER_MAIN, 300);
                player->PlayerTalkClass->SendGossipMenu(100513, me->GetGUID());
                return true;
                break;
            case 100:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_AGILITY_1H);
                GossipHello(player);
                break;
            case 101:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_SPIRIT);
                GossipHello(player);
                break;
            case 102:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_BLADE_WARD);
                GossipHello(player);
                break;
            case 103:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_BLOOD_DRAINING);
                GossipHello(player);
                break;
            case 104:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_BERSERKING);
                GossipHello(player);
                break;
            case 105:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_ACCURACY);
                GossipHello(player);
                break;
            case 106:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_BLACK_MAGIC);
                GossipHello(player);
                break;
            case 107:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_BATTLEMASTER);
                GossipHello(player);
                break;
            case 108:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_ICEBREAKER);
                GossipHello(player);
                break;
            case 109:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_LIFEWARD);
                GossipHello(player);
                break;
            case 110:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_TITANGUARD);
                GossipHello(player);
                break;
            case 111:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_POTENCY);
                GossipHello(player);
                break;
            case 112:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_MIGHTY_SPELL_POWER);
                GossipHello(player);
                break;
            case 113:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_2WEP_MONGOOSE);
                GossipHello(player);
                break;
            case 114:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_EXECUTIONER);
                GossipHello(player);
                break;
            case 701:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_STRYJA);
                GossipHello(player);
                break;
            case 115:
            {
                item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND);
                if (!item)
                {
                    player->GetSession()->SendAreaTriggerMessage("This enchant needs a 2H weapon equipped.");
                    GossipHello(player);
                    return false;
                }
                if (item->GetTemplate()->InventoryType == INVTYPE_2HWEAPON)
                {
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_2WEP_GREATER_SPELL_POWER);
                    GossipHello(player);
                }
                else
                {
                    player->GetSession()->SendAreaTriggerMessage("You don't have a Two-Handed weapon equipped.");
                    GossipHello(player);
                }
            }
            break;
            case 116:
            {
                item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND);
                if (!item)
                {
                    player->GetSession()->SendAreaTriggerMessage("This enchant needs a 2H weapon equipped.");
                    GossipHello(player);
                    return false;
                }
                if (item->GetTemplate()->InventoryType == INVTYPE_2HWEAPON)
                {
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_2WEP_AGILITY);
                    GossipHello(player);
                }
                else
                {
                    player->GetSession()->SendAreaTriggerMessage("You don't have a Two-Handed weapon equipped.");
                    GossipHello(player);
                }
            }
            break;
            case 117:
            {
                item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND);
                if (!item)
                {
                    player->GetSession()->SendAreaTriggerMessage("This enchant needs a 2H weapon equipped.");
                    GossipHello(player);
                    return false;
                }
                if (item->GetTemplate()->InventoryType == INVTYPE_2HWEAPON)
                {
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_2WEP_MASSACRE);
                    GossipHello(player);
                }
                else
                {
                    player->GetSession()->SendAreaTriggerMessage("You don't have a Two-Handed weapon equipped.");
                    GossipHello(player);
                }
            }
            break;
            case 118:
            {
                item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
                if (!item)
                {
                    player->GetSession()->SendAreaTriggerMessage("This enchant needs a shield equipped.");
                    GossipHello(player);
                    return false;
                }
                if (item->GetTemplate()->InventoryType == INVTYPE_SHIELD)
                {
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_SHIELD_DEFENSE);
                    GossipHello(player);
                }
                else
                {
                    player->GetSession()->SendAreaTriggerMessage("You don't have a shield equipped.");
                    GossipHello(player);
                }
            }
            break;
            case 119:
            {
                item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
                if (!item)
                {
                    player->GetSession()->SendAreaTriggerMessage("This enchant needs a shield equipped.");
                    GossipHello(player);
                    return false;
                }
                if (item->GetTemplate()->InventoryType == INVTYPE_SHIELD)
                {
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_SHIELD_INTELLECT);
                    GossipHello(player);
                }
                else
                {
                    player->GetSession()->SendAreaTriggerMessage("You don't have a shield equipped.");
                    GossipHello(player);
                }
            }
            break;
            case 120:
            {
                item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);

                if (!item)
                {
                    player->GetSession()->SendAreaTriggerMessage("This enchant needs a shield equipped.");
                    GossipHello(player);
                    return false;
                }

                if (item->GetTemplate()->InventoryType == INVTYPE_SHIELD)
                {
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_SHIELD_RESILIENCE);
                    GossipHello(player);
                }
                else
                {
                    player->GetSession()->SendAreaTriggerMessage("You don't have a shield equipped.");
                    GossipHello(player);
                }
            }
            break;
            case 121:
            {
                item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
                if (!item)
                {
                    player->GetSession()->SendAreaTriggerMessage("This enchant needs a shield equipped.");
                    GossipHello(player);
                    return false;
                }

                if (item->GetTemplate()->InventoryType == INVTYPE_SHIELD)
                {
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_SHIELD_TITANIUM_PLATING);
                    GossipHello(player);
                }
                else
                {
                    player->GetSession()->SendAreaTriggerMessage("You don't have a shield equipped.");
                    GossipHello(player);
                }
            }
            break;
            case 122:
            {
                item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
                if (!item)
                {
                    player->GetSession()->SendAreaTriggerMessage("This enchant needs a shield equipped.");
                    GossipHello(player);
                    return false;
                }
                if (item->GetTemplate()->InventoryType == INVTYPE_SHIELD)
                {
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_SHIELD_STAMINA);
                    GossipHello(player);
                }
                else
                {
                    player->GetSession()->SendAreaTriggerMessage("You don't have a shield equipped.");
                    GossipHello(player);
                }
            }
            break;
            case 123:
            {
                item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
                if (!item)
                {
                    player->GetSession()->SendAreaTriggerMessage("This enchant needs a shield equipped.");
                    GossipHello(player);
                    return false;
                }
                if (item->GetTemplate()->InventoryType == INVTYPE_SHIELD)
                {
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_SHIELD_TOUGHSHIELD);
                    GossipHello(player);
                }
                else
                {
                    player->GetSession()->SendAreaTriggerMessage("You don't have a shield equipped.");
                    GossipHello(player);
                }
            }
            break;
            case 124:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD), ENCHANT_HEAD_BLISSFUL_MENDING);
                GossipHello(player);
                break;
            case 125:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD), ENCHANT_HEAD_BURNING_MYSTERIES);
                GossipHello(player);
                break;
            case 126:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD), ENCHANT_HEAD_DOMINANCE);
                GossipHello(player);
                break;
            case 127:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD), ENCHANT_HEAD_SAVAGE_GLADIATOR);
                GossipHello(player);
                break;
            case 128:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD), ENCHANT_HEAD_STALWART_PROTECTOR);
                GossipHello(player);
                break;
            case 129:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD), ENCHANT_HEAD_TORMENT);
                GossipHello(player);
                break;
            case 130:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD), ENCHANT_HEAD_TRIUMPH);
                GossipHello(player);
                break;
            case 131:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD), ENCHANT_HEAD_ECLIPSED_MOON);
                GossipHello(player);
                break;
            case 132:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD), ENCHANT_HEAD_FLAME_SOUL);
                GossipHello(player);
                break;
            case 133:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD), ENCHANT_HEAD_FLEEING_SHADOW);
                GossipHello(player);
                break;
            case 134:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD), ENCHANT_HEAD_FROSTY_SOUL);
                GossipHello(player);
                break;
            case 135:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD), ENCHANT_HEAD_TOXIC_WARDING);
                GossipHello(player);
                break;
            case 136:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_SHOULDERS), ENCHANT_SHOULDER_MASTERS_AXE);
                GossipHello(player);
                break;
            case 137:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_SHOULDERS), ENCHANT_SHOULDER_MASTERS_CRAG);
                GossipHello(player);
                break;
            case 411:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_SHOULDERS), ENCHANT_SHOULDER_KRIT);
                GossipHello(player);
                break;
            case 138:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_SHOULDERS), ENCHANT_SHOULDER_MASTERS_PINNACLE);
                GossipHello(player);
                break;
            case 139:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_SHOULDERS), ENCHANT_SHOULDER_MASTERS_STORM);
                GossipHello(player);
                break;
            case 140:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_SHOULDERS), ENCHANT_SHOULDER_GREATER_AXE);
                GossipHello(player);
                break;
            case 141:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_SHOULDERS), ENCHANT_SHOULDER_GREATER_CRAG);
                GossipHello(player);
                break;
            case 142:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_SHOULDERS), ENCHANT_SHOULDER_GREATER_GLADIATOR);
                GossipHello(player);
                break;
            case 143:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_SHOULDERS), ENCHANT_SHOULDER_GREATER_PINNACLE);
                GossipHello(player);
                break;
            case 144:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_SHOULDERS), ENCHANT_SHOULDER_GREATER_STORM);
                GossipHello(player);
                break;
            case 145:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_SHOULDERS), ENCHANT_SHOULDER_DOMINANCE);
                GossipHello(player);
                break;
            case 146:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_SHOULDERS), ENCHANT_SHOULDER_TRIUMPH);
                GossipHello(player);
                break;
            case 147:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK), ENCHANT_CLOAK_SPRINGY_ARACHNOWEAVE);
                GossipHello(player);
                break;
            case 148:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK), ENCHANT_CLOAK_SHADOW_ARMOR);
                GossipHello(player);
                break;
            case 149:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK), ENCHANT_CLOAK_DARKGLOW_EMBROIDERY);
                GossipHello(player);
                break;
            case 150:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK), ENCHANT_CLOAK_LIGHTWEAVE_EMBROIDERY);
                GossipHello(player);
                break;
            case 151:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK), ENCHANT_CLOAK_SWORDGUARD_EMBROIDERY);
                GossipHello(player);
                break;
            case 351:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK), ENCHANT_SPINADVA);
                GossipHello(player);
                break;
            case 350:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK), ENCHANT_SPINA);
                GossipHello(player);
                break;
            case 152:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK), ENCHANT_CLOAK_WISDOM);
                GossipHello(player);
                break;
            case 153:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK), ENCHANT_CLOAK_TITANWEAVE);
                GossipHello(player);
                break;
            case 154:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK), ENCHANT_CLOAK_SPELL_PIERCING);
                GossipHello(player);
                break;
            case 155:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK), ENCHANT_CLOAK_MIGHTY_ARMOR);
                GossipHello(player);
                break;
            case 156:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK), ENCHANT_CLOAK_MAJOR_AGILITY);
                GossipHello(player);
                break;
            case 157:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK), ENCHANT_CLOAK_GREATER_SPEED);
                GossipHello(player);
                break;
            case 158:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_CHEST), ENCHANT_CHEST_POWERFUL_STATS);
                GossipHello(player);
                break;
            case 159:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_CHEST), ENCHANT_CHEST_SUPER_HEALTH);
                GossipHello(player);
                break;
            case 160:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_CHEST), ENCHANT_CHEST_GREATER_MAINA_REST);
                GossipHello(player);
                break;
            case 161:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_CHEST), ENCHANT_CHEST_EXCEPTIONAL_RESIL);
                GossipHello(player);
                break;
            case 162:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_CHEST), ENCHANT_CHEST_GREATER_DEFENSE);
                GossipHello(player);
                break;
            case 163:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_MAJOR_STAMINA);
                GossipHello(player);
                break;
            case 601:
                AdvancedEnchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), PRISMATIC_ENCHANTMENT_SLOT, ENCHANT_ZAPYASTIE_SOCKET); // Socket Belt		
                GossipHello(player);
                break;
            case 164:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_SUPERIOR_SP);
                GossipHello(player);
                break;
            case 165:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_GREATER_ASSUALT);
                GossipHello(player);
                break;
            case 166:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_MAJOR_SPIRT);
                GossipHello(player);
                break;
            case 167:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_EXPERTISE);
                GossipHello(player);
                break;
            case 169:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_INTELLECT);
                GossipHello(player);
                break;
            case 170:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_FURL_ARCANE);
                GossipHello(player);
                break;
            case 171:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_FURL_FIRE);
                GossipHello(player);
                break;
            case 172:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_FURL_FROST);
                GossipHello(player);
                break;
            case 173:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_FURL_NATURE);
                GossipHello(player);
                break;
            case 174:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_FURL_SHADOW);
                GossipHello(player);
                break;
            case 175:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_FURL_ATTACK);
                GossipHello(player);
                break;
            case 176:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_FURL_STAMINA);
                GossipHello(player);
                break;
            case 177:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_FURL_SPELLPOWER);
                GossipHello(player);
                break;
            case 178:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HANDS), ENCHANT_GLOVES_GREATER_BLASTING);
                GossipHello(player);
                break;
            case 179:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HANDS), ENCHANT_GLOVES_ARMSMAN);
                GossipHello(player);
                break;
            case 180:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HANDS), ENCHANT_GLOVES_CRUSHER);
                GossipHello(player);
                break;
            case 181:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HANDS), ENCHANT_GLOVES_AGILITY);
                GossipHello(player);
                break;
            case 182:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HANDS), ENCHANT_GLOVES_PRECISION);
                GossipHello(player);
                break;
            case 600:
                AdvancedEnchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HANDS), PRISMATIC_ENCHANTMENT_SLOT, ENCHANT_KISTIRYK_SOCKET); // Socket Belt
                GossipHello(player);
                break;
            case 168:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HANDS), ENCHANT_GLOVES_SPD);
                GossipHello(player);
                break;
            case 184:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_LEGS), ENCHANT_LEG_EARTHEN);
                GossipHello(player);
                break;
            case 185:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_LEGS), ENCHANT_LEG_FROSTHIDE);
                GossipHello(player);
                break;
            case 186:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_LEGS), ENCHANT_LEG_ICESCALE);
                GossipHello(player);
                break;
            case 187:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_LEGS), ENCHANT_LEG_BRILLIANT_SPELLTHREAD);
                GossipHello(player);
                break;
            case 188:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_LEGS), ENCHANT_LEG_SAPPHIRE_SPELLTHREAD);
                GossipHello(player);
                break;
            case 189:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_LEGS), ENCHANT_LEG_DRAGONSCALE);
                GossipHello(player);
                break;
            case 190:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_LEGS), ENCHANT_LEG_WYRMSCALE);
                GossipHello(player);
                break;
            case 191:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FEET), ENCHANT_BOOTS_GREATER_ASSULT);
                GossipHello(player);
                break;
            case 192:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FEET), ENCHANT_BOOTS_TUSKARS_VITLIATY);
                GossipHello(player);
                break;
            case 193:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FEET), ENCHANT_BOOTS_SUPERIOR_AGILITY);
                GossipHello(player);
                break;
            case 194:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FEET), ENCHANT_BOOTS_GREATER_SPIRIT);
                GossipHello(player);
                break;
            case 195:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FEET), ENCHANT_BOOTS_GREATER_VITALITY);
                GossipHello(player);
                break;
            case 196:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FEET), ENCHANT_BOOTS_ICEWALKER);
                GossipHello(player);
                break;
            case 197:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FEET), ENCHANT_BOOTS_GREATER_FORTITUDE);
                GossipHello(player);
                break;
            case 198:
                item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FEET);
                if (!item)
                {
                    player->GetSession()->SendAreaTriggerMessage("Вы должны одеть боты.");
                    player->PlayerTalkClass->SendCloseGossip();
                    return false;
                }
                if (item->GetTemplate()->ItemLevel>150)
                {
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FEET), ENCHANT_BOOTS_NITRO_BOOTS);
                    GossipHello(player);
                }
                else
                {
                    player->GetSession()->SendAreaTriggerMessage("Вы не можете зачарить данный итем");
                    GossipHello(player);
                }
                break;
            case 202:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FINGER1), ENCHANT_RING_ASSULT);
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FINGER2), ENCHANT_RING_ASSULT);
                GossipHello(player);
                break;
            case 203:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FINGER1), ENCHANT_RING_GREATER_SP);
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FINGER2), ENCHANT_RING_GREATER_SP);
                GossipHello(player);
                break;
            case 204:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FINGER1), ENCHANT_RING_STAMINA);
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FINGER2), ENCHANT_RING_STAMINA);
                GossipHello(player);
                break;
            case 220:
                AdvancedEnchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WAIST), PRISMATIC_ENCHANTMENT_SLOT, ENCHANT_WAIST_SOCKET);
                GossipHello(player);
                break;
            case 221:
                item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WAIST);
                if (!item)
                {
                    player->GetSession()->SendAreaTriggerMessage("Вы должны одеть пояс.");
                    player->PlayerTalkClass->SendCloseGossip();
                    return false;
                }
                if (item->GetTemplate()->ItemLevel>150)
                {
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WAIST), ENCHANT_BOMBA);
                    GossipHello(player);
                }
                else
                {
                    player->GetSession()->SendAreaTriggerMessage("Вы не можете зачарить данный итем");
                    GossipHello(player);
                }
                break;
            case 299: // 
                item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WAIST);
                if (!item)
                {
                    player->GetSession()->SendAreaTriggerMessage("Вы должны одеть пояс.");
                    player->PlayerTalkClass->SendCloseGossip();
                    return false;
                }
                if (item->GetTemplate()->ItemLevel>150)
                {
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WAIST), ENCHANT_VZRIV);
                    GossipHello(player);
                }
                else
                {
                    player->GetSession()->SendAreaTriggerMessage("Вы не можете зачарить данный итем");
                    GossipHello(player);
                }
                break;
            case 222:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HANDS), ENCHANT_GLOVES_HAST);
                GossipHello(player);
                break;
            case 223:
                Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HANDS), ENCHANT_GLOVES_RAKETA);
                GossipHello(player);
                break;
            case 205:
            {
                item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
                if (!item)
                {
                    player->GetSession()->SendAreaTriggerMessage("This enchant needs a one-hand weapon equipped in the off-hand.");
                    GossipHello(player);
                    return false;
                }
                if (item->GetTemplate()->InventoryType == INVTYPE_WEAPON)
                {
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_BLADE_WARD);
                    GossipHello(player);
                }
                else
                {
                    player->GetSession()->SendAreaTriggerMessage("This enchant needs a one-hand weapon equipped in the off-hand.");
                    GossipHello(player);
                }
            }
            break;

            case 206:
            {
                item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
                if (!item)
                {
                    player->GetSession()->SendAreaTriggerMessage("This enchant needs a one-hand weapon equipped in the off-hand.");
                    GossipHello(player);
                    return false;
                }
                if (item->GetTemplate()->InventoryType == INVTYPE_WEAPON)
                {
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_AGILITY_1H);
                    GossipHello(player);
                }
                else
                {
                    player->GetSession()->SendAreaTriggerMessage("This enchant needs a one-hand weapon equipped in the off-hand.");
                    GossipHello(player);
                }
            }
            break;

            case 207:
            {
                item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
                if (!item)
                {
                    player->GetSession()->SendAreaTriggerMessage("This enchant needs a one-hand weapon equipped in the off-hand.");
                    GossipHello(player);
                    return false;
                }
                if (item->GetTemplate()->InventoryType == INVTYPE_WEAPON)
                {
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_SPIRIT);
                    GossipHello(player);
                }
                else
                {
                    player->GetSession()->SendAreaTriggerMessage("This enchant needs a one-hand weapon equipped in the off-hand.");
                    GossipHello(player);
                }
            }
            break;

            case 208:
            {
                item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
                if (!item)
                {
                    player->GetSession()->SendAreaTriggerMessage("This enchant needs a one-hand weapon equipped in the off-hand.");
                    GossipHello(player);
                    return false;
                }
                if (item->GetTemplate()->InventoryType == INVTYPE_WEAPON)
                {
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_BERSERKING);
                    GossipHello(player);
                }
                else
                {
                    player->GetSession()->SendAreaTriggerMessage("This enchant needs a one-hand weapon equipped in the off-hand.");
                    GossipHello(player);
                }
            }
            break;
            case 209:
            {
                item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
                if (!item)
                {
                    player->GetSession()->SendAreaTriggerMessage("This enchant needs a one-hand weapon equipped in the off-hand.");
                    GossipHello(player);
                    return false;
                }
                if (item->GetTemplate()->InventoryType == INVTYPE_WEAPON)
                {
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_ACCURACY);
                    GossipHello(player);
                }
                else
                {
                    player->GetSession()->SendAreaTriggerMessage("This enchant needs a one-hand weapon equipped in the off-hand.");
                    GossipHello(player);
                }
            }
            break;
            case 210:
            {
                item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
                if (!item)
                {
                    player->GetSession()->SendAreaTriggerMessage("This enchant needs a one-hand weapon equipped in the off-hand.");
                    GossipHello(player);
                    return false;
                }
                if (item->GetTemplate()->InventoryType == INVTYPE_WEAPON)
                {
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_BLACK_MAGIC);
                    GossipHello(player);
                }
                else
                {
                    player->GetSession()->SendAreaTriggerMessage("This enchant needs a one-hand weapon equipped in the off-hand.");
                    GossipHello(player);
                }
            }
            break;
            case 211:
            {
                item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
                if (!item)
                {
                    player->GetSession()->SendAreaTriggerMessage("This enchant needs a one-hand weapon equipped in the off-hand.");
                    GossipHello(player);
                    return false;
                }
                if (item->GetTemplate()->InventoryType == INVTYPE_WEAPON)
                {
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_BATTLEMASTER);
                    GossipHello(player);
                }
                else
                {
                    player->GetSession()->SendAreaTriggerMessage("This enchant needs a one-hand weapon equipped in the off-hand.");
                    GossipHello(player);
                }
            }
            break;

            case 212:
            {
                item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
                if (!item)
                {
                    player->GetSession()->SendAreaTriggerMessage("This enchant needs a one-hand weapon equipped in the off-hand.");
                    GossipHello(player);
                    return false;
                }
                if (item->GetTemplate()->InventoryType == INVTYPE_WEAPON)
                {
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_ICEBREAKER);
                    GossipHello(player);
                }
                else
                {
                    player->GetSession()->SendAreaTriggerMessage("This enchant needs a one-hand weapon equipped in the off-hand.");
                    GossipHello(player);
                }
            }
            break;
            case 213:
            {
                item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
                if (!item)
                {
                    player->GetSession()->SendAreaTriggerMessage("This enchant needs a one-hand weapon equipped in the off-hand.");
                    GossipHello(player);
                    return false;
                }
                if (item->GetTemplate()->InventoryType == INVTYPE_WEAPON)
                {
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_LIFEWARD);
                    GossipHello(player);
                }
                else
                {
                    player->GetSession()->SendAreaTriggerMessage("This enchant needs a one-hand weapon equipped in the off-hand.");
                    GossipHello(player);
                }
            }
            break;
            case 214:
            {
                item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
                if (!item)
                {
                    player->GetSession()->SendAreaTriggerMessage("This enchant needs a one-hand weapon equipped in the off-hand.");
                    GossipHello(player);
                    return false;
                }
                if (item->GetTemplate()->InventoryType == INVTYPE_WEAPON)
                {
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_TITANGUARD);
                    GossipHello(player);
                }
                else
                {
                    player->GetSession()->SendAreaTriggerMessage("This enchant needs a one-hand weapon equipped in the off-hand.");
                    GossipHello(player);
                }
            }
            break;
            case 215:
            {
                item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
                if (!item)
                {
                    player->GetSession()->SendAreaTriggerMessage("This enchant needs a one-hand weapon equipped in the off-hand.");
                    GossipHello(player);
                    return false;
                }
                if (item->GetTemplate()->InventoryType == INVTYPE_WEAPON)
                {
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_POTENCY);
                    GossipHello(player);
                }
                else
                {
                    player->GetSession()->SendAreaTriggerMessage("This enchant needs a one-hand weapon equipped in the off-hand.");
                    GossipHello(player);
                }
            }
            break;

            case 216:
            {
                item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
                if (!item)
                {
                    player->GetSession()->SendAreaTriggerMessage("This enchant needs a one-hand weapon equipped in the off-hand.");
                    GossipHello(player);
                    return false;
                }
                if (item->GetTemplate()->InventoryType == INVTYPE_WEAPON)
                {
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_MIGHTY_SPELL_POWER);
                    GossipHello(player);
                }
                else
                {
                    player->GetSession()->SendAreaTriggerMessage("This enchant needs a one-hand weapon equipped in the off-hand.");
                    GossipHello(player);
                }
            }
            break;

            case 217:
            {
                item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
                if (!item)
                {
                    player->GetSession()->SendAreaTriggerMessage("This enchant needs a one-hand weapon equipped in the off-hand.");
                    GossipHello(player);
                    return false;
                }
                if (item->GetTemplate()->InventoryType == INVTYPE_WEAPON)
                {
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_2WEP_MONGOOSE);
                    GossipHello(player);
                }
                else
                {
                    player->GetSession()->SendAreaTriggerMessage("This enchant needs a one-hand weapon equipped in the off-hand.");
                    GossipHello(player);
                }
            }
            break;

            case 218:
            {
                item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
                if (!item)
                {
                    player->GetSession()->SendAreaTriggerMessage("This enchant needs a one-hand weapon equipped in the off-hand.");
                    GossipHello(player);
                    return false;
                }
                if (item->GetTemplate()->InventoryType == INVTYPE_WEAPON)
                {
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_EXECUTIONER);
                    GossipHello(player);
                }
                else
                {
                    player->GetSession()->SendAreaTriggerMessage("This enchant needs a one-hand weapon equipped in the off-hand.");
                    GossipHello(player);
                }
            }
            break;
            case 219:
            {
                item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
                if (!item)
                {
                    player->GetSession()->SendAreaTriggerMessage("This enchant needs a one-hand weapon equipped in the off-hand.");
                    GossipHello(player);
                    return false;
                }
                if (item->GetTemplate()->InventoryType == INVTYPE_WEAPON)
                {
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_BLOOD_DRAINING);
                    GossipHello(player);
                }
                else
                {
                    player->GetSession()->SendAreaTriggerMessage("This enchant needs a one-hand weapon equipped in the off-hand.");
                    GossipHello(player);
                }
            }
            break;
            case 300: //<-Back menu
            {
                AddGossipItemFor(player, 7, "|TInterface\\icons\\inv_helmet_164:20:20:-15:0|t[Зачарить: Голову]", GOSSIP_SENDER_MAIN, 4);
                AddGossipItemFor(player, 7, "|TInterface\\icons\\inv_shoulder_126:20:20:-15:0|t[Зачарить: Плечи]", GOSSIP_SENDER_MAIN, 5);
                AddGossipItemFor(player, 7, "|TInterface\\icons\\inv_chest_plate26:20:20:-15:0|t[Зачарить: Грудь]", GOSSIP_SENDER_MAIN, 7);
                AddGossipItemFor(player, 7, "|TInterface\\icons\\inv_pants_plate_37:20:20:-15:0|t[Зачарить: Ноги]", GOSSIP_SENDER_MAIN, 10);
                AddGossipItemFor(player, 7, "|TInterface\\icons\\inv_gauntlets_91:20:20:-15:0|t[Зачарить: Кисти рук]", GOSSIP_SENDER_MAIN, 9);
                AddGossipItemFor(player, 7, "|TInterface\\icons\\inv_bracer_43:20:20:-15:0|t[Зачарить: Запястья]", GOSSIP_SENDER_MAIN, 8);
                AddGossipItemFor(player, 7, "|TInterface\\icons\\inv_belt_48c:20:20:-15:0|t[Зачарить: Пояс]", GOSSIP_SENDER_MAIN, 15);
                AddGossipItemFor(player, 7, "|TInterface\\icons\\inv_boots_plate_13:20:20:-15:0|t[Зачарить: Ступни]", GOSSIP_SENDER_MAIN, 11);
                AddGossipItemFor(player, 7, "|TInterface\\icons\\inv_misc_cape_18:20:20:-15:0|t[Зачарить: Спину]", GOSSIP_SENDER_MAIN, 6);
                AddGossipItemFor(player, 7, "|TInterface\\icons\\inv_axe_113:20:20:-15:0|t[Зачарить: Оружие]", GOSSIP_SENDER_MAIN, 1);
                AddGossipItemFor(player, 7, "|TInterface\\icons\\inv_axe_116:20:20:-15:0|t[Зачарить: Оружие в левой руке]", GOSSIP_SENDER_MAIN, 13);
                AddGossipItemFor(player, 7, "|TInterface\\icons\\inv_axe_115:20:20:-15:0|t[Зачарить: Двуручное оружие]", GOSSIP_SENDER_MAIN, 2);
                AddGossipItemFor(player, 7, "|TInterface\\icons\\inv_shield_75:20:20:-15:0|t[Зачарить: Щит]", GOSSIP_SENDER_MAIN, 3);
                if (player->HasSkill(SKILL_ENCHANTING) && player->GetSkillValue(SKILL_ENCHANTING) == 450)
                {
                    AddGossipItemFor(player, 7, "|TInterface\\icons\\inv_jewelry_ring_83:20:20:-15:0|t[Зачарить: Кольца]", GOSSIP_SENDER_MAIN, 12);
                }
                player->PlayerTalkClass->SendGossipMenu(100007, me->GetGUID());
                return true;
            }
            break;

            }
            return true;
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_enchantmentAI(creature);
    }
};

void AddSC_npc_enchantment()
{
    new npc_enchantment();
}