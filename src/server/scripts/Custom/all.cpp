#include "ScriptPCH.h"
#include "vip_account.h"
#include "ScriptedGossip.h"
#include "Chat.h"
#include "DBCStores.h"

#define CONST_ARENA_RENAME 500
#define CONST_ARENA_CUSTOMIZE 500
#define CONST_ARENA_CHANGE_FACTION 1000
#define CONST_ARENA_CHANGE_RACE 1000

#define SPELL_ID_PASSIVE_RESURRECTION_SICKNESS  15007

class npc_all : public CreatureScript
{
public:
    npc_all() : CreatureScript("npc_all") { }

    struct npc_allAI : public ScriptedAI
    {
      npc_allAI(Creature* creature) : ScriptedAI(creature) { }
      
      bool OnGossipHello(Player *player, Creature *_Creature)
      {
          player->PlayerTalkClass->ClearMenus();
          if (IsVip(player))
        {

          AddGossipItemFor(player,0, "|cff000080Морфы", GOSSIP_SENDER_MAIN, 100);
          AddGossipItemFor(player,0, "|cff0000CD Телепорт", GOSSIP_SENDER_MAIN, 101); // 1.1
          AddGossipItemFor(player,0, "|cff000080Поднять уровень владения оружиями на максимум", GOSSIP_SENDER_MAIN, 102);
          AddGossipItemFor(player,0, "|cffB22222 Тренеры", GOSSIP_SENDER_MAIN, 103); // 1.2
          AddGossipItemFor(player,0, "|cff000080Забавные вещи", GOSSIP_SENDER_MAIN, 105);
          AddGossipItemFor(player,0, "|cff000080Анонсы  поля боев", GOSSIP_SENDER_MAIN, 106);
          AddGossipItemFor(player,0, "|cff000080Снять маску смерти", GOSSIP_SENDER_MAIN, 120);
          AddGossipItemFor(player,0, "|cff000080Снять дизертира", GOSSIP_SENDER_MAIN, 121);
          AddGossipItemFor(player,0, "|cff000080Бафы", GOSSIP_SENDER_MAIN, 125);
          AddGossipItemFor(player,0, "|cff000080Обнулить время перезарядки на задания", GOSSIP_SENDER_MAIN, 126);
          AddGossipItemFor(player,0, "|cff000080Сохранить персонажа", GOSSIP_SENDER_MAIN, 127);
          AddGossipItemFor(player,0, "|cff000080Обнулить время перезарядки способностей", GOSSIP_SENDER_MAIN, 128);
          AddGossipItemFor(player,0, "|cff000080Получить 4 V.I.P сумки на 36 слотов", GOSSIP_SENDER_MAIN, 129);
          AddGossipItemFor(player,0, "|cff000080Сменить имя,фракцию,расу за очки арены", GOSSIP_SENDER_MAIN, 130);
          AddGossipItemFor(player,0, "|cff000080Просмотр трейлеров", GOSSIP_SENDER_MAIN, 131);
          AddGossipItemFor(player,0, "|cff000080V.I.P звания", GOSSIP_SENDER_MAIN, 132);
          AddGossipItemFor(player,0, "|cff000080Снять все ауры", GOSSIP_SENDER_MAIN, 134);
          AddGossipItemFor(player,0, "|cff000080Починить всю экипировку", GOSSIP_SENDER_MAIN, 135);
          AddGossipItemFor(player,0, "|cff000080Изучить верховую езду", GOSSIP_SENDER_MAIN, 136);
          AddGossipItemFor(player,0, "|cff000080Исцели меня", GOSSIP_SENDER_MAIN, 137);
          AddGossipItemFor(player,0, "|cffCD3333 Сбросить мои таланты", GOSSIP_SENDER_MAIN, 138);
          AddGossipItemFor(player,0, "|cff000080Верховые  животные", GOSSIP_SENDER_MAIN, 139);
          AddGossipItemFor(player,0, "|cff000080Выйти из боя", GOSSIP_SENDER_MAIN, 140);
          SendGossipMenuFor(player, 1, _Creature->GetGUID());
          return true;
          }
          else
          {
            ChatHandler(player->GetSession()).PSendSysMessage("|cffff6060[Премиум мастер]:|r У вас нет доступа к  этому  нпс!");
            CloseGossipMenuFor(player);
          }
      }

      void SendDefaultMenu(Player *player, Creature *_Creature, uint32 action)
      {

      // Not allow in combat
      if (player->IsInCombat())
      {
          CloseGossipMenuFor(player);
          ChatHandler(player->GetSession()).PSendSysMessage("|cffff6060[Премиум мастер]:|r Вы в бою!");
          return;
      }

          switch(action)

          {

              //////////////// MAIN MENUS -> Normal Menus //////////////////
          case 100: // Morphs
            player->PlayerTalkClass->ClearMenus();
              AddGossipItemFor(player,0, "Стар крафт мурлок", GOSSIP_SENDER_MAIN, 500);
              AddGossipItemFor(player,0, "Ворген", GOSSIP_SENDER_MAIN, 501);
              AddGossipItemFor(player,0, "Пандарен монах", GOSSIP_SENDER_MAIN, 502);
              AddGossipItemFor(player,0, "Маг гном", GOSSIP_SENDER_MAIN, 503);
              AddGossipItemFor(player,0, "Таурен", GOSSIP_SENDER_MAIN, 504);
              AddGossipItemFor(player,0, "Нежить", GOSSIP_SENDER_MAIN, 505);
              AddGossipItemFor(player,0, "Человек", GOSSIP_SENDER_MAIN, 506);
              AddGossipItemFor(player,0, "Король лич", GOSSIP_SENDER_MAIN, 507);
              AddGossipItemFor(player,0, "Тирион Фолдринг", GOSSIP_SENDER_MAIN, 508);
              AddGossipItemFor(player,0, "Утер Светоносный", GOSSIP_SENDER_MAIN, 509);
              AddGossipItemFor(player,0, "Артас", GOSSIP_SENDER_MAIN, 510);
              ///////////// From SPGM
              AddGossipItemFor(player,4, "Алгалон", GOSSIP_SENDER_MAIN, 711);
              AddGossipItemFor(player,4, "Судьболом", GOSSIP_SENDER_MAIN, 712);
              AddGossipItemFor(player,4, "Дум Лорд", GOSSIP_SENDER_MAIN, 713);
              AddGossipItemFor(player,4, "Ониксия", GOSSIP_SENDER_MAIN, 714);
              AddGossipItemFor(player,4, "Таддиус", GOSSIP_SENDER_MAIN, 715);
              AddGossipItemFor(player,4, "Глут", GOSSIP_SENDER_MAIN, 716);
              AddGossipItemFor(player,4, "Мексна", GOSSIP_SENDER_MAIN, 717);
              AddGossipItemFor(player,4, "Сапфирон", GOSSIP_SENDER_MAIN, 718);
              AddGossipItemFor(player,4, "Нот Чумной", GOSSIP_SENDER_MAIN, 719);
              AddGossipItemFor(player,4, "Кел` Тузад", GOSSIP_SENDER_MAIN, 720);
              AddGossipItemFor(player,4, "Кел` Талас", GOSSIP_SENDER_MAIN, 721);
              AddGossipItemFor(player,4, "К'Тун", GOSSIP_SENDER_MAIN, 722);
              AddGossipItemFor(player,4, "Тралл", GOSSIP_SENDER_MAIN, 723);
          AddGossipItemFor(player,4, "Илидан", GOSSIP_SENDER_MAIN, 724);
          AddGossipItemFor(player,4, "Снять все морфы", GOSSIP_SENDER_MAIN, 725);
              SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE,_Creature->GetGUID());
          break;


          case 101: // Teleport
              AddGossipItemFor(player,1, "Подземелья/Рейды", GOSSIP_SENDER_MAIN, 600);
              AddGossipItemFor(player,1, "Города", GOSSIP_SENDER_MAIN, 601);
              SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE,_Creature->GetGUID());
              break;

          case 102: // Skill maxer
              CloseGossipMenuFor(player);
              player->UpdateSkillsForLevel();
          ChatHandler(player->GetSession()).PSendSysMessage("|cffff6060[Премиум мастер]:|r Ваши умения владения оружием повышены  до максимума!");
              break;

          case 103: // Trainers
              player->PlayerTalkClass->ClearMenus();
              AddGossipItemFor(player,1, "Тренер профессий", GOSSIP_SENDER_MAIN, 800);
              AddGossipItemFor(player,1, "Классовый тренер", GOSSIP_SENDER_MAIN, 801);
              AddGossipItemFor(player,1, "Мастер верховой езды", GOSSIP_SENDER_MAIN, 803);
              SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE,_Creature->GetGUID());
              break;

          case 104: // Server Menus
            player->PlayerTalkClass->ClearMenus();
              AddGossipItemFor(player,1, "Помогите! Я был взломан (блокировка аккаунта)", GOSSIP_SENDER_MAIN, 900);
              AddGossipItemFor(player,0, "Сообщить о лаге", GOSSIP_SENDER_MAIN, 901);
              AddGossipItemFor(player,0, "Помощь", GOSSIP_SENDER_MAIN, 903);
              SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE,_Creature->GetGUID());
              break;

          case 105: // Funny Things, Need some more
            player->PlayerTalkClass->ClearMenus();
              //AddGossipItemFor(player,0, "Получить деньги", GOSSIP_SENDER_MAIN, 1000); // DELETE THIS IF YOU DONT WANT PLAYERS TO GET MONEY
              AddGossipItemFor(player,0, "Опьянеть", GOSSIP_SENDER_MAIN, 1001);
              AddGossipItemFor(player,0, "Протрезветь", GOSSIP_SENDER_MAIN, 1002);
              SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE,_Creature->GetGUID());
              break;

          case 106: // Announcer Part
            player->PlayerTalkClass->ClearMenus();
              AddGossipItemFor(player,0, "Регистрируйте Ущелье Песни Войны!", GOSSIP_SENDER_MAIN, 1100);
              AddGossipItemFor(player,0, "Регистрируйте Низину Арати!", GOSSIP_SENDER_MAIN, 1101);
              AddGossipItemFor(player,0, "Регистрируйте Озеро Ледяных Оков!", GOSSIP_SENDER_MAIN, 1102);
              AddGossipItemFor(player,0, "Регистрируйте Берег Древних!", GOSSIP_SENDER_MAIN, 1103);
              AddGossipItemFor(player,0, "Регистрируйте Альтеракскую долину!", GOSSIP_SENDER_MAIN, 1104);
              AddGossipItemFor(player,0, "Регистрируйте Случайное поле боя!", GOSSIP_SENDER_MAIN, 1105);
              AddGossipItemFor(player,0, "Регистрируйте арену 2v2!", GOSSIP_SENDER_MAIN, 1106);
              AddGossipItemFor(player,0, "Регистрируйте арену 3v3!", GOSSIP_SENDER_MAIN, 1107);
              AddGossipItemFor(player,0, "Регистрируйте арену 5v5!", GOSSIP_SENDER_MAIN, 1108);
              AddGossipItemFor(player,0, "Регистрируйте Око Бури!", GOSSIP_SENDER_MAIN, 1109);
              SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE,_Creature->GetGUID());
              break;


          // Titles
        case 132: // Titles
            player->PlayerTalkClass->ClearMenus();
          AddGossipItemFor(player,0, " Львиное Сердце", GOSSIP_SENDER_MAIN, 3499);
              AddGossipItemFor(player,0, " Бессмерный", GOSSIP_SENDER_MAIN, 3501);
              AddGossipItemFor(player,0, " посланник титанов", GOSSIP_SENDER_MAIN, 3502);
              AddGossipItemFor(player,0, " Вершитель правосудия", GOSSIP_SENDER_MAIN, 3503);
              AddGossipItemFor(player,0, " Безумно влюбленный", GOSSIP_SENDER_MAIN, 3504);
              AddGossipItemFor(player,0, " повелитель штормов", GOSSIP_SENDER_MAIN, 3505);
          SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE,_Creature->GetGUID());
              break;
          
       
        case 125://Бафы
          CloseGossipMenuFor(player);
          player->CastSpell(player, 30550, true);
          player->CastSpell(player, 23766, true);
          player->CastSpell(player, 23769, true);
          player->CastSpell(player, 23738, true);
          player->CastSpell(player, 23737, true);
          player->CastSpell(player, 23736, true);
          player->CastSpell(player, 23767, true);
          player->CastSpell(player, 23735, true);
          player->CastSpell(player, 23768, true);
          ChatHandler(player->GetSession()).PSendSysMessage("|cffff6060[Премиум мастер]:|r На вас наложены  |cFF76EE00V.I.P бафы|r!");
          break;
              SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE,_Creature->GetGUID());
               break;
           
           ///Customize
        case 130://Customize
          player->PlayerTalkClass->ClearMenus();
          AddGossipItemFor(player,0, "Сменить имя [500 очков арены]", GOSSIP_SENDER_MAIN, 2400);
          AddGossipItemFor(player,0, "Изменить персонажа [500 очков арены]", GOSSIP_SENDER_MAIN, 2401);
          AddGossipItemFor(player,0, "Сменить фракцию [1000 очков арены]", GOSSIP_SENDER_MAIN, 2402);
          AddGossipItemFor(player,0, "Сменить рассу [1000 очков арены]", GOSSIP_SENDER_MAIN, 2403);
          SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE,_Creature->GetGUID());
          break;
          
        case 139://Mounts
            player->PlayerTalkClass->ClearMenus();
          AddGossipItemFor(player,9, "Обычные верховые животные", GOSSIP_SENDER_MAIN, 8000);
          AddGossipItemFor(player,9, "Элитные верховые животные", GOSSIP_SENDER_MAIN, 8100);
          AddGossipItemFor(player,9, "Прото верховые животные", GOSSIP_SENDER_MAIN, 8200);
          SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE,_Creature->GetGUID());
          
        case 8000: // Mounts Ground
              player->PlayerTalkClass->ClearMenus();
              AddGossipItemFor(player,5, "Черный анкиражский боевой кристал", GOSSIP_SENDER_MAIN, 8001);
              AddGossipItemFor(player,5, "Аманский боевой медведь", GOSSIP_SENDER_MAIN, 8002);
              AddGossipItemFor(player,5, "Большой боевой медведь", GOSSIP_SENDER_MAIN, 8003);
              AddGossipItemFor(player,5, "Мертвый скакун", GOSSIP_SENDER_MAIN, 8004);
              AddGossipItemFor(player,5, "Боевой  конь", GOSSIP_SENDER_MAIN, 8005);
              AddGossipItemFor(player,5, "Большой волк", GOSSIP_SENDER_MAIN, 8006);
              AddGossipItemFor(player,5, "Огромный кодо", GOSSIP_SENDER_MAIN, 8007);
              AddGossipItemFor(player,5, "Ездовой волк", GOSSIP_SENDER_MAIN, 8008);
              AddGossipItemFor(player,5, "Магическое ездовое  яйцо", GOSSIP_SENDER_MAIN, 8009);
              AddGossipItemFor(player,5, "Поводья белого полярного медведя", GOSSIP_SENDER_MAIN, 8010);
              AddGossipItemFor(player,5, "Поводья серого полярного медведя", GOSSIP_SENDER_MAIN, 8011);
              AddGossipItemFor(player,5, "Поводья мертвого  скакуна", GOSSIP_SENDER_MAIN, 8012);
              AddGossipItemFor(player,5, "Поводья повелителя воронов", GOSSIP_SENDER_MAIN, 8013);
              AddGossipItemFor(player,5, "Поводья призрачного тигра", GOSSIP_SENDER_MAIN, 8014);
              AddGossipItemFor(player,5, "Морская черепаха", GOSSIP_SENDER_MAIN, 8015);
              AddGossipItemFor(player,5, "Ездовой раптор", GOSSIP_SENDER_MAIN, 8016);
              AddGossipItemFor(player,5, "Стремительный белый крылобег", GOSSIP_SENDER_MAIN, 8017);
              AddGossipItemFor(player,5, "Зебра", GOSSIP_SENDER_MAIN, 8018);
              AddGossipItemFor(player,5, "Зуляманский тигр", GOSSIP_SENDER_MAIN, 8019);
              AddGossipItemFor(player,5, "Поводья Всадника без головы", GOSSIP_SENDER_MAIN, 8020);
              AddGossipItemFor(player,5, "Белый ездовой Рино", GOSSIP_SENDER_MAIN, 8021);
              AddGossipItemFor(player,5, "Четыре анкиражских ездовых кристала", GOSSIP_SENDER_MAIN, 8022);
              SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE,_Creature->GetGUID());
              break;
              
        case 8100: // Mounts Fly
              player->PlayerTalkClass->ClearMenus();
              AddGossipItemFor(player,5, "Серебряный Гиппогриф", GOSSIP_SENDER_MAIN, 8101);
              AddGossipItemFor(player,5, "Пепел Алар", GOSSIP_SENDER_MAIN, 8102);
              AddGossipItemFor(player,5, "Большая ракета любви", GOSSIP_SENDER_MAIN, 8103);
              AddGossipItemFor(player,5, "Сверкающий Гиппогриф", GOSSIP_SENDER_MAIN, 8104);
              AddGossipItemFor(player,5, "Стремительный дракон пустоты", GOSSIP_SENDER_MAIN, 8105);
              AddGossipItemFor(player,5, "Мстительный дракон Пустоты", GOSSIP_SENDER_MAIN, 8106);
              AddGossipItemFor(player,5, "Безжалостный дракон Пустоты", GOSSIP_SENDER_MAIN, 8107);
              AddGossipItemFor(player,5, "Жестокий дракон Пустоты ", GOSSIP_SENDER_MAIN, 8108);
              AddGossipItemFor(player,5, "Небесные Скакун", GOSSIP_SENDER_MAIN, 8109);
              AddGossipItemFor(player,5, "Смертельный Ледяной змей", GOSSIP_SENDER_MAIN, 8110);
              AddGossipItemFor(player,5, "Неистовый Ледяной змей", GOSSIP_SENDER_MAIN, 8111);
              AddGossipItemFor(player,5, "Безжалостный Ледяной змей", GOSSIP_SENDER_MAIN, 8112);
              AddGossipItemFor(player,5, "Разгневаный Ледяной змей ", GOSSIP_SENDER_MAIN, 8113);
              AddGossipItemFor(player,5, "Поводья непобедимого", GOSSIP_SENDER_MAIN, 8114);
              AddGossipItemFor(player,5, "Голова мирмирона", GOSSIP_SENDER_MAIN, 8115);
              AddGossipItemFor(player,5, "Поводья альбиноса  дракона", GOSSIP_SENDER_MAIN, 8116);
              AddGossipItemFor(player,5, "Поводья лазурного дракона", GOSSIP_SENDER_MAIN, 8117);
              AddGossipItemFor(player,5, "Поводья черного дракона", GOSSIP_SENDER_MAIN, 8118);
              AddGossipItemFor(player,5, "Поводья синего дракона", GOSSIP_SENDER_MAIN, 8119);
              AddGossipItemFor(player,5, "Поводья бронзового дракона", GOSSIP_SENDER_MAIN, 8120);
              AddGossipItemFor(player,5, "Поводья сумеречного дракона", GOSSIP_SENDER_MAIN, 8121);
              AddGossipItemFor(player,5, "Поводья дракона ониксии", GOSSIP_SENDER_MAIN, 8122);
              AddGossipItemFor(player,5, "Х -51 Ракета Пустоты X- TREME", GOSSIP_SENDER_MAIN, 8123);
              AddGossipItemFor(player,5, "X-53 Ракета путешествий", GOSSIP_SENDER_MAIN, 8124);
              AddGossipItemFor(player,5, "Оледеневшее сердце ледяного змея", GOSSIP_SENDER_MAIN, 8125);
              SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE,_Creature->GetGUID());
              break;
              
        case 8200:
              player->PlayerTalkClass->ClearMenus();
              AddGossipItemFor(player,5, "Поводья Черного протодракона", GOSSIP_SENDER_MAIN, 8201);
              AddGossipItemFor(player,5, "Поводья Синего протодракона", GOSSIP_SENDER_MAIN, 8202);
              AddGossipItemFor(player,5, "Поводья Зеленого протодракона", GOSSIP_SENDER_MAIN, 8203);
              AddGossipItemFor(player,5, "Поводья Железного протодракона", GOSSIP_SENDER_MAIN, 8204);
              AddGossipItemFor(player,5, "Поводья Измученного протодракона", GOSSIP_SENDER_MAIN, 8205);
              AddGossipItemFor(player,5, "Поводья Красного протодракона", GOSSIP_SENDER_MAIN, 8206);
              AddGossipItemFor(player,5, "Поводья Ржавого протодракона", GOSSIP_SENDER_MAIN, 8207);
              AddGossipItemFor(player,5, "Поводья Затерянного во времени протодракона", GOSSIP_SENDER_MAIN, 8208);
              AddGossipItemFor(player,5, "Поводья Фиолетового протодракона", GOSSIP_SENDER_MAIN, 8209);
              SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE,_Creature->GetGUID());
              break;
              
        case 140: // Clear combat
              CloseGossipMenuFor(player);
              player->ClearInCombat();
              ChatHandler(player->GetSession()).PSendSysMessage("|cffff6060[Премиум мастер]:|r Вы вышли из боя!");
              break;
              
        case 8001: // Black Qiraji Resonating Crystal
              CloseGossipMenuFor(player);
              player->AddItem(21176, 1);
              break;
        case 8002: // Amani War Bear
              CloseGossipMenuFor(player);
              player->AddItem(33809, 1);
              break;
        case 8003: // Big Battle Bear
              CloseGossipMenuFor(player);
              player->AddItem(38576, 1);
              break;
        case 8004: // Deathcharger's Reins
              CloseGossipMenuFor(player);
              player->AddItem(13335, 1);
              break;
        case 8005: // Fiery Warhorse's Reins
              CloseGossipMenuFor(player);
              player->AddItem(30480, 1);
              break;
        case 8006: // Swift Burgundy Wolf
              CloseGossipMenuFor(player);
              player->CastSpell(player, 65646, true); // Cast Metod Because Item (Faction Flag)
              break;
        case 8007: // Great Brewfest Kodo
              CloseGossipMenuFor(player);
              player->AddItem(37828, 1);
              break;
        case 8008: // Horn of the Frostwolf Howler
              CloseGossipMenuFor(player);
              player->CastSpell(player, 23509, true); // Cast Metod Because Item (Faction Flag)
              break;
        case 8009: // Magic Rooster Egg
              CloseGossipMenuFor(player);
              player->AddItem(46778, 1);
              break;
        case 8010: // Reins of the White Polar Bear
              CloseGossipMenuFor(player);
              player->AddItem(43962, 1);
              break;
        case 8011: // Reins of the Brown Polar Bear
              CloseGossipMenuFor(player);
              player->AddItem(43963, 1);
              break;
        case 8012: // Reins of the Crimson Deathcharger
              CloseGossipMenuFor(player);
              player->AddItem(52200, 1);
              break;
        case 8013: // Reins of the Raven Lord
              CloseGossipMenuFor(player);
              player->AddItem(32768, 1);
              break;
        case 8014: // Reins of the Swift Spectral Tiger
              CloseGossipMenuFor(player);
              player->AddItem(33225, 1);
              break;
        case 8015: // Sea Turtle
              CloseGossipMenuFor(player);
              player->AddItem(46109, 1);
              break;
        case 8016: // Swift Razzashi Raptor
              CloseGossipMenuFor(player);
              player->AddItem(19872, 1);
              break;
        case 8017: // Swift White Hawkstrider
              CloseGossipMenuFor(player);
              player->AddItem(35513, 1);
              break;
        case 8018: // Swift Zhevra
              CloseGossipMenuFor(player);
              player->AddItem(37719, 1);
              break;
        case 8019: // Swift Zulian Tiger
              CloseGossipMenuFor(player);
              player->AddItem(19902, 1);
              break;
        case 8020: // The Horseman's Reins
              CloseGossipMenuFor(player);
              player->AddItem(37012, 1);
              break;
        case 8021: // Wooly White Rhino
              CloseGossipMenuFor(player);
              player->AddItem(54068, 1);
              break;
        case 8022: // Four Qiraji Resonating Crystal
              CloseGossipMenuFor(player);
              player->AddItem(21218, 1); // Blue Qiraji Resonating Crystal
              player->AddItem(21323, 1); // Green Qiraji Resonating Crystal
              player->AddItem(21321, 1); // Red Qiraji Resonating Crystal
              player->AddItem(21324, 1); // Yellow Qiraji Resonating Crystal
              break;
        case 8101: // Argent Hippogryph
              CloseGossipMenuFor(player);
              player->AddItem(45725, 1);
              break;
        case 8102: // Ashes of Alar
              CloseGossipMenuFor(player);
              player->AddItem(32458, 1);
              break;
        case 8103: // Big Love Rocket
              CloseGossipMenuFor(player);
              player->AddItem(50520, 1);
              break;
        case 8104: // Blazing Hippogryph
              CloseGossipMenuFor(player);
              player->AddItem(54069, 1);
              break;
        case 8105: // Swift Nether Drake
              CloseGossipMenuFor(player);
              player->AddItem(30609, 1);
              break;
        case 8106: // Vengeful Nether Drake
              CloseGossipMenuFor(player);
              player->AddItem(37676, 1);
              break;
        case 8107: // Merciless Nether Drake
              CloseGossipMenuFor(player);
              player->AddItem(34092, 1);
              break;
        case 8108: // Brutal Nether Drake
              CloseGossipMenuFor(player);
              player->AddItem(43516, 1);
              break;
        case 8109: // Celestial Steed
              CloseGossipMenuFor(player);
              player->AddItem(54811, 1);
              break;
        case 8110: // Deadly Gladiator's Frost Wyrm
              CloseGossipMenuFor(player);
              player->AddItem(46708, 1);
              break;
        case 8111: // Furious Gladiator's Frost Wyrm
              CloseGossipMenuFor(player);
              player->AddItem(46171, 1);
              break;
        case 8112: // Relentless Gladiator's Frost Wyrm
              CloseGossipMenuFor(player);
              player->AddItem(47840, 1);
              break;
        case 8113: // Wrathful Gladiator's Frost Wyrm
              CloseGossipMenuFor(player);
              player->AddItem(50435, 1);
              break;
        case 8114: // Invincibles Reins
              CloseGossipMenuFor(player);
              player->AddItem(50818, 1);
              break;
        case 8115: // Mimiron's Head
              CloseGossipMenuFor(player);
              player->AddItem(45693, 1);
              break;
        case 8116: // Reins of the Albino Drake
              CloseGossipMenuFor(player);
              player->AddItem(44178, 1);
              break;
        case 8117: // Reins of the Azure Drake
              CloseGossipMenuFor(player);
              player->AddItem(43952, 1);
              break;
        case 8118: // Reins of the Black Drake
              CloseGossipMenuFor(player);
              player->AddItem(43986, 1);
              break;
        case 8119: // Reins of the Blue Drake
              CloseGossipMenuFor(player);
              player->AddItem(43953, 1);
              break;
        case 8120: // Reins of the Bronze Drake
              CloseGossipMenuFor(player);
              player->AddItem(43951, 1);
              break;
        case 8121: // Reins of the Onyxian Drake
              CloseGossipMenuFor(player);
              player->AddItem(49636, 1);
              break;
        case 8122: // Reins of the Twilight Drake
              CloseGossipMenuFor(player);
              player->AddItem(43954, 1);
              break;
        case 8123: // X-51 Nether-Rocket X-TREME
              CloseGossipMenuFor(player);
              player->AddItem(35226, 1);
              break;
        case 8124: // X-53 Touring Rocket
              CloseGossipMenuFor(player);
              player->AddItem(54860, 1);
              break;
        case 8125: // Frozen Frost Wyrm Heart
              CloseGossipMenuFor(player);   // 33182 Swift Flying Broom // // 33184 Swift Magic Broom // 37011 Magic Broom // 33183 ld Magic Broom
              player->AddItem(38690, 1);
              break;
        case 8201: // Reins of the Black Proto-Drake
              CloseGossipMenuFor(player);
              player->AddItem(44164, 1);
              break;
        case 8202: // Reins of the Blue Proto-Drake
              CloseGossipMenuFor(player);
              player->AddItem(44151, 1);
              break;
        case 8203: // Reins of the Green Proto-Drake
              CloseGossipMenuFor(player);
              player->AddItem(44707, 1);
              break;
        case 8204: // Reins of the Ironbound Proto-Drake
              CloseGossipMenuFor(player);
              player->AddItem(45801, 1);
              break;
        case 8205: // Reins of the Plagued Proto-Drake
              CloseGossipMenuFor(player);
              player->AddItem(44175, 1);
              break;
        case 8206: // Reins of the Red Proto-Drake
              CloseGossipMenuFor(player);
              player->AddItem(44160, 1);
              break;
        case 8207: // Reins of the Rusted Proto-Drake
              CloseGossipMenuFor(player);
              player->AddItem(45802, 1);
              break;
        case 8208: // Reins of the Time-Lost Proto-Drake
              CloseGossipMenuFor(player);
              player->AddItem(44168, 1);
              break;
        case 8209: // Reins of the Violet Proto-Drake
              CloseGossipMenuFor(player);
              player->AddItem(44177, 1);
              break;
          
        case 2400: // Rename
              if (player->GetArenaPoints() < CONST_ARENA_RENAME)
              {
                ChatHandler(player->GetSession()).PSendSysMessage("|cffff6060[Премиум мастер]:|r У вас нет  достаточного количества очков арены для изменения имя персонажа!");
                CloseGossipMenuFor(player);
              }
              else
              {
                CloseGossipMenuFor(player);
                player->SetAtLoginFlag(AT_LOGIN_RENAME);
                player->ModifyArenaPoints(-CONST_ARENA_RENAME);
                ChatHandler(player->GetSession()).PSendSysMessage("|cffff6060[Премиум мастер]:|r Выйдите с мира для смены имени!");
              }
              break;
              
        case 2401: // Customize
              if (player->GetArenaPoints() < CONST_ARENA_CUSTOMIZE)
              {
                ChatHandler(player->GetSession()).PSendSysMessage("|cffff6060[Премиум мастер]:|r У вас нет  достаточного количества очков арены для изменения имя персонажа и вида!");
                CloseGossipMenuFor(player);
              }
              else
              {
                CloseGossipMenuFor(player);
                player->SetAtLoginFlag(AT_LOGIN_CUSTOMIZE);
                player->ModifyArenaPoints(-CONST_ARENA_CUSTOMIZE);
                ChatHandler(player->GetSession()).PSendSysMessage("|cffff6060[Премиум мастер]:|r Выйдите с мира для преобразования персонажа!");
              }
              break;
              
        case 2402: // Change Faction
              if (player->GetArenaPoints() < CONST_ARENA_CHANGE_FACTION)
              {
                ChatHandler(player->GetSession()).PSendSysMessage("|cffff6060[Премиум мастер]:|r У вас нет  достаточного количества очков арены для изменения фракции персонажа!");
                CloseGossipMenuFor(player);
              }
              else
              {
                CloseGossipMenuFor(player);
                player->SetAtLoginFlag(AT_LOGIN_CHANGE_FACTION);
                player->ModifyArenaPoints(-CONST_ARENA_CHANGE_FACTION);
                ChatHandler(player->GetSession()).PSendSysMessage("|cffff6060[Премиум мастер]:|r Выйдите с мира для смены фракции персонажа!");
              }
              break;
              
        case 2403: // Change Race
              if (player->GetArenaPoints() < CONST_ARENA_CHANGE_RACE)
              {
                ChatHandler(player->GetSession()).PSendSysMessage("|cffff6060[Премиум мастер]:|r У вас нет  достаточного количества очков арены для изменения расы персонажа!");
                CloseGossipMenuFor(player);
              }
              else
              {
                CloseGossipMenuFor(player);
                player->SetAtLoginFlag(AT_LOGIN_CHANGE_RACE);
                player->ModifyArenaPoints(-CONST_ARENA_CHANGE_RACE);
                ChatHandler(player->GetSession()).PSendSysMessage("|cffff6060[Премиум мастер]:|r Выйдите с мира для смены расы персонажа!");
              }
              break;
              
        case 131: // Movie Menu
              player->PlayerTalkClass->ClearMenus();
              AddGossipItemFor(player,5, "Времена Илидана", GOSSIP_SENDER_MAIN, 2410);
              AddGossipItemFor(player,5, "Гнев Короля лича", GOSSIP_SENDER_MAIN, 2411);
              AddGossipItemFor(player,5, "Падение короля лича", GOSSIP_SENDER_MAIN, 2412);
              SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE,_Creature->GetGUID());
              break;
              
        case 2410: // Classic
              CloseGossipMenuFor(player);
              player->SendMovieStart(2);
              break;
              
        case 2411: // The Wrath Gate
              CloseGossipMenuFor(player);
              player->SendMovieStart(14);
              break;
              
        case 2412: // Fall of The Lich King
              CloseGossipMenuFor(player);
              player->SendMovieStart(16);
              break;
          
          ///Reset Daily and Weekly Quest Cooldown
          case 126:
            CloseGossipMenuFor(player);
          player->ResetDailyQuestStatus();
          player->ResetWeeklyQuestStatus();
          ChatHandler(player->GetSession()).PSendSysMessage("|cffff6060[Премиум мастер]:|r Время перезарядки заданий,сброшено!");
          break;
          
          ///Save Character Online	
          case 127:
          CloseGossipMenuFor(player);
            player->SaveToDB();
          ChatHandler(player->GetSession()).PSendSysMessage("|cffff6060[Премиум мастер]:|r Ваш персонаж сохранен!");
          break;
          
          ///Reset cooldown
          case 128:
          CloseGossipMenuFor(player);
          player->RemoveArenaSpellCooldowns();
          ChatHandler(player->GetSession()).PSendSysMessage("|cffff6060[Премиум мастер]:|r Перезарядка на классовые навыки,обнулена!");
          break;
          
          /// 36 Slot Bag
          case 129:
                  if (player->HasItemCount(23162,4))
              {
                CloseGossipMenuFor(player);
                ChatHandler(player->GetSession()).PSendSysMessage("|cffff6060[Премиум мастер]:|r У вас уже есть VIP сумки!");
              }
              else
              {
                CloseGossipMenuFor(player);
                player->AddItem(23162, 4);
              }
              break;
              
            case 137: // Heal
              if (player->HasAura(45523))
            {
              CloseGossipMenuFor(player);
              ChatHandler(player->GetSession()).PSendSysMessage("|cffff6060[Премиум мастер]:|r Вы были исцелены!");
                      }
                      else
                      {
                CloseGossipMenuFor(player);
                          player->CastSpell(player, 25840, true);
                          player->CastSpell(player, 45523, true);
                      }
                      break;
              
          case 138: // Reset talents
                      CloseGossipMenuFor(player);
                      player->SendTalentWipeConfirm(_Creature->GetGUID());
                      break;
          ///Title
            case 3499:
                  CloseGossipMenuFor(player);
              player->SetTitle(sCharTitlesStore.LookupEntry(114));
              ChatHandler(player->GetSession()).PSendSysMessage("|cffff6060[Премиум мастер]:|r Вы получили новое звание,приятной игры на нашем сервере!");
                      break;
              
          case 3501:
                  CloseGossipMenuFor(player);
              player->SetTitle(sCharTitlesStore.LookupEntry(141));
              ChatHandler(player->GetSession()).PSendSysMessage("|cffff6060[Премиум мастер]:|r Вы получили новое звание,приятной игры на нашем сервере!");
                      break;
              
          case 3502:
                  CloseGossipMenuFor(player);
              player->SetTitle(sCharTitlesStore.LookupEntry(166));
              ChatHandler(player->GetSession()).PSendSysMessage("|cffff6060[Премиум мастер]:|r Вы получили новое звание,приятной игры на нашем сервере!");
                      break;
              
          case 3503:
                  CloseGossipMenuFor(player);
              player->SetTitle(sCharTitlesStore.LookupEntry(48));
              ChatHandler(player->GetSession()).PSendSysMessage("|cffff6060[Премиум мастер]:|r Вы получили новое звание,приятной игры на нашем сервере!");
                      break;
              
          case 3504:
                  CloseGossipMenuFor(player);
              player->SetTitle(sCharTitlesStore.LookupEntry(135));
              ChatHandler(player->GetSession()).PSendSysMessage("|cffff6060[Премиум мастер]:|r Вы получили новое звание,приятной игры на нашем сервере!");
                      break;
              
          case 3505:
                  CloseGossipMenuFor(player);
              player->SetTitle(sCharTitlesStore.LookupEntry(86));
              ChatHandler(player->GetSession()).PSendSysMessage("|cffff6060[Премиум мастер]:|r Вы получили новое звание,приятной игры на нашем сервере!");
                      break;
          
              
          case 134:///Remove all uara
                  CloseGossipMenuFor(player);
                      player->RemoveAllAuras();
              ChatHandler(player->GetSession()).PSendSysMessage("|cffff6060[Премиум мастер]:|r С вас были сняты все ауры!");
              break;
              
          case 135: // Repair Items
                     player->DurabilityRepairAll(false, 0.0f, true);
                     ChatHandler(player->GetSession()).PSendSysMessage("|cffff6060[Премиум мастер]:|r Я починил всю вашу  экипировку!");
                     CloseGossipMenuFor(player);
                     break;
               
            case 136: // Riding
                    CloseGossipMenuFor(player);
                    player->LearnSpell(33388, false);
                    player->LearnSpell(33391, false);
                    player->LearnSpell(34090, false);
                    player->LearnSpell(34091, false);
       
                    player->UpdateSkill(762, 300);
                    ChatHandler(player->GetSession()).PSendSysMessage("|cffff6060[Премиум мастер]:|r Вы выучили верховую езду!");
                    break;
           

              case 1000: // Money
              CloseGossipMenuFor(player);
              player->ModifyMoney(200000000);
              ChatHandler(player->GetSession()).PSendSysMessage("|cffff6060[Премиум мастер]:|r Вы получили небольшое количество золота!");
              break;

              case 1001: // Drunk
              CloseGossipMenuFor(player);
              player->CastSpell(player, 46876, false);
          ChatHandler(player->GetSession()).PSendSysMessage("|cffff6060[Премиум мастер]:|r Вы пьяны!");
              break;

              case 1002: // Sober
              CloseGossipMenuFor(player);
              player->SetDrunkValue(0, 9);
          ChatHandler(player->GetSession()).PSendSysMessage("|cffff6060[Премиум мастер]:|r Вы протрезвели!");
              break;



                     case 500:
              CloseGossipMenuFor(player);
              player->SetDisplayId(29348);
          player->SetFloatValue(OBJECT_FIELD_SCALE_X, 2.0f);
              break;

              case 501:
              CloseGossipMenuFor(player);
              player->SetDisplayId(657);
          player->SetFloatValue(OBJECT_FIELD_SCALE_X, 1.0f);
              break;

              case 502:
              CloseGossipMenuFor(player);
              player->SetDisplayId(30414);
          player->SetFloatValue(OBJECT_FIELD_SCALE_X, 2.0f);
              break;

              case 503:
              CloseGossipMenuFor(player);
              player->SetDisplayId(28586);
              break;

              case 504:
              CloseGossipMenuFor(player);
              player->SetDisplayId(30199);
              break;

              case 505:
              CloseGossipMenuFor(player);
              player->SetDisplayId(2789);
              break;

              case 506:
              CloseGossipMenuFor(player);
              player->SetDisplayId(16280);
              break;

              case 507:
              CloseGossipMenuFor(player);
              player->SetDisplayId(24191);
              break;

              case 508:
              CloseGossipMenuFor(player);
              player->SetDisplayId(22209);
              break;

              case 509:
              CloseGossipMenuFor(player);
              player->SetDisplayId(23889);
              break;

              case 510:
              CloseGossipMenuFor(player);
              player->SetDisplayId(24949);
              break;

              case 711: // Algalon
              CloseGossipMenuFor(player);
              player->SetDisplayId(28641);
              player->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.2f);
              break;

              case 712: // Doomwalker
              CloseGossipMenuFor(player);
              player->SetDisplayId(16630);
              player->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.1f);
              break;

              case 713: // Doomlord kazzak
              CloseGossipMenuFor(player);
              player->SetDisplayId(17887);
              player->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.10f);
              break;

              case 714: // Onyxia
              CloseGossipMenuFor(player);
              player->SetDisplayId(8570);
              player->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.1f);
              break;

              case 715: // Thaddius
              CloseGossipMenuFor(player);
              player->SetDisplayId(16137);
              player->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.2f);
              break;

              case 716: // Gluth
              CloseGossipMenuFor(player);
              player->SetDisplayId(16064);
              player->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.2f);
              break;

              case 717: // Maexxna
              CloseGossipMenuFor(player);
              player->SetDisplayId(15928);
              player->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.15f);
              break;

              case 718: // Sapphiron
              CloseGossipMenuFor(player);
              player->SetDisplayId(16033);
              player->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.1f);
              break;

              case 719: // Noth The Plaguebringer
              CloseGossipMenuFor(player);
              player->SetDisplayId(16590);
              player->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.7f);
              break;

              case 720: // Kel'Thuzad
              CloseGossipMenuFor(player);
              player->SetDisplayId(15945);
              player->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.2f);
              break;

              case 721: // Kael'thas Sunstrider
              CloseGossipMenuFor(player);
              player->SetDisplayId(20023);
              player->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.5f);
              break;

              case 722: // C'thun
              CloseGossipMenuFor(player);
              player->SetDisplayId(15786);
              player->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.1f);
              break;
          
          case 723: // Thrall
              CloseGossipMenuFor(player);
              player->SetDisplayId(4527);
              player->SetFloatValue(OBJECT_FIELD_SCALE_X, 1.0f);
              break;
          
          case 724:// Illidan
              CloseGossipMenuFor(player);
              player->SetDisplayId(21135);
          player->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.5f);
              break;

              case 725: // Demorph Player (remove all morphs)
              CloseGossipMenuFor(player);
                      player->DeMorph();
              player->SetFloatValue(OBJECT_FIELD_SCALE_X, 1.0f);
                      break;
          
          

              case 803: // Mount Trainer
              {
              CloseGossipMenuFor(player);
              player->LearnSpell(33388, false);
              player->LearnSpell(33391, false);
              player->LearnSpell(34090, false);
              player->LearnSpell(34091, false);
              player->LearnSpell(54197, false);
              }


              case 120: // Remove Resurrection Sickness Aura
              if(!player->HasAura(15007))
              {
                CloseGossipMenuFor(player);
                ChatHandler(player->GetSession()).PSendSysMessage("|cffff6060[Премиум мастер]:|r С вас была  снята маска смерти!");
              }
              else
              {
                CloseGossipMenuFor(player);
                player->RemoveAurasDueToSpell(15007);
                player->SetHealth(player->GetMaxHealth()); //Restore Health
                player->SetPower(POWER_MANA, player->GetMaxPower(POWER_MANA)); // Restore Mana
              }
              break;
              
          case 121: // Remove Deserter Aura
              if(!player->HasAura(26013))
              {
                CloseGossipMenuFor(player);
                ChatHandler(player->GetSession()).PSendSysMessage("|cffff6060[Премиум мастер]:|r С вас был снят дизертир!");
              }
              else
              {
                CloseGossipMenuFor(player);
                player->RemoveAurasDueToSpell(26013);
              }
              break;
              
              
      }
      //return true;
          }

      bool OnGossipSelect(Player* player, Creature* _Creature, uint32 sender, uint32 action)
      {
          // Main menu
          player->PlayerTalkClass->ClearMenus();
          if (sender == GOSSIP_SENDER_MAIN)
          SendDefaultMenu(player, _Creature, action);

      return true;
      }
    };
    
    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_allAI(creature);
    }
};

void AddSC_npc_all()
{
    new npc_all();
}