#include "ScriptPCH.h"
#include "SpellScript.h"
#include <sstream>
#include "../pvp_zone.h"
#include "ScriptedGossip.h"
#include "Chat.h"
#include "World.h"
#include "WorldSession.h"

std::string to_string(uint32 value)
{
    std::ostringstream stream;
    stream << value;
    return stream.str();
}

class npc_Teleporter : public CreatureScript
{
public:
    npc_Teleporter() : CreatureScript("npc_Teleporter") { }

    struct npc_TeleporterAI : public ScriptedAI
    {
        npc_TeleporterAI(Creature* creature) : ScriptedAI(creature) { }

    std::string GetZoneOnline()
    {
        SessionMap const& sessions = sWorld->GetAllSessions();
        uint32 count = 0;

        for (SessionMap::const_iterator it = sessions.begin(); it != sessions.end(); ++it)
        {
            if (Player* player = it->second->GetPlayer())
            {
                if (player->IsInWorld() && player->GetZoneId() == 616)
                    ++count;
            }
        }
        if (count < 1 || count >= 5) {
            std::string str = "|cffE80000" + to_string(count) + " игроков|r";
            return str;
        }
        if (count == 1) {
            std::string str = "|cffE80000" + to_string(count) + " игрок|r";
            return str;
        }
        if (count == 2 || count == 3 || count == 4) {
            std::string str = "|cffE80000" + to_string(count) + " игрока|r";
            return str;
        }

        return "GetZoneOnline::error";
    }

