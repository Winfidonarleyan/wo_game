#include "ScriptPCH.h"
#include "ScriptMgr.h"
#include "ScriptedGossip.h"
#include "DBCStores.h"

#define MSG_GOSSIP_TEXT_GETTING_STARTED "Добро пожаловать на сервер WoW Game ! Стоймость любого звания 300 Апа"

#define MSG_ERR_ARENA_POINT "Нужно больше Апа!"
#define MSG_ERR_TITLE "У тебя уже есть такое звание!"
#define MSG_ERR_INCOMBAT "Прекрати драться!"

#define MSG_GOSSIP_TEXT_NEXT_2 "|TInterface\\icons\\Achievement_pvp_g_01:25|t [Вторая страница] ->"
#define MSG_GOSSIP_TEXT_NEXT_3 "|TInterface\\icons\\Achievement_pvp_g_01:25|t [Третья страница] ->"
#define MSG_GOSSIP_TEXT_NEXT_4 "|TInterface\\icons\\Achievement_pvp_g_01:25|t [Четвертая страница] ->"
#define MSG_GOSSIP_TEXT_NEXT_5 "|TInterface\\icons\\Achievement_pvp_g_01:25|t [Пятая страница] ->"

#define MSG_GOSSIP_TEXT_BACK_4 "|TInterface\\icons\\Achievement_pvp_h_01:25|t <- [Четвертая страница]"
#define MSG_GOSSIP_TEXT_BACK_3 "|TInterface\\icons\\Achievement_pvp_h_01:25|t <- [Третья страница] "
#define MSG_GOSSIP_TEXT_BACK_2 "|TInterface\\icons\\Achievement_pvp_h_01:25|t <- [Вторая страница] "
#define MSG_GOSSIP_TEXT_BACK_1 "|TInterface\\icons\\Achievement_pvp_h_01:25|t <- [Первая страница] "


#define MSG_GOSSIP_TEXT_144 "|TInterface\\icons\\Inv_misc_note_02:25|t Адмирал Кровавого Паруса"
#define MSG_GOSSIP_TEXT_135 "|TInterface\\icons\\Inv_misc_note_02:25|t Безумно влюбленный"
#define MSG_GOSSIP_TEXT_141 "|TInterface\\icons\\Inv_misc_note_02:25|t Бессмертный"
#define MSG_GOSSIP_TEXT_138 "|TInterface\\icons\\Inv_misc_note_02:25|t Благодетель"
#define MSG_GOSSIP_TEXT_170 "|TInterface\\icons\\Inv_misc_note_02:25|t Великий крестоносец"
#define MSG_GOSSIP_TEXT_28 "|TInterface\\icons\\Inv_misc_note_02:25|t Верховный вождь"
#define MSG_GOSSIP_TEXT_48 "|TInterface\\icons\\Inv_misc_note_02:25|t Вершитель правосудия"
#define MSG_GOSSIP_TEXT_134 "|TInterface\\icons\\Inv_misc_note_02:25|t Весельчак"
#define MSG_GOSSIP_TEXT_72 "|TInterface\\icons\\Inv_misc_note_02:25|t Военачальник"
#define MSG_GOSSIP_TEXT_113 "|TInterface\\icons\\Inv_misc_note_02:25|t Гномреганский"
#define MSG_GOSSIP_TEXT_147 "|TInterface\\icons\\Inv_misc_note_02:25|t Дарнасский"
#define MSG_GOSSIP_TEXT_143 "|TInterface\\icons\\Inv_misc_note_02:25|t Дженкинс"
#define MSG_GOSSIP_TEXT_79 "|TInterface\\icons\\Inv_misc_note_02:25|t Дипломат"
#define MSG_GOSSIP_TEXT_64 "|TInterface\\icons\\Inv_misc_note_02:25|t Длань А'дала"
#define MSG_GOSSIP_TEXT_43 "|TInterface\\icons\\Inv_misc_note_02:25|t Дуэлянт"
#define MSG_GOSSIP_TEXT_47 "|TInterface\\icons\\Inv_misc_note_02:25|t Завоеватель"
#define MSG_GOSSIP_TEXT_122 "|TInterface\\icons\\Inv_misc_note_02:25|t Завоеватель Наксрамаса"
#define MSG_GOSSIP_TEXT_81 "|TInterface\\icons\\Inv_misc_note_02:25|t Искатель"
#define MSG_GOSSIP_TEXT_78 "|TInterface\\icons\\Inv_misc_note_02:25|t Исследователь"
#define MSG_GOSSIP_TEXT_156 "|TInterface\\icons\\Inv_misc_note_02:25|t Крестоносец"
#define MSG_GOSSIP_TEXT_83 "|TInterface\\icons\\Inv_misc_note_02:25|t Морской дьявол"
#define MSG_GOSSIP_TEXT_142 "|TInterface\\icons\\Inv_misc_note_02:25|t Неумирающий"
#define MSG_GOSSIP_TEXT_150 "|TInterface\\icons\\Inv_misc_note_02:25|t Оргриммарский"
#define MSG_GOSSIP_TEXT_46 "|TInterface\\icons\\Inv_misc_note_02:25|t Повелитель Скарабеев"
#define MSG_GOSSIP_TEXT_139 "|TInterface\\icons\\Inv_misc_note_02:25|t Покоритель Обсидианового святилища"
#define MSG_GOSSIP_TEXT_77 "|TInterface\\icons\\Inv_misc_note_02:25|t Превозносимый"
#define MSG_GOSSIP_TEXT_74 "|TInterface\\icons\\Inv_misc_note_02:25|t Премудрый"
#define MSG_GOSSIP_TEXT_45 "|TInterface\\icons\\Inv_misc_note_02:25|t Претендент"
#define MSG_GOSSIP_TEXT_151 "|TInterface\\icons\\Inv_misc_note_02:25|t Сен'джинский"
#define MSG_GOSSIP_TEXT_171 "|TInterface\\icons\\Inv_misc_note_02:25|t Серебряный заступник"
#define MSG_GOSSIP_TEXT_131 "|TInterface\\icons\\Inv_misc_note_02:25|t Серебряный защитник"
#define MSG_GOSSIP_TEXT_173 "|TInterface\\icons\\Inv_misc_note_02:25|t Сияние Рассвета"
#define MSG_GOSSIP_TEXT_75 "|TInterface\\icons\\Inv_misc_note_02:25|t Страж огня"
#define MSG_GOSSIP_TEXT_140 "|TInterface\\icons\\Inv_misc_note_02:25|t Сумеречный"
#define MSG_GOSSIP_TEXT_172 "|TInterface\\icons\\Inv_misc_note_02:25|t Терпеливый"
#define MSG_GOSSIP_TEXT_124 "|TInterface\\icons\\Inv_misc_note_02:25|t Тыквер"
#define MSG_GOSSIP_TEXT_133 "|TInterface\\icons\\Inv_misc_note_02:25|t Хмелевар"
#define MSG_GOSSIP_TEXT_125 "|TInterface\\icons\\Inv_misc_note_02:25|t Хранитель мудрости"
#define MSG_GOSSIP_TEXT_76 "|TInterface\\icons\\Inv_misc_note_02:25|t Хранитель огня"
#define MSG_GOSSIP_TEXT_145 "|TInterface\\icons\\Inv_misc_note_02:25|t Чокнутый"
#define MSG_GOSSIP_TEXT_155 "|TInterface\\icons\\Inv_misc_note_02:25|t Чудесный"
#define MSG_GOSSIP_TEXT_84 "|TInterface\\icons\\Inv_misc_note_02:25|t Шеф-повар"

#define CONST_ARENA_POINT 300

class npc_title : public CreatureScript
{
public:
    npc_title() : CreatureScript("npc_title") { }

    struct npc_titleAI : public ScriptedAI
    {
        npc_titleAI(Creature* creature) : ScriptedAI(creature) { }

        bool AddTitle(Player* player, Creature* me, CharTitlesEntry const* titleInfo)
        {
            if (player->HasTitle(titleInfo)) {
                me->Whisper(MSG_ERR_TITLE, LANG_UNIVERSAL, player);
                CloseGossipMenuFor(player);
                return false;
            }

            if (player->GetArenaPoints() < CONST_ARENA_POINT) {
                me->Whisper(MSG_ERR_ARENA_POINT, LANG_UNIVERSAL, player);
                CloseGossipMenuFor(player);

            }
            else {

                player->SetTitle(titleInfo);
                player->ModifyArenaPoints(-CONST_ARENA_POINT);
                CloseGossipMenuFor(player);
            }
            return true;
        }

        bool GossipHello(Player* player) override
        {
            me->Whisper(MSG_GOSSIP_TEXT_GETTING_STARTED, LANG_UNIVERSAL, player);
            player->PlayerTalkClass->ClearMenus();
            AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_144, GOSSIP_SENDER_MAIN, 2);
            AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_135, GOSSIP_SENDER_MAIN, 4);
            AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_141, GOSSIP_SENDER_MAIN, 5);
            AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_138, GOSSIP_SENDER_MAIN, 6);
            AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_170, GOSSIP_SENDER_MAIN, 7);
            AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_28, GOSSIP_SENDER_MAIN, 8);
            AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_48, GOSSIP_SENDER_MAIN, 9);
            AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_134, GOSSIP_SENDER_MAIN, 10);
            AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_72, GOSSIP_SENDER_MAIN, 11);
            AddGossipItemFor(player, GOSSIP_ICON_TALK, MSG_GOSSIP_TEXT_NEXT_2, GOSSIP_SENDER_MAIN, 12);
            SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, me->GetGUID());
            return true;

        }

        bool GossipSelect(Player* player, uint32 /*menu_id*/, uint32 gossipListId) override
        {
            uint32 const sender = player->PlayerTalkClass->GetGossipOptionSender(gossipListId);
            uint32 const action = player->PlayerTalkClass->GetGossipOptionAction(gossipListId);
            if (!player->getAttackers().empty())
            {
                me->Whisper(MSG_ERR_INCOMBAT, LANG_UNIVERSAL, player);
                CloseGossipMenuFor(player);
                return false;
            }

            switch (action)
            {
            case 2:
            {
                CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(144);
                AddTitle(player, me, titleInfo);
            }
            break;
            case 4:
            {
                CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(135);
                AddTitle(player, me, titleInfo);
            }
            break;
            case 5:
            {
                CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(141);
                AddTitle(player, me, titleInfo);
            }
            break;
            case 6:
            {
                CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(138);
                AddTitle(player, me, titleInfo);
            }
            break;
            case 7:
            {
                CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(170);
                AddTitle(player, me, titleInfo);
            }
            break;
            case 8:
            {
                CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(28);
                AddTitle(player, me, titleInfo);
            }
            break;
            case 9:
            {
                CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(48);
                AddTitle(player, me, titleInfo);
            }
            break;
            case 10:
            {
                CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(134);
                AddTitle(player, me, titleInfo);
            }
            break;
            case 11:
            {
                CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(72);
                AddTitle(player, me, titleInfo);
            }
            break;
            case 12:
                player->PlayerTalkClass->ClearMenus();
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_113, GOSSIP_SENDER_MAIN, 14);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_147, GOSSIP_SENDER_MAIN, 15);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_143, GOSSIP_SENDER_MAIN, 16);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_79, GOSSIP_SENDER_MAIN, 17);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_64, GOSSIP_SENDER_MAIN, 18);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_43, GOSSIP_SENDER_MAIN, 19);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_47, GOSSIP_SENDER_MAIN, 21);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_122, GOSSIP_SENDER_MAIN, 22);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, MSG_GOSSIP_TEXT_NEXT_3, GOSSIP_SENDER_MAIN, 23);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, me->GetGUID());
                break;
            case 14:
            {
                CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(113);
                AddTitle(player, me, titleInfo);
            }
            break;
            case 15:
            {
                CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(147);
                AddTitle(player, me, titleInfo);
            }
            break;
            case 16:
            {
                CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(143);
                AddTitle(player, me, titleInfo);
            }
            break;
            case 17:
            {
                CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(79);
                AddTitle(player, me, titleInfo);
            }
            break;
            case 18:
            {
                CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(64);
                AddTitle(player, me, titleInfo);
            }
            break;
            case 19:
            {
                CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(43);
                AddTitle(player, me, titleInfo);
            }
            break;
            case 21:
            {
                CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(47);
                AddTitle(player, me, titleInfo);
            }
            break;
            case 22:
            {
                CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(122);
                AddTitle(player, me, titleInfo);
            }
            break;
            case 23:
                player->PlayerTalkClass->ClearMenus();
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_81, GOSSIP_SENDER_MAIN, 24);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_78, GOSSIP_SENDER_MAIN, 25);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_156, GOSSIP_SENDER_MAIN, 26);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_83, GOSSIP_SENDER_MAIN, 27);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_142, GOSSIP_SENDER_MAIN, 29);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_150, GOSSIP_SENDER_MAIN, 31);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_46, GOSSIP_SENDER_MAIN, 32);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, MSG_GOSSIP_TEXT_NEXT_4, GOSSIP_SENDER_MAIN, 34);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, me->GetGUID());
                break;
            case 24:
            {
                CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(81);
                AddTitle(player, me, titleInfo);
            }
            break;
            case 25:
            {
                CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(78);
                AddTitle(player, me, titleInfo);
            }
            break;
            case 26:
            {
                CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(156);
                AddTitle(player, me, titleInfo);
            }
            break;
            case 27:
            {
                CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(83);
                AddTitle(player, me, titleInfo);
            }
            break;
            case 29:
            {
                CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(142);
                AddTitle(player, me, titleInfo);
            }
            break;
            case 31:
            {
                CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(150);
                AddTitle(player, me, titleInfo);
            }
            break;
            case 32:
            {
                CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(46);
                AddTitle(player, me, titleInfo);
            }
            break;
            case 33:
            {
                CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(278);
                AddTitle(player, me, titleInfo);
            }
            break;
            case 34:
                player->PlayerTalkClass->ClearMenus();
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_139, GOSSIP_SENDER_MAIN, 35);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_77, GOSSIP_SENDER_MAIN, 36);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_74, GOSSIP_SENDER_MAIN, 37);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_45, GOSSIP_SENDER_MAIN, 38);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_151, GOSSIP_SENDER_MAIN, 40);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_171, GOSSIP_SENDER_MAIN, 41);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_131, GOSSIP_SENDER_MAIN, 42);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_173, GOSSIP_SENDER_MAIN, 43);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, MSG_GOSSIP_TEXT_NEXT_5, GOSSIP_SENDER_MAIN, 45);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, me->GetGUID());
                break;
            case 35:
            {
                CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(139);
                AddTitle(player, me, titleInfo);
            }
            break;
            case 36:
            {
                CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(77);
                AddTitle(player, me, titleInfo);
            }
            break;
            case 37:
            {
                CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(74);
                AddTitle(player, me, titleInfo);
            }
            break;
            case 38:
            {
                CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(45);
                AddTitle(player, me, titleInfo);
            }
            break;
            case 40:
            {
                CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(151);
                AddTitle(player, me, titleInfo);
            }
            break;
            case 41:
            {
                CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(171);
                AddTitle(player, me, titleInfo);
            }
            break;
            case 42:
            {
                CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(131);
                AddTitle(player, me, titleInfo);
            }
            break;
            case 43:
            {
                CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(173);
                AddTitle(player, me, titleInfo);
            }
            break;
            case 45:
                player->PlayerTalkClass->ClearMenus();
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_75, GOSSIP_SENDER_MAIN, 46);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_140, GOSSIP_SENDER_MAIN, 47);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_172, GOSSIP_SENDER_MAIN, 48);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_124, GOSSIP_SENDER_MAIN, 49);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_133, GOSSIP_SENDER_MAIN, 50);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_76, GOSSIP_SENDER_MAIN, 51);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_125, GOSSIP_SENDER_MAIN, 52);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_145, GOSSIP_SENDER_MAIN, 53);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_155, GOSSIP_SENDER_MAIN, 54);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, MSG_GOSSIP_TEXT_84, GOSSIP_SENDER_MAIN, 55);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, me->GetGUID());
                break;
            case 46:
            {
                CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(75);
                AddTitle(player, me, titleInfo);
            }
            break;
            case 47:
            {
                CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(140);
                AddTitle(player, me, titleInfo);
            }
            break;
            case 48:
            {
                CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(172);
                AddTitle(player, me, titleInfo);
            }
            break;
            case 49:
            {
                CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(124);
                AddTitle(player, me, titleInfo);
            }
            break;
            case 50:
            {
                CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(133);
                AddTitle(player, me, titleInfo);
            }
            break;
            case 51:
            {
                CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(125);
                AddTitle(player, me, titleInfo);
            }
            break;
            case 52:
            {
                CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(76);
                AddTitle(player, me, titleInfo);
            }
            break;
            case 53:
            {
                CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(145);
                AddTitle(player, me, titleInfo);
            }
            break;
            case 54:
            {
                CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(155);
                AddTitle(player, me, titleInfo);
            }
            break;
            case 55:
            {
                CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(84);
                AddTitle(player, me, titleInfo);
            }
            break;
            case 56:
                GossipHello(player);
                break;
            }

            return true;
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_titleAI(creature);
    }
};

void AddSC_npc_title()
{
    new npc_title;
}