    bool GossipHello(Player *player) override
    {
        if (player->GetTeamId() == TEAM_ALLIANCE)
        {
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Spell_Arcane_TeleportStormWind:30:30:-18:0|t Штормград", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1, "Вы уверены, что вы хотите попасть в Штормград?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Spell_Arcane_TeleportDarnassus:30:30:-18:0|t Дарнас", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2, "Вы уверены, что вы хотите попасть в Дарнас?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Spell_Arcane_TeleportIronForge:30:30:-18:0|t Стальгорн", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3, "Вы уверены, что вы хотите попасть в Стальгорн?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Spell_Arcane_TeleportExodar:30:30:-18:0|t Эксодар", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4, "Вы уверены, что вы хотите попасть в Эксодар?", 0, false);
        }
        else
        {
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Spell_Arcane_TeleportOrgrimmar:30:30:-18:0|t Оргриммар", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5, "Вы уверены, что вы хотите попасть в Оргриммар?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Spell_Arcane_TeleportThunderBluff:30:30:-18:0|t Громовой утес", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6, "Вы уверены, что вы хотите попасть в Громовой утес?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Spell_Arcane_TeleportUnderCity:30:30:-18:0|t Подгород", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 7, "Вы уверены, что вы хотите попасть в Подгород?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Spell_Arcane_TeleportSilvermoon:30:30:-18:0|t Луносвет", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 8, "Вы уверены, что вы хотите попасть в Луносвет?", 0, false);
        }
        AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Spell_Arcane_TeleportShattrath:30:30:-18:0|t Шаттрат", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 9, "Вы уверены, что вы хотите попасть в Шаттрат?", 0, false);
        AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Spell_Arcane_TeleportDalaran:30:30:-18:0|t Даларан", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 10, "Вы уверены, что вы хотите попасть в Даларан?", 0, false);
        AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "|TInterface/ICONS/Achievement_Zone_EasternKingdoms_01:30:30:-18:0|t Восточные королевство", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 11);
        AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "|TInterface/ICONS/Achievement_Zone_Kalimdor_01:30:30:-18:0|t Калимдор", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 12);
        AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "|TInterface/ICONS/Achievement_Zone_Outland_01:30:30:-18:0|t Запределье", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 13);
        AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "|TInterface/ICONS/Achievement_Zone_Northrend_01:30:30:-18:0|t Нордскол", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 14);
        AddGossipItemFor(player, GOSSIP_ICON_BATTLE, "|TInterface/ICONS/Spell_Holy_ReviveChampion:30:30:-18:0|t Классические подземелья", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 15);
        AddGossipItemFor(player, GOSSIP_ICON_BATTLE, "|TInterface/ICONS/Spell_Holy_ChampionsBond:30:30:-18:0|t Подземелья BC", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 16);
        AddGossipItemFor(player, GOSSIP_ICON_BATTLE, "|TInterface/ICONS/Spell_Holy_SummonChampion:30:30:-18:0|t Подземелья WotLK", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 17);
        AddGossipItemFor(player, GOSSIP_ICON_BATTLE, "|TInterface/ICONS/Spell_Holy_EmpowerChampion:30:30:-18:0|t Рейдовые телепорты", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 18);
        AddGossipItemFor(player, GOSSIP_ICON_BATTLE, "|TInterface/ICONS/Achievement_Arena_2v2_7:30:30:-18:0|t Мировой босс", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 190);
		AddGossipItemFor(player, GOSSIP_ICON_BATTLE, "|TInterface\\icons\\Achievement_pvp_g_07:30:30:-18:0|t Дуэль Зона ", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 520);
        AddGossipItemFor(player, 0, "|TInterface\\icons\\Achievement_pvp_g_07:30:30:-18:0|t PvP зона: Хиджал " + GetZoneOnline(), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 509);

        SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, me->GetGUID());
        return true;
    }

    bool GossipSelect(Player* player, uint32 /*menu_id*/, uint32 gossipListId) override
    {
        uint32 uiAction = player->PlayerTalkClass->GetGossipOptionAction(gossipListId);
        player->PlayerTalkClass->ClearMenus();

        switch (uiAction)
        {
        case GOSSIP_ACTION_INFO_DEF + 1:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(0, -8831.966797f, 632.503906f, 94.210480f, 4.61f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 2:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(1, 9875.390625f, 2491.826660f, 1315.875854f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 3:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(0, -4902.013672f, -960.356812f, 501.459686f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 4:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(530, -3864.390381f, -11643.900391f, -137.643341f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 5:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(1, 1569.170410f, -4433.635742f, 6.718508f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 6:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(1, -1287.810181f, 137.872406f, 130.679062f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 7:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(0, 1561.338013f, 240.706558f, -43.102699f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 8:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(530, 9733.644531f, -7462.711426f, 13.575151f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 9:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(530, -1885.936768f, 5363.905762f, -12.427077f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 10:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(571, 5820.612305f, 543.924438f, 651.134399f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 190:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(0, -14284.236328f, 149.365234f, 8.483326f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }
		
		case GOSSIP_ACTION_INFO_DEF + 520:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(1, -1734.015137f, -280.815033f, -9.425349f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 11:
        {
            if (player->GetTeamId() == TEAM_HORDE)
            {
                AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Zone_EversongWoods:30:30:-18:0|t Леса Вечной Песни", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1100, "Вы уверены, что вы хотите попасть в Леса Вечной Песни?", 0, false);
                AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Zone_TirisfalGlades_01:30:30:-18:0|t Тирисфальские леса", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1101, "Вы уверены, что вы хотите попасть в Тирисфальские леса?", 0, false);
                AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Zone_Ghostlands:30:30:-18:0|t Призрачные земли", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1102, "Вы уверены, что вы хотите попасть в Призрачные земли?", 0, false);
                AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Zone_Silverpine_01:30:30:-18:0|t Серебряный бор", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1103, "Вы уверены, что вы хотите попасть в Серебряный бор?", 0, false);
            }
            if (player->GetTeamId() == TEAM_ALLIANCE)
            {
                AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Zone_ElwynnForest:30:30:-18:0|t Элвиннский лес", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1104, "Вы уверены, что вы хотите попасть в Элвиннский лес?", 0, false);
                AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Zone_DunMorogh:30:30:-18:0|t Дун Морог", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1105, "Вы уверены, что вы хотите попасть в Дун Морог?", 0, false);
                AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Zone_LochModan:30:30:-18:0|t Лок Модан", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1106, "Вы уверены, что вы хотите попасть в Лок Модан?", 0, false);
                AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Zone_WestFall_01:30:30:-18:0|t Западный Край", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1107, "Вы уверены, что вы хотите попасть в Западный Край?", 0, false);
                AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Zone_RedridgeMountains:30:30:-18:0|t Красногорье", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1108, "Вы уверены, что вы хотите попасть в Красногорье?", 0, false);
                AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Zone_Duskwood:30:30:-18:0|t Сумеречный лес", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1109, "Вы уверены, что вы хотите попасть в Сумеречный лес?", 0, false);
            }
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Zone_HillsbradFoothills:30:30:-18:0|t Предгорья Хилсбрада", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1110, "Вы уверены, что вы хотите попасть в Предгорья Хилсбрада?", 0, false);
            if (player->GetTeamId() == TEAM_ALLIANCE)
            {
                AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Zone_Wetlands_01:30:30:-18:0|t Болотина", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1111, "Вы уверены, что вы хотите попасть в Болотину?", 0, false);
            }
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Zone_AlteracMountains_01:30:30:-18:0|t Альтеракские горы", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1112, "Вы уверены, что вы хотите попасть в Альтеракские горы?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Zone_ArathiHighlands_01:30:30:-18:0|t Нагорье Арати", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1113, "Вы уверены, что вы хотите попасть в Нагорье Арати?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Zone_Stranglethorn_01:30:30:-18:0|t Тернистая долина", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1114, "Вы уверены, что вы хотите попасть в Тернистую долину?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Zone_Badlands_01:30:30:-18:0|t Бесплодные земли", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1115, "Вы уверены, что вы хотите попасть в Бесплодные земли?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Zone_SwampSorrows_01:30:30:-18:0|t Болото Печали", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1116, "Вы уверены, что вы хотите попасть в Болото Печали?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Zone_Hinterlands_01:30:30:-18:0|t Внутренние земли", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1117, "Вы уверены, что вы хотите попасть в Внутренние земли?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Zone_SearingGorge_01:30:30:-18:0|t Тлеющее ущелье", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1118, "Вы уверены, что вы хотите попасть в Тлеющее ущелье?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Zone_BlastedLands_01:30:30:-18:0|t Выжженные земли", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1119, "Вы уверены, что вы хотите попасть в Выжженные земли?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Zone_BurningSteppes_01:30:30:-18:0|t Пылающие степи", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1120, "Вы уверены, что вы хотите попасть в Пылающие степи?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Zone_WesternPlaguelands_01:30:30:-18:0|t Западные Чумные земли", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1121, "Вы уверены, что вы хотите попасть в Западные Чумные земли?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Zone_EasternPlaguelands:30:30:-18:0|t Восточные Чумные земли", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1122, "Вы уверены, что вы хотите попасть в Восточные Чумные земли?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Zone_IsleOfQuelDanas:30:30:-18:0|t Остров Кель'Данас", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1123, "Вы уверены, что вы хотите попасть на Остров Кель'Данас?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TALK, "|TInterface/ICONS/Ability_Spy:30:30:-18:0|t  Назад...", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 19);

            SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, me->GetGUID());
            return true;
        }

        case GOSSIP_ACTION_INFO_DEF + 12:
        {
            if (player->GetTeamId() == TEAM_ALLIANCE)
            {
                AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Zone_AzuremystIsle_01:30:30:-18:0|t Остров Лазурной Дымки", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1200, "Вы уверены, что вы хотите попасть на Остров Лазурной Дымки?", 0, false);
                AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Zone_Darnassus:30:30:-18:0|t Тельдрассил", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1201, "Вы уверены, что вы хотите попасть в Тельдрассил?", 0, false);
                AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Zone_BloodmystIsle_01:30:30:-18:0|t Остров Кровавой Дымки", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1202, "Вы уверены, что вы хотите попасть на Остров Кровавой Дымки?", 0, false);
                AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Zone_Darkshore_01:30:30:-18:0|t Темные берега", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1203, "Вы уверены, что вы хотите попасть в Темные берега?", 0, false);
            }
            if (player->GetTeamId() == TEAM_HORDE)
            {
                AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Zone_Durotar:30:30:-18:0|t Дуротар", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1204, "Вы уверены, что вы хотите попасть в Дуротар?", 0, false);
                AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Zone_Mulgore_01:30:30:-18:0|t Мулгор", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1205, "Вы уверены, что вы хотите попасть в Мулгор?", 0, false);
                AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Zone_Barrens_01:30:30:-18:0|t Степи", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1206, "Вы уверены, что вы хотите попасть в Степи?", 0, false);
            }
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Zone_Stonetalon_01:30:30:-18:0|t Когтистые горы", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1207, "Вы уверены, что вы хотите попасть в Когтистые горы?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Zone_Ashenvale_01:30:30:-18:0|t Ясеневый лес", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1208, "Вы уверены, что вы хотите попасть в Ясеневый лес?", 0, false);
            if (player->GetTeamId() == TEAM_HORDE)
            {
                AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Zone_ThousandNeedles_01:30:30:-18:0|t Тысяча игл", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1209, "Вы уверены, что вы хотите попасть в Тысячу игл?", 0, false);
            }
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Zone_Desolace:30:30:-18:0|t Пустоши", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1210, "Вы уверены, что вы хотите попасть в Пустоши?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Zone_DustwallowMarsh:30:30:-18:0|t Пылевые топи", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1211, "Вы уверены, что вы хотите попасть в Пылевые топи?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Zone_Feralas:30:30:-18:0|t Фералас", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1212, "Вы уверены, что вы хотите попасть в Фералас?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Zone_Tanaris_01:30:30:-18:0|t Танарис", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1213, "Вы уверены, что вы хотите попасть в Танарис?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Zone_Azshara_01:30:30:-18:0|t Азшара", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1214, "Вы уверены, что вы хотите попасть в Азшару?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Zone_Felwood:30:30:-18:0|t Оскверненный лес", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1215, "Вы уверены, что вы хотите попасть в Оскверненный лес?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Zone_UnGoroCrater_01:30:30:-18:0|t Кратер Ун'Горо", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1216, "Вы уверены, что вы хотите попасть в Кратер Ун'Горо?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Zone_Silithus_01:30:30:-18:0|t Силитус", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1217, "Вы уверены, что вы хотите попасть в Силитус?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Zone_Winterspring:30:30:-18:0|t Зимние Ключи", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1218, "Вы уверены, что вы хотите попасть в Зимние Ключи?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TALK, "|TInterface/ICONS/Ability_Spy:30:30:-18:0|t  Назад...", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 19);

            SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, me->GetGUID());
            return true;
        }

        case GOSSIP_ACTION_INFO_DEF + 13:
        {
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Zone_HellfirePeninsula_01:30:30:-18:0|t Полуостров Адского Пламени", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1300, "Вы уверены, что вы хотите попасть на Полуостров Адского Пламени?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Zone_Zangarmarsh:30:30:-18:0|t Зангартопь", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1301, "Вы уверены, что вы хотите попасть в Зангартопь?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Zone_Terrokar:30:30:-18:0|t Лес Тероккар", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1302, "Вы уверены, что вы хотите попасть в Лес Тероккар?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Zone_Nagrand_01:30:30:-18:0|t Награнд", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1303, "Вы уверены, что вы хотите попасть в Награнд?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Zone_BladesEdgeMtns_01:30:30:-18:0|t Острогорье", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1304, "Вы уверены, что вы хотите попасть в Острогорье?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Zone_Netherstorm_01:30:30:-18:0|t Пустоверть", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1305, "Вы уверены, что вы хотите попасть в Пустоверть?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Zone_Shadowmoon:30:30:-18:0|t Долина Призрачной Луны", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1306, "Вы уверены, что вы хотите попасть в Долину Призрачной Луны?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TALK, "|TInterface/ICONS/Ability_Spy:30:30:-18:0|t Назад...", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 19);

            SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, me->GetGUID());
            return true;
        }

        case GOSSIP_ACTION_INFO_DEF + 14:
        {
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Zone_BoreanTundra_01:30:30:-18:0|t Борейская тундра", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1400, "Вы уверены, что вы хотите попасть в Борейскау тундру?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Zone_HowlingFjord_01:30:30:-18:0|t Ревущий фьорд", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1401, "Вы уверены, что вы хотите попасть в Ревущий фьорд?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Zone_DragonBlight_01:30:30:-18:0|t Драконий погост", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1402, "Вы уверены, что вы хотите попасть в Драконий погост?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Zone_GrizzlyHills_01:30:30:-18:0|t Седые холмы", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1403, "Вы уверены, что вы хотите попасть в Седые холмы?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Zone_ZulDrak_03:30:30:-18:0|t Зул'Драк", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1404, "Вы уверены, что вы хотите попасть в Зул'Драк?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Zone_Sholazar_01:30:30:-18:0|t Низина Шолазар", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1405, "Вы уверены, что вы хотите попасть в Низину Шолазар?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Zone_CrystalSong_01:30:30:-18:0|t Лес Хрустальной Песни", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1406, "Вы уверены, что вы хотите попасть в Лес Хрустальной Песни?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Zone_StormPeaks_01:30:30:-18:0|t Грозовая Гряда", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1407, "Вы уверены, что вы хотите попасть в Грозовую Гряду?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Zone_IceCrown_01:30:30:-18:0|t Ледяная Корона", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1408, "Вы уверены, что вы хотите попасть в Ледяную Корону?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Spell_Frost_ChillingBlast:30:30:-18:0|t Озеро Ледяных Оков", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1409, "Вы уверены, что вы хотите попасть на Озеро Ледяных Оков?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TALK, "|TInterface/ICONS/Ability_Spy:30:30:-18:0|t Назад...", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 19);

            SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, me->GetGUID());
            return true;
        }

        case GOSSIP_ACTION_INFO_DEF + 15:
        {
            if (player->GetTeamId() == TEAM_HORDE)
            {
                AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Spell_Shadow_SummonFelGuard:30:30:-18:0|t Огненная пропасть", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1500, "Вы уверены, что вы хотите попасть в Огненную пропасть?", 0, false);
            }
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/INV_Misc_Bandana_03:30:30:-18:0|t Мертвые копи", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1501, "Вы уверены, что вы хотите попасть в Мертвые копи?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Boss_Mutanus_the_Devourer:30:30:-18:0|t Пещеры Стенаний", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1502, "Вы уверены, что вы хотите попасть в Пещеры Стенаний?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Spell_Arcane_PrismaticCloak:30:30:-18:0|t Крепость Темного Клыка", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1503, "Вы уверены, что вы хотите попасть в Крепость Темного Клыка?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Boss_Bazil_Akumai:30:30:-18:0|t Непроглядная Пучина", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1504, "Вы уверены, что вы хотите попасть в Непроглядную Пучину?", 0, false);
            if (player->GetTeamId() == TEAM_ALLIANCE)
            {
                AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Boss_Bazil_Thredd:30:30:-18:0|t Тюрьма Штормграда", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1505, "Вы уверены, что вы хотите попасть в Тюрьму Штормграда?", 0, false);
            }
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Boss_CharlgaRazorflank:30:30:-18:0|t Лабиринты Иглошкурых", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1506, "Вы уверены, что вы хотите попасть в Лабиринты Иглошкурых?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/inv_misc_tournaments_symbol_gnome:30:30:-18:0|t Гномреган", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1507, "Вы уверены, что вы хотите попасть в Гномреган?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/INV_Helmet_52:30:30:-18:0|t Монастырь Алого ордена", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1508, "Вы уверены, что вы хотите попасть в Монастырь Алого ордена?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Boss_Amnennar_the_Coldbringer:30:30:-18:0|t Курганы Иглошкурых", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1509, "Вы уверены, что вы хотите попасть в Курганы Иглошкурых?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Boss_Archaedas:30:30:-18:0|t Ульдаман", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1510, "Вы уверены, что вы хотите попасть в Ульдаман?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Boss_PrincessTheradras:30:30:-18:0|t Мародон", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1511, "Вы уверены, что вы хотите попасть в Мародон?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Boss_ChiefUkorzSandscalp:30:30:-18:0|t Зул'Фаррак", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1512, "Вы уверены, что вы хотите попасть в Зул'Фаррак?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Boss_ShadeOfEranikus:30:30:-18:0|t Затонувший храм", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1513, "Вы уверены, что вы хотите попасть в Затонувший храм?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Ability_Warrior_DecisiveStrike:30:30:-18:0|t Забытый Город", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1514, "Вы уверены, что вы хотите попасть в Забытый Город?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Spell_Holy_SenseUndead:30:30:-18:0|t Некроситет", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1515, "Вы уверены, что вы хотите попасть в Некроситет?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Boss_Overlord_Wyrmthalak:30:30:-18:0|t Нижняя часть Черной горы", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1516, "Вы уверены, что вы хотите попасть в Нижнюю часть Черной горы?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Boss_EmperorDagranThaurissan:30:30:-18:0|t Глубины Черной горы", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1517, "Вы уверены, что вы хотите попасть в Глубины Черной горы?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Spell_DeathKnight_ArmyOfTheDead:30:30:-18:0|t Стратхольм", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1518, "Вы уверены, что вы хотите попасть в Стратхольм?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TALK, "|TInterface/ICONS/Ability_Spy:30:30:-18:0|t Назад...", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 19);

            SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, me->GetGUID());
            return true;
        }

        case GOSSIP_ACTION_INFO_DEF + 16:
        {
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Boss_OmarTheUnscarred_01:30:30:-18:0|t Бастионы Адского Пламени", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1600, "Вы уверены, что вы хотите попасть в Бастион Адского Пламени?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Boss_KelidanTheBreaker:30:30:-18:0|t Кузня Крови", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1601, "Вы уверены, что вы хотите попасть в Кузню Крови?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Boss_Quagmirran:30:30:-18:0|t Узилище", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1602, "Вы уверены, что вы хотите попасть в Узилище?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Boss_theBlackStalker:30:30:-18:0|t Нижетопь", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1603, "Вы уверены, что вы хотите попасть в Нижетопь?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Boss_Nexus_Prince_Shaffar:30:30:-18:0|t Гробницы Маны", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1604, "Вы уверены, что вы хотите попасть в Гробницы Маны?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Boss_Exarch_Maladaar:30:30:-18:0|t Аукенайские гробницы", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1605, "Вы уверены, что вы хотите попасть в Аукенайские гробницы?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Boss_EpochHunter:30:30:-18:0|t Побег из Дарнхольда", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1606, "Вы уверены, что вы хотите попасть к Побегу из Дарнхольда?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Boss_TalonKingIkiss:30:30:-18:0|t Сетеккские залы", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1607, "Вы уверены, что вы хотите попасть в Сетеккские залы?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Boss_WarpSplinter:30:30:-18:0|t Ботаника", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1608, "Вы уверены, что вы хотите попасть в Ботанику?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Boss_PathaleonTheCalculator:30:30:-18:0|t Механар", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1609, "Вы уверены, что вы хотите попасть в Механар?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Boss_Warlord_Kalithresh:30:30:-18:0|t Паровое подземелье", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1610, "Вы уверены, что вы хотите попасть в Паровое подземелье?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Boss_KargathBladefist_01:30:30:-18:0|t Разрушенные залы", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1611, "Вы уверены, что вы хотите попасть в Разрушенные залы?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Boss_Murmur:30:30:-18:0|t Темный лабиринт", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1612, "Вы уверены, что вы хотите попасть в Темный лабиринт?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Boss_Kael'thasSunstrider_01:30:30:-18:0|t Терраса Магистров", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1613, "Вы уверены, что вы хотите попасть в Террасу Магистров?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Boss_Aeonus_01:30:30:-18:0|t Черные топи", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1614, "Вы уверены, что вы хотите попасть в Черные топи?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Boss_Harbinger_Skyriss:30:30:-18:0|t Аркатрац", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1615, "Вы уверены, что вы хотите попасть в Аркатрац?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TALK, "|TInterface/ICONS/Ability_Spy:30:30:-18:0|t Назад...", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 19);

            SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, me->GetGUID());
            return true;
        }

        case GOSSIP_ACTION_INFO_DEF + 17:
        {
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Dungeon_UtgardeKeep_Normal:30:30:-18:0|t Крепость Утгард", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1700, "Вы уверены, что вы хотите попасть в Крепость Утгард?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Dungeon_Nexus70_Normal:30:30:-18:0|t Нексус", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1701, "Вы уверены, что вы хотите попасть в Нексус?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Dungeon_AzjolUppercity_Normal:30:30:-18:0|t Азжол-Неруб", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1702, "Вы уверены, что вы хотите попасть в Азжол-Неруб?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Dungeon_AzjolLowercity_Normal:30:30:-18:0|t Ан'кахет: Старое Королевство", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1703, "Вы уверены, что вы хотите попасть в Ан'кахет: Старое Королевство?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Dungeon_Drak'Tharon_Normal:30:30:-18:0|t Крепость Драк'Тарон", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1704, "Вы уверены, что вы хотите попасть в Крепость Драк'Тарон?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Dungeon_TheVioletHold_Normal:30:30:-18:0|t Аметистовая крепость", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1705, "Вы уверены, что вы хотите попасть в Аметистовую крепость?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Dungeon_Gundrak_Normal:30:30:-18:0|t Гундрак", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1706, "Вы уверены, что вы хотите попасть в Гундрак?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Dungeon_Ulduar77_Normal:30:30:-18:0|t Чертоги камня", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1707, "Вы уверены, что вы хотите попасть в Чертоги камня?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Dungeon_UtgardePinnacle_Normal:30:30:-18:0|t Вершина Утгард", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1708, "Вы уверены, что вы хотите попасть в Вершину Утгард?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Dungeon_Nexus80_Normal:30:30:-18:0|t Окулус", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1709, "Вы уверены, что вы хотите попасть в Окулус?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Dungeon_CoTStratholme_Normal:30:30:-18:0|t Очищение Стратхольма", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1710, "Вы уверены, что вы хотите попасть в Очищение Стратхольма?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Dungeon_Ulduar80_Normal:30:30:-18:0|t Чертоги молний", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1711, "Вы уверены, что вы хотите попасть в Чертоги молний?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/INV_Spear_05:30:30:-18:0|t Испытание чемпиона", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1712, "Вы уверены, что вы хотите попасть в Испытание чемпиона?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/achievement_boss_lichking:30:30:-18:0|t Подземелья ЦЛК", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1713, "Вы уверены, что вы хотите попасть в Подземелья ЦЛК?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TALK, "|TInterface/ICONS/Ability_Spy:30:30:-18:0|t Назад...", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 19);

            SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, me->GetGUID());
            return true;
        }

        case GOSSIP_ACTION_INFO_DEF + 18:
        {
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Boss_Nefarion:30:30:-18:0|t Логово Крыла Тьмы", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1800, "Вы уверены, что вы хотите попасть в Логово Крыла Тьмы?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Boss_CThun:30:30:-18:0|t Храм Ан'Киража", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1801, "Вы уверены, что вы хотите попасть в Храм Ан'Киража?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Boss_OssirianTheUnscarred:30:30:-18:0|t Руины Ан'Киража", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1802, "Вы уверены, что вы хотите попасть в Руины Ан'Киража?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Boss_Ragnaros:30:30:-18:0|t Огненные Недра", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1803, "Вы уверены, что вы хотите попасть в Огненные Недра?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Boss_Hakkar:30:30:-18:0|t Зул'Гуруб", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1804, "Вы уверены, что вы хотите попасть в Зул'Гуруб?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Boss_PrinceMalchezaar_02:30:30:-18:0|t Каражан", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1805, "Вы уверены, что вы хотите попасть в Каражан?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Boss_GruulTheDragonkiller:30:30:-18:0|t Логово Груула", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1806, "Вы уверены, что вы хотите попасть в Логово Груула?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Boss_LadyVashj:30:30:-18:0|t Змеиное святилище", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1807, "Вы уверены, что вы хотите попасть в Змеиное святилище?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Character_Bloodelf_Male:30:30:-18:0|t Крепость Бурь", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1808, "Вы уверены, что вы хотите попасть в Крепость Бурь?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Boss_Kiljaedan:30:30:-18:0|t Плато Солнечного Колодца", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1809, "Вы уверены, что вы хотите попасть в Плато Солнечного Колодца?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Boss_Zuljin:30:30:-18:0|t Зул'Аман", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1810, "Вы уверены, что вы хотите попасть в Зул'Аман?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Boss_Magtheridon:30:30:-18:0|t Логово Магтеридона", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1811, "Вы уверены, что вы хотите попасть в Логово Магтеридона?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Boss_Archimonde:30:30:-18:0|t Битва за гору Хиджал", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1812, "Вы уверены, что вы хотите попасть к Битве за гору Хиджал?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Boss_Illidan:30:30:-18:0|t Черный храм", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1813, "Вы уверены, что вы хотите попасть в Черный храм?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/INV_Trinket_Naxxramas06:30:30:-18:0|t Наксрамас", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1814, "Вы уверены, что вы хотите попасть в Наксрамас?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Boss_Algalon_01:30:30:-18:0|t Ульдуар", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1815, "Вы уверены, что вы хотите попасть в Ульдуар?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Boss_Onyxia:30:30:-18:0|t Логово Ониксии", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1816, "Вы уверены, что вы хотите попасть в Логово Ониксии?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Spell_Nature_ElementalPrecision_2:30:30:-18:0|t Склеп Аркаона", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1817, "Вы уверены, что вы хотите попасть в Склеп Аркаона?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Reputation_ArgentChampion:30:30:-18:0|t Испытание крестоносца", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1818, "Вы уверены, что вы хотите попасть к Испытанию крестоносца?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Achievement_Dungeon_CoABlackDragonflight_25man:30:30:-18:0|t Обсидиановое святилище", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1819, "Вы уверены, что вы хотите попасть в Обсидиановое святилище?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/Spell_Shadow_Twilight:30:30:-18:0|t Рубиновое святилище", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1820, "Вы уверены, что вы хотите попасть в Рубиновое святилище?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TAXI, "|TInterface/ICONS/achievement_boss_lichking:30:30:-18:0|t Цитадель Ледяной Короны", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1821, "Вы уверены, что вы хотите попасть в Цитадель Ледяной Короны?", 0, false);
            AddGossipItemFor(player, GOSSIP_ICON_TALK, "|TInterface/ICONS/Ability_Spy:30:30:-18:0|t Назад...", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 19);

            SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, me->GetGUID());
            return true;
        }

        case GOSSIP_ACTION_INFO_DEF + 19:
        {
            GossipHello(player);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1100:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(530, 8710.403320f, -6671.434570f, 70.336700f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1101:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(0, 2258.842529f, 291.025208f, 34.113724f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1102:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(530, 7575.331543f, -6817.992188f, 86.735321f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1103:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(0, 496.497559f, 1559.458984f, 128.082184f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1104:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(0, -9450.225586f, 62.199814f, 55.922474f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1105:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(0, -5603.308594f, -484.044220f, 397.015869f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1106:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(0, -5351.827148f, -2955.287598f, 323.788330f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1107:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(0, -10517.923828f, 1070.166748f, 54.737370f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1108:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(0, -9255.051758f, -2157.081055f, 64.019783f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1109:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(0, -10559.156250f, -1188.063110f, 28.073257f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1110:
        {
            CloseGossipMenuFor(player);
            if (player->GetTeamId() == TEAM_ALLIANCE)
            {
                player->TeleportTo(0, -854.630127f, -530.738220f, 10.010766f, 0.f);
            }
            else
            {
                player->TeleportTo(0, -33.078804f, -926.860901f, 54.459236f, 0.f);
            }
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1111:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(0, -3657.482666f, -839.019409f, 9.899013f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1112:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(0, 444.232422f, -624.817627f, 166.801392f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1113:
        {
            CloseGossipMenuFor(player);
            if (player->GetTeamId() == TEAM_ALLIANCE)
            {
                player->TeleportTo(0, -1211.127686f, -2531.696289f, 21.811106f, 0.f);
            }
            else
            {
                player->TeleportTo(0, -828.577820f, -3516.470947f, 72.289757f, 0.f);
            }
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1114:
        {
            CloseGossipMenuFor(player);
            if (player->GetTeamId() == TEAM_ALLIANCE)
            {
                player->TeleportTo(0, -11317.606445f, -199.714859f, 75.996849f, 0.f);
            }
            else
            {
                player->TeleportTo(0, -12388.219727f, 180.921005f, 2.343284f, 0.f);
            }
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1115:
        {
            CloseGossipMenuFor(player);
            if (player->GetTeamId() == TEAM_ALLIANCE)
            {
                player->TeleportTo(0, -6775.137695f, -3140.591797f, 240.538483f, 0.f);
            }
            else
            {
                player->TeleportTo(0, -6646.120117f, -2178.156982f, 246.758911f, 0.f);
            }
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1116:
        {
            CloseGossipMenuFor(player);
            if (player->GetTeamId() == TEAM_ALLIANCE)
            {
                player->TeleportTo(0, -10387.115234f, -2699.825439f, 21.677799f, 0.f);
            }
            else
            {
                player->TeleportTo(0, -10429.958984f, -3254.421387f, 20.178413f, 0.f);
            }
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1117:
        {
            CloseGossipMenuFor(player);
            if (player->GetTeamId() == TEAM_ALLIANCE)
            {
                player->TeleportTo(0, 289.343018f, -2004.510254f, 194.126434f, 0.f);
            }
            else
            {
                player->TeleportTo(0, -563.798279f, -4570.668457f, 9.425814f, 0.f);
            }
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1118:
        {
            CloseGossipMenuFor(player);
            if (player->GetTeamId() == TEAM_ALLIANCE)
            {
                player->TeleportTo(0, -6551.519531f, -1169.589722f, 309.262848f, 0.f);
            }
            else
            {
                player->TeleportTo(0, -6554.264160f, -1102.049072f, 309.402161f, 0.f);
            }
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1119:
        {
            CloseGossipMenuFor(player);
            if (player->GetTeamId() == TEAM_ALLIANCE)
            {
                player->TeleportTo(0, -11000.829102f, -3381.570068f, 62.250340f, 0.f);
            }
            else
            {
                player->TeleportTo(0, -11183.447266f, -3019.626953f, 7.314721f, 0.f);
            }
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1120:
        {
            CloseGossipMenuFor(player);
            if (player->GetTeamId() == TEAM_ALLIANCE)
            {
                player->TeleportTo(0, -8378.108398f, -2756.785645f, 187.429642f, 0.f);
            }
            else
            {
                player->TeleportTo(0, -7503.033203f, -2185.369629f, 165.745621f, 0.f);
            }
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1121:
        {
            CloseGossipMenuFor(player);
            if (player->GetTeamId() == TEAM_ALLIANCE)
            {
                player->TeleportTo(0, 951.413696f, -1425.585327f, 64.955223f, 0.f);
            }
            else
            {
                player->TeleportTo(0, 1718.489380f, -808.889221f, 57.890518f, 0.f);
            }
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1122:
        {
            CloseGossipMenuFor(player);
            if (player->GetTeamId() == TEAM_ALLIANCE)
            {
                player->TeleportTo(0, 2271.206543f, -5340.872559f, 87.121506f, 0.f);
            }
            else
            {
                player->TeleportTo(0, 2316.267578f, -5296.465332f, 81.996613f, 0.f);
            }
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1123:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(530, 12804.658203f, -6908.806641f, 41.115768f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1200:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(530, -4164.518555f, -12502.922852f, 44.362820f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1201:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(1, 9867.769531f, 947.978821f, 1306.964355f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1202:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(530, -2008.464722f, -11847.952148f, 48.134071f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1203:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(1, 6411.376465f, 522.530762f, 8.666072f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1204:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(1, 283.766785f, -4744.416016f, 9.550735f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1205:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(1, -2315.844238f, -366.869598f, -9.402488f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1206:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(1, -455.785645f, -2653.802490f, 95.656563f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1207:
        {
            CloseGossipMenuFor(player);
            if (player->GetTeamId() == TEAM_ALLIANCE)
            {
                player->TeleportTo(1, 2679.994141f, 1464.800171f, 233.019638f, 0.f);
            }
            else
            {
                player->TeleportTo(1, 951.244080f, 948.097717f, 104.635170f, 0.f);
            }
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1208:
        {
            CloseGossipMenuFor(player);
            if (player->GetTeamId() == TEAM_ALLIANCE)
            {
                player->TeleportTo(1, 2754.927979f, -333.423981f, 107.192116f, 0.f);
            }
            else
            {
                player->TeleportTo(1, 2308.828369f, -2540.538818f, 99.956779f, 0.f);
            }
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1209:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(1, -5431.372559f, -2449.937500f, 89.282677f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1210:
        {
            CloseGossipMenuFor(player);
            if (player->GetTeamId() == TEAM_ALLIANCE)
            {
                player->TeleportTo(1, 120.029694f, 1234.651855f, 162.644104f, 0.f);
            }
            else
            {
                player->TeleportTo(1, -1661.616211f, 3095.756104f, 30.497908f, 0.f);
            }
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1211:
        {
            CloseGossipMenuFor(player);
            if (player->GetTeamId() == TEAM_ALLIANCE)
            {
                player->TeleportTo(1, -3683.843506f, -2526.836426f, 65.885284f, 0.f);
            }
            else
            {
                player->TeleportTo(1, -3118.754883f, -2860.226074f, 34.613083f, 0.f);
            }
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1212:
        {
            CloseGossipMenuFor(player);
            if (player->GetTeamId() == TEAM_ALLIANCE)
            {
                player->TeleportTo(1, -4374.256836f, 3287.875244f, 13.564287f, 0.f);
            }
            else
            {
                player->TeleportTo(1, -4400.354492f, 258.061066f, 25.369953f, 0.f);
            }
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1213:
        {
            CloseGossipMenuFor(player);
            if (player->GetTeamId() == TEAM_ALLIANCE)
            {
                player->TeleportTo(1, -7220.702148f, -3732.372314f, 8.401043f, 0.f);
            }
            else
            {
                player->TeleportTo(1, -7054.277344f, -3781.617188f, 9.971897f, 0.f);
            }
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1214:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(1, 3327.874512f, -4585.886230f, 91.925835f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1215:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(1, 3986.736084f, -1292.744507f, 250.144958f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1216:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(1, -7863.241211f, -2101.181396f, -265.453766f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1217:
        {
            CloseGossipMenuFor(player);
            if (player->GetTeamId() == TEAM_ALLIANCE)
            {
                player->TeleportTo(1, -6762.262207f, 770.657410f, 88.915543f, 0.f);
            }
            else
            {
                player->TeleportTo(1, -6805.900391f, 837.114319f, 49.537369f, 0.f);
            }
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1218:
        {
            CloseGossipMenuFor(player);
            if (player->GetTeamId() == TEAM_ALLIANCE)
            {
                player->TeleportTo(1, 6798.555664f, -4741.371582f, 701.498474f, 0.f);
            }
            else
            {
                player->TeleportTo(1, 6807.989258f, -4609.876465f, 710.506165f, 0.f);
            }
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1300:
        {
            CloseGossipMenuFor(player);
            if (player->GetTeamId() == TEAM_ALLIANCE)
            {
                player->TeleportTo(530, -704.257935f, 2681.303223f, 93.821556f, 0.f);
            }
            else
            {
                player->TeleportTo(530, 141.451675f, 2673.095215f, 85.670227f, 0.f);
            }
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1301:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(530, -226.890259f, 5436.788086f, 21.591129f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1302:
        {
            CloseGossipMenuFor(player);
            if (player->GetTeamId() == TEAM_ALLIANCE)
            {
                player->TeleportTo(530, -2897.453369f, 3978.660400f, 0.483634f, 0.f);
            }
            else
            {
                player->TeleportTo(530, -2590.101074f, 4383.216797f, 29.111372f, 0.f);
            }
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1303:
        {
            CloseGossipMenuFor(player);
            if (player->GetTeamId() == TEAM_ALLIANCE)
            {
                player->TeleportTo(530, -2550.599365f, 7273.160645f, 14.766545f, 0.f);
            }
            else
            {
                player->TeleportTo(530, -1321.986572f, 7213.049316f, 33.438786f, 0.f);
            }
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1304:
        {
            CloseGossipMenuFor(player);
            if (player->GetTeamId() == TEAM_ALLIANCE)
            {
                player->TeleportTo(530, 2060.358398f, 6850.749512f, 172.537704f, 0.f);
            }
            else
            {
                player->TeleportTo(530, 2383.457031f, 6041.785156f, 140.037018f, 0.f);
            }
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1305:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(530, 3056.779541f, 3668.813477f, 142.396835f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1306:
        {
            CloseGossipMenuFor(player);
            if (player->GetTeamId() == TEAM_ALLIANCE)
            {
                player->TeleportTo(530, -4018.491699f, 2212.500244f, 109.935982f, 0.f);
            }
            else
            {
                player->TeleportTo(530, -2981.978027f, 2598.322021f, 78.510063f, 0.f);
            }
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1400:
        {
            CloseGossipMenuFor(player);
            if (player->GetTeamId() == TEAM_ALLIANCE)
            {
                player->TeleportTo(571, 2223.859131f, 5309.309570f, 10.572764f, 0.f);
            }
            else
            {
                player->TeleportTo(571, 2747.021484f, 6078.503418f, 77.670151f, 0.f);
            }
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1401:
        {
            CloseGossipMenuFor(player);
            if (player->GetTeamId() == TEAM_ALLIANCE)
            {
                player->TeleportTo(571, 2471.282959f, -5033.352539f, 283.161316f, 0.f);
            }
            else
            {
                player->TeleportTo(571, 2659.295410f, -4382.727539f, 282.249237f, 0.f);
            }
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1402:
        {
            CloseGossipMenuFor(player);
            if (player->GetTeamId() == TEAM_ALLIANCE)
            {
                player->TeleportTo(571, 3686.159424f, -721.455200f, 212.542450f, 0.f);
            }
            else
            {
                player->TeleportTo(571, 3837.189453f, 1543.396851f, 89.724007f, 0.f);
            }
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1403:
        {
            CloseGossipMenuFor(player);
            if (player->GetTeamId() == TEAM_ALLIANCE)
            {
                player->TeleportTo(571, 3414.050293f, -2826.611084f, 201.127228f, 0.f);
            }
            else
            {
                player->TeleportTo(571, 3248.657227f, -2251.771240f, 114.636642f, 0.f);
            }
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1404:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(571, 5449.020508f, -2579.456055f, 303.954163f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1405:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(571, 5568.410645f, 5750.153320f, -75.386864f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1406:
        {
            CloseGossipMenuFor(player);
            if (player->GetTeamId() == TEAM_ALLIANCE)
            {
                player->TeleportTo(571, 5055.169922f, -525.626526f, 219.349594f, 0.f);
            }
            else
            {
                player->TeleportTo(571, 5600.377441f, -690.192993f, 206.627075f, 0.f);
            }
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1407:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(571, 6122.825684f, -1061.005249f, 402.581390f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1408:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(571, 6236.627930f, -19.826708f, 410.097137f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1409:
        {
            CloseGossipMenuFor(player);
            if (player->GetTeamId() == TEAM_ALLIANCE)
            {
                player->TeleportTo(571, 5095.779785f, 2179.112061f, 365.600159f, 0.f);
            }
            else
            {
                player->TeleportTo(571, 5026.125977f, 3674.520020f, 362.731018f, 0.f);
            }
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1500:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(1, 1811.334595f, -4410.361816f, -18.293844f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1501:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(0, -11208.534180f, 1673.485352f, 24.634235f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1502:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(1, -740.299744f, -2216.026855f, 16.192665f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1503:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(0, -233.499649f, 1566.007690f, 76.892044f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1504:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(1, 4249.207520f, 741.622498f, -25.309313f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1505:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(0, -8772.289063f, 839.907959f, 91.127991f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1506:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(1, -4467.586426f, -1670.864014f, 81.873016f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1507:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(0, -5163.063477f, 925.362244f, 257.180573f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1508:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(0, 2872.487305f, -762.954163f, 160.332031f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1509:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(1, -4658.110840f, -2525.904297f, 81.426933f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1510:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(0, -6071.449707f, -2954.793457f, 209.781067f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1511:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(1, -1187.540894f, 2886.911865f, 86.012833f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1512:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(1, -6800.640137f, -2890.818604f, 8.882755f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1513:
        {
            CloseGossipMenuFor(player);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1514:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(1, -4085.258057f, 1136.126831f, 161.213196f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1515:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(0, 1273.802368f, -2574.718018f, 92.665535f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1516:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(0, -7526.054688f, -1227.449097f, 285.732269f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1517:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(0, -7180.155762f, -919.251587f, 165.490509f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1518:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(0, 3391.480225f, -3350.638916f, 142.247513f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1600:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(530, -362.516052f, 3074.392822f, -15.073921f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1601:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(530, -300.036438f, 3159.384277f, 31.679262f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1602:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(530, 727.309143f, 7010.300293f, -72.245384f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1603:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(530, 780.506042f, 6760.712402f, -72.538437f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1604:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(530, -3084.625000f, 4942.677246f, -101.047295f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1605:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(530, -3361.904297f, 5219.282715f, -101.049004f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1606:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(1, -8352.790039f, -4059.967529f, -208.107239f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1607:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(530, -3361.873535f, 4666.265625f, -101.047676f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1608:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(530, 3408.064941f, 1487.714844f, 182.837753f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1609:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(530, 2869.277344f, 1552.126221f, 252.159531f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1610:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(530, 817.324829f, 6936.354492f, -80.600777f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1611:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(530, -308.734558f, 3069.407715f, -3.357915f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1612:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(530, -3547.817627f, 4943.270996f, -101.395714f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1613:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(530, 12884.964844f, -7333.929199f, 65.487953f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1614:
        {
            CloseGossipMenuFor(player);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1615:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(530, 3309.076416f, 1339.236572f, 505.559967f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1700:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(571, 1219.957764f, -4864.713867f, 41.248600f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1701:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(571, 3892.663086f, 6985.346680f, 69.488052f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1702:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(571, 3680.591797f, 2165.664063f, 35.847965f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1703:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(571, 3643.248535f, 2036.293335f, 1.787633f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1704:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(571, 4774.270996f, -2030.222534f, 229.380524f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1705:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(571, 5693.236328f, 502.699402f, 652.671387f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1706:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(571, 6717.083496f, -4646.453613f, 449.999542f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1707:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(571, 8922.200195f, -981.891846f, 1039.307983f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1708:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(571, 1245.750488f, -4856.479492f, 216.830505f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1709:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(571, 3878.191895f, 6971.602539f, 106.320099f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1710:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(1, -8755.504883f, -4448.065918f, -199.521881f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1711:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(571, 9179.422852f, -1383.386353f, 1107.951172f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1712:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(571, 8576.164063f, 792.177002f, 558.234985f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1713:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(571, 5634.636719f, 2044.328369f, 798.270691f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1800:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(229, 165.905334f, -474.620667f, 116.839996f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1801:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(1, -8249.565430f, 1984.193359f, 129.071960f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1802:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(1, -8412.162109f, 1499.543335f, 28.727203f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1803:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(230, 1123.922974f, -453.699615f, -101.326218f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1804:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(0, -11916.026367f, -1204.653198f, 92.287567f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1805:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(0, -11118.515625f, -2010.693115f, 47.079922f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1806:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(530, 3535.287598f, 5098.154785f, 3.735290f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1807:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(530, 820.460449f, 6866.308594f, -66.678024f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1808:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(530, 3087.782227f, 1377.703003f, 184.885452f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1809:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(530, 12564.985352f, -6774.775879f, 15.090400f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1810:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(530, 6850.897461f, -7977.910645f, 182.492538f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1811:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(530, -318.783325f, 3095.481689f, -116.427521f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1812:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(1, -8176.613770f, -4180.662598f, -167.299789f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1813:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(530, -3639.328125f, 315.077515f, 35.283680f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1814:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(571, 3657.932373f, -1270.455200f, 243.547836f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1815:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(571, 9333.841797f, -1114.736938f, 1245.125122f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1816:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(1, -4713.428223f, -3731.389160f, 54.151909f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1817:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(571, 5464.988281f, 2841.296143f, 418.675262f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1818:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(571, 8515.751953f, 732.981201f, 558.550293f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1819:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(571, 3457.352539f, 262.626343f, -113.933411f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1820:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(571, 3599.521484f, 198.589981f, -113.772163f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }

        case GOSSIP_ACTION_INFO_DEF + 1821:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(571, 5788.104980f, 2070.332520f, 636.063782f, 0.f);
            player->CastSpell(player, 35517, true);
            break;
        }
        case GOSSIP_ACTION_INFO_DEF + 509:
            if (player->GetZoneId() == 616)
            {
                ChatHandler(player->GetSession()).PSendSysMessage("|cff006699Вы уже в пвп зоне !");
                player->PlayerTalkClass->SendCloseGossip();
            }
            else
            {
                teleportToRandomLocation(player);
                player->PlayerTalkClass->SendCloseGossip();
            }
            break;

        default: break;
        }
        return true;
    }
};

CreatureAI* GetAI(Creature* creature) const override
{
    return new npc_TeleporterAI(creature);
}
};


void AddSC_npc_Teleporter()
{
    new npc_Teleporter();
}