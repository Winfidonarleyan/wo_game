#include "ScriptMgr.h"
#include "ReputationMgr.h"
#include "AchievementMgr.h"
#include "Pet.h"
#include "Log.h"
#include "DatabaseEnv.h"
#include "AccountMgr.h"
#include "ObjectMgr.h"
#include "WorldSession.h"
#include "World.h"
#include "Item.h"
#include "Chat.h"
#include "Mail.h"
#include "SpellMgr.h"
#include "Unit.h"
#include "Bag.h"

uint32 transferId = 1;

struct TalentInfo
{
    uint32 id;
    uint32 rank;
};

struct TalentSpecInfo
{
    std::vector<TalentInfo> talents;
    std::vector<uint32> glyphs;
};

struct ReputationInfo
{
    uint32 flags;
    uint32 standing;
};

struct ItemInfo
{
    uint32 id;
    uint32 count;
    uint32 bagIndex;
    uint32 position;
};

struct AchievementInfo
{
    uint32 id;
    /*uint32 date;*/
    uint32 progress = 0;
};

struct PetInfo
{
    uint32 id;
    uint32 level;
    uint32 exp;
    uint32 talentPoints;
    std::vector<TalentInfo> talents;
};

#define PARSE(...) if (!parse(__VA_ARGS__)) return false

class TransferInfo
{
public:
    TransferInfo(const char* args)
    {
        _args = (char*)args;

        TC_LOG_INFO("server.loading", "Args length: %u", strlen(args));

        _withArgs = true;
        id = 0;
    }

    uint32 id;

    uint32 accountId;
    uint32 race;
    uint32 pClass;
    uint32 gender;
    uint32 skin;
    uint32 face;
    uint32 hairStyle;
    uint32 hairColor;
    uint32 facialStyle;
    uint32 flags = 0;
    uint32 level;
    uint32 money;
    uint32 honorPoints;
    uint32 arenaPoints;
    uint32 mapId;
    float x, y, z;

    std::unordered_map<uint32, uint32> skills;
    std::vector<uint32> spells;

    uint32 talentPoints;
    uint32 activeSpec;
    std::vector<TalentSpecInfo> talents;

    std::vector<ReputationInfo> reputation;
    std::vector<AchievementInfo> completedAchievements;
    std::vector<AchievementInfo> achievementProgress;
    std::vector<uint32> actionButtons;
    std::vector<ItemInfo> bags;
    std::vector<ItemInfo> items;

    PetInfo pet;

    void fillId()
    {
        if (id != 0)
            return;

        id = transferId++;
        CharacterDatabase.DirectPExecute("INSERT INTO character_transfer (id, status) VALUES(%u, 0)", id);
    }

    // Format:
    // character info: account race class gender skin face hair_style hair_color facial_hair level money honor arena flags map x y z
    // skills: count packed1 packed2
    // spells: count spell_id
    // talents: free_talent_points spec_count active_spec id_count id rank glyph_count glyph_id
    // reputation: count flags standing
    // achievements: completed_count id progress_count id counter
    // action buttons: count packed_data
    // items: bag_count bag_entry position item_count entry count bag_index position
    // pet info: id name level free_talents talent_count talent_id rank
    bool parse()
    {
        PARSE(accountId, race, pClass, gender, skin, face, hairStyle, hairColor, facialStyle,
            level, money, honorPoints, arenaPoints, flags, mapId, x, y, z);

        // Skills
        uint32 skillCount;
        PARSE(skillCount);

        skills.reserve(skillCount);

        for (uint32 i = 0; i < skillCount; i++)
        {
            uint32 p1, p2;
            PARSE(p1, p2);

            skills[p1] = p2;
        }

        // Spells
        uint32 spellCount;
        PARSE(spellCount);

        spells.resize(spellCount);

        for (uint32 i = 0; i < spellCount; i++)
        {
            uint32 spellId;

            PARSE(spellId);

            spells[i] = spellId;
        }

        // Talents
        uint32 specCount;
        PARSE(talentPoints, specCount, activeSpec);

        talents.reserve(specCount);

        for (uint32 i = 0; i < specCount; i++)
        {
            uint32 count, glyphCount;
            TalentSpecInfo spec;

            PARSE(count);

            spec.talents.reserve(count);

            for (uint32 j = 0; j < count; j++)
            {
                TalentInfo talent;
                PARSE(talent.id, talent.rank);

                spec.talents.push_back(talent);
            }

            PARSE(glyphCount);

            spec.glyphs.reserve(glyphCount);

            for (uint32 j = 0; j < glyphCount; j++)
            {
                uint32 glyph;
                PARSE(glyph);

                spec.glyphs.push_back(glyph);
            }

            talents.push_back(spec);
        }

        // Reputation
        uint32 repCount;
        PARSE(repCount);

        reputation.reserve(repCount);

        for (uint32 i = 0; i < repCount; i++)
        {
            ReputationInfo rep;
            PARSE(rep.flags, rep.standing);

            reputation.push_back(rep);
        }

        // Achievements
        uint32 completedAchievementsCount, achievementProgressCount;

        PARSE(completedAchievementsCount);

        completedAchievements.reserve(completedAchievementsCount);

        for (uint32 i = 0; i < completedAchievementsCount; i++)
        {
            AchievementInfo info;
            PARSE(info.id);

            achievementProgress.push_back(info);
        }

        PARSE(achievementProgressCount);

        achievementProgress.reserve(achievementProgressCount);

        for (uint32 i = 0; i < achievementProgressCount; i++)
        {
            AchievementInfo info;
            PARSE(info.id, info.progress);

            achievementProgress.push_back(info);
        }

        // Action buttons

        uint32 actionCount;
        PARSE(actionCount);

        actionButtons.reserve(actionCount);

        for (uint32 i = 0; i < actionCount; i++)
        {
            uint32 data;
            PARSE(data);

            actionButtons.push_back(data);
        }

        // Bags
        uint32 bagCount;
        PARSE(bagCount);

        bags.reserve(bagCount);

        for (uint32 i = 0; i < bagCount; i++)
        {
            ItemInfo item;
            PARSE(item.id, item.position);

            bags.push_back(item);
        }

        // Items
        uint32 itemCount;
        PARSE(itemCount);

        items.reserve(itemCount);

        for (uint32 i = 0; i < itemCount; i++)
        {
            ItemInfo item;
            PARSE(item.id, item.count, item.bagIndex, item.position);

            items.push_back(item);
        }

        PARSE(pet.id);

        if (pet.id)
        {
            uint32 talentCount;

            PARSE(pet.level, pet.exp, pet.talentPoints, talentCount);

            pet.talents.reserve(talentCount);

            for (uint32 i = 0; i < talentCount; i++)
            {
                TalentInfo talent;
                PARSE(talent.id, talent.rank);

                pet.talents.push_back(talent);
            }
        }

        return true;
    }

private:
    const char* parseStr()
    {
        char* args = NULL;
        if (_withArgs)
        {
            args = _args;
            _withArgs = false;
        }

        return strtok(args, " ");
    }

    bool parse(uint32& n)
    {
        auto result = parseStr();

        if (!result || !*result)
            return false;

        n = atoi(result);
        return true;
    }

    template<typename... Args>
    bool parse(uint32& n, Args&... args)
    {
        return parse(n) && parse(args...);
    }

    bool parse(float& n)
    {
        auto result = parseStr();

        if (!result || !*result)
            return false;

        n = atof(result);
        return true;
    }

    template<typename... Args>
    bool parse(float& n, Args&... args)
    {
        return parse(n) && parse(args...);
    }

private:
    char* _args;
    bool _withArgs;
};

std::unordered_map<uint32, std::function<void(Player*)>> handlers;
std::unordered_map<uint32, std::pair<WorldSession*, TransferInfo*>> sessions;
EventMap events;

class transfer_commandScript : CommandScript
{
public:
    transfer_commandScript() : CommandScript("transfer_commandScript") { }

    std::vector<ChatCommand> GetCommands() const override
    {
        /*static std::vector<ChatCommand> transferCommandTable =
        {
            { "test", rbac::RBAC_PERM_RA_COMMANDS, true, &HandleTransferTestCommand , "" },
            { "",     rbac::RBAC_PERM_RA_COMMANDS, true, &HandleTransferCommand,      "" },
        };*/

        static std::vector<ChatCommand> commandTable =
        {
            { "transfer", rbac::RBAC_PERM_RA_COMMANDS,  true, &HandleTransferCommand, "" },
        };

        return commandTable;
    }

    static void SetStatus(WorldSession* session, uint32 id, uint32 status, uint32 guid = 0)
    {
        session->KickPlayer();
        CharacterDatabase.PExecute("UPDATE character_transfer SET status = %u, guid = %u WHERE id = %u", status, guid, id);
    }


    static bool CreatePlayer(char const* args, uint32& id)
    {
        if (!args || !*args)
            return false;

        TransferInfo* info = new TransferInfo(args);

        if (!info->parse())
            return false;

        std::string accName;
        if (!AccountMgr::GetName(info->accountId, accName))
        {
            delete info;
            return false;
        }

        info->fillId();
        id = info->id;

        std::string name = "t_";
        name += std::to_string(info->id);

        WorldSession* session = new WorldSession(info->accountId, std::move(accName), nullptr, SEC_PLAYER, 2, 0, LOCALE_enUS, 0, false);
        session->SetFake(true);
        sessions[info->accountId] = std::make_pair(session, info);

        WorldPacket* pkt = new WorldPacket(CMSG_CHAR_CREATE);

        *pkt << name << (uint8)info->race << (uint8)info->pClass
            << (uint8)info->gender << (uint8)info->skin << (uint8)info->face
            << (uint8)info->hairStyle << (uint8)info->hairColor << (uint8)info->facialStyle
            << (uint8)0; // outfit

        session->QueuePacket(pkt);
        session->CatchPacket(SMSG_CHAR_CREATE, [session, info](WorldPacket& pkt)
        {
            uint8 result;
            pkt >> result;

            ResponseCodes code = (ResponseCodes)result;

            if (code == CHAR_CREATE_SUCCESS)
            {
                events.ScheduleEvent(session->GetAccountId(), 5000);
            }
            else
            {
                SetStatus(session, info->id, result);
            }

            return true;
        });

        return true;
    }

    static void LoginPlayer(TransferInfo* info, WorldSession* session)
    {
        WorldPacket* pkt = new WorldPacket(CMSG_PLAYER_LOGIN);
        *pkt << session->LastCreatedCharGuid;

        session->QueuePacket(pkt);

        handlers[session->LastCreatedCharGuid.GetCounter()] = ([info](Player* player)
        {
            UpdatePlayer(player, info);
            SetStatus(player->GetSession(), info->id, CHAR_CREATE_SUCCESS, player->GetGUID().GetCounter());
        });
    }

    static bool UpdatePlayer(Player* player, TransferInfo* info)
    {
        player->SetAtLoginFlag(AtLoginFlags(AT_LOGIN_RENAME | info->flags));

        if (info->level > sWorld->getIntConfig(CONFIG_MAX_PLAYER_LEVEL))
            info->level = sWorld->getIntConfig(CONFIG_MAX_PLAYER_LEVEL);

        player->GiveLevel(info->level);
        player->ModifyMoney(info->money);

        if (info->honorPoints > 0)
            player->SetHonorPoints(info->honorPoints);

        if (info->arenaPoints > 0)
            player->SetArenaPoints(info->arenaPoints);


        for (auto itr = info->skills.begin(); itr != info->skills.end(); itr++)
        {
            uint32 id = PAIR32_LOPART(itr->first), step = PAIR32_HIPART(itr->first);
            uint32 value = PAIR32_LOPART(itr->second), max = PAIR32_HIPART(itr->second);

            player->SetSkill(id, step, value, max);
        }

        player->SetSpecsCount(info->talents.size());

        for (uint32 specId = 0; specId < info->talents.size(); specId++)
        {
            auto& spec = info->talents[specId];
            player->ActivateSpec(specId);

            for (auto itr = spec.talents.begin(); itr != spec.talents.end(); itr++)
            {
                player->LearnTalent(itr->id, itr->rank); // TODO: This is partially NOT WORKING!
            }

            for (uint8 glyphSlot = 0; glyphSlot < spec.glyphs.size(); glyphSlot++)
            {
                if (auto glyph = spec.glyphs[glyphSlot])
                {
                    if (GlyphPropertiesEntry const* gp = sGlyphPropertiesStore.LookupEntry(glyph))
                    {
                        if (GlyphSlotEntry const* gs = sGlyphSlotStore.LookupEntry(player->GetGlyphSlot(glyphSlot)))
                        {
                            if (gp->TypeFlags != gs->TypeFlags)
                                continue;
                        }

                        // remove old glyph
                        if (uint32 oldglyph = player->GetGlyph(glyphSlot))
                        {
                            if (GlyphPropertiesEntry const* old_gp = sGlyphPropertiesStore.LookupEntry(oldglyph))
                            {
                                player->RemoveAurasDueToSpell(old_gp->SpellId);
                                player->SetGlyph(glyphSlot, 0);
                            }
                        }

                        player->CastSpell(player, gp->SpellId, true);
                        player->SetGlyph(glyphSlot, glyph);
                    }
                }
            }
        }

        player->ActivateSpec(info->activeSpec);

        for (auto itr = info->spells.begin(); itr != info->spells.end(); itr++)
        {
            uint32 spellId = *itr;

            if (auto info = sSpellMgr->GetSpellInfo(spellId))
            {
                if (!player->HasSpell(spellId))
                    player->LearnSpell(spellId, false);
            }
        }

        player->TeleportTo(info->mapId, info->x, info->y, info->z, 0);

        for (uint32 i = 0; i < info->reputation.size(); i++)
        {
            auto& rep = info->reputation[i];

            if (rep.standing != 0)
            {
                auto& mgr = player->GetReputationMgr();

                if (auto state = mgr.GetState(i))
                {
                    if (auto entry = sFactionStore.LookupEntry(state->ID))
                    {
                        if (rep.standing != 999999)
                        {
                            mgr.SetOneFactionReputation(entry, rep.standing, true);

                            if (rep.flags & FACTION_FLAG_VISIBLE)
                                mgr.SetVisible(entry);                    // have internal checks for forced invisibility

                            mgr.SetInactive(i, rep.flags & FACTION_FLAG_INACTIVE);

                            if (rep.flags & FACTION_FLAG_AT_WAR)  // DB at war
                                mgr.SetAtWar(i, true);                 // have internal checks for FACTION_FLAG_PEACE_FORCED
                            else                                        // DB not at war
                            {
                                // allow remove if visible (and then not FACTION_FLAG_INVISIBLE_FORCED or FACTION_FLAG_HIDDEN)
                                if (rep.flags & FACTION_FLAG_VISIBLE)
                                    mgr.SetAtWar(i, false);            // have internal checks for FACTION_FLAG_PEACE_FORCED
                            }
                        }
                    }
                }
            }
        }

        for (auto itr = info->completedAchievements.begin(); itr != info->completedAchievements.end(); itr++)
        {
            if (AchievementEntry const* achievementEntry = sAchievementMgr->GetAchievement(itr->id))
                player->CompletedAchievement(achievementEntry);
        }

        // Skip achievement progress

        for (uint32 i = 0; i < info->actionButtons.size(); i++)
        {
            uint32 packed = info->actionButtons[i];

            if (!packed)
                player->removeActionButton(i);
            else
                player->addActionButton(i, ACTION_BUTTON_ACTION(packed), ACTION_BUTTON_TYPE(packed));
        }

        std::vector<Bag*> bags;
        bags.reserve(info->bags.size());

        uint8 bankSlots = player->GetBankBagSlotCount();

        for (auto itr = info->bags.begin(); itr != info->bags.end(); itr++)
        {
            uint8 slot = itr->position;

            if (slot >= BANK_SLOT_BAG_START && slot < BANK_SLOT_BAG_END)
            {
                uint8 slotId = (slot - BANK_SLOT_BAG_START) + 1;

                if (bankSlots < slotId)
                    player->SetBankBagSlotCount(bankSlots = slotId);
            }

            if (auto item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, slot))
            {
                uint32 count = item->GetCount();
                player->DestroyItemCount(item, count, true);
            }

            ItemPosCountVec pos;
            auto err = player->CanStoreNewItem(INVENTORY_SLOT_BAG_0, slot, pos, itr->id, 1);

            if (err == EQUIP_ERR_OK)
            {
                auto bag = player->StoreNewItem(pos, itr->id, true);
                bags.push_back(bag->ToBag());
            }
            else
                TC_LOG_ERROR("misc", "player->CanStoreNewItem: id: %u, bag: %u, slot: %u, count: 1 == %u", itr->id, INVENTORY_SLOT_BAG_0, slot, err);
        }

        std::unordered_map<uint32, uint32> mailItems;
        mailItems.reserve(info->items.size());

        for (auto itr = info->items.begin(); itr != info->items.end(); itr++)
        {
            uint8 itemBag = INVENTORY_SLOT_BAG_0;
            uint8 slot = itr->position;

            if (itr->bagIndex < bags.size())
            {
                if (auto bag = bags[itr->bagIndex])
                    itemBag = bag->GetSlot();
                else
                    TC_LOG_ERROR("misc", "bagindex exists, but no bag created");
            }
            else
                itemBag = INVENTORY_SLOT_BAG_0;

            if (itemBag == INVENTORY_SLOT_BAG_0 && slot != INVENTORY_SLOT_BAG_0)
            {
                if (auto item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, slot))
                {
                    uint32 count = item->GetCount();
                    player->DestroyItemCount(item, count, true);
                }
            }

            if (itemBag == INVENTORY_SLOT_BAG_0 && slot >= EQUIPMENT_SLOT_START && slot < EQUIPMENT_SLOT_END)
            {
                uint16 pos;
                auto err = player->CanEquipNewItem(slot, pos, itr->id, false);

                if (err == EQUIP_ERR_OK)
                    player->EquipNewItem(pos, itr->id, true);
                else
                    AddMailItem(mailItems, itr->id, itr->count);
            }
            else if (slot != INVENTORY_SLOT_BAG_0)
            {
                ItemPosCountVec pos;
                auto err = player->CanStoreNewItem(itemBag, slot, pos, itr->id, itr->count);

                if (err == EQUIP_ERR_OK)
                    player->StoreNewItem(pos, itr->id, true);
                else
                    AddMailItem(mailItems, itr->id, itr->count);
            }
            else
            {
                AddMailItem(mailItems, itr->id, itr->count);
            }
        }

        std::vector<std::pair<uint32, uint32>> mailItemsVec;

        for (auto itr = mailItems.begin(); itr != mailItems.end(); itr++) // Split items by stack count
        {
            if (auto proto = sObjectMgr->GetItemTemplate(itr->first))
            {
                uint32 count = itr->second,
                    perStack = proto->Stackable < 0 ? count : (proto->Stackable == 0 ? 1 : proto->Stackable);

                do
                {
                    uint32 stack = count <= perStack ? count : perStack;

                    mailItemsVec.push_back(std::make_pair(itr->first, stack));
                    count -= stack;
                } while (count > 0);
            }
        }

        if (!mailItemsVec.empty())
        {
            // Send items
            auto trans = CharacterDatabase.BeginTransaction();

            for (auto itr = mailItemsVec.begin(); itr != mailItemsVec.end();)
            {
                MailSender sender(MAIL_NORMAL, 0, MAIL_STATIONERY_GM);
                MailDraft draft("Transfer items", "Your items which can't be added default way");
                uint32 count = 0;

                do
                {
                    if (!player->StoreNewItemInBestSlots(itr->first, itr->second)) // Try add to inventory instead of sending mail
                    {
                        if (Item* item = Item::CreateItem(itr->first, itr->second, player))
                        {
                            item->SaveToDB(trans);
                            draft.AddItem(item);
                        }

                        count++;
                    }
                    
                    itr++;
                } while (itr != mailItemsVec.end() && count < MAX_MAIL_ITEMS);

                if (count > 0)
                    draft.SendMailTo(trans, MailReceiver(player, player->GetGUID().GetCounter()), sender);
            }

            CharacterDatabase.CommitTransaction(trans);
        }

        if (info->pet.id && player->getClass() == CLASS_HUNTER)
        {
            if (auto ct = sObjectMgr->GetCreatureTemplate(info->pet.id))
            {
                if (ct->IsTameable(player->CanTameExoticPets()))
                {
                    Pet* pet = new Pet(player, HUNTER_PET);

                    if (pet->CreateBaseAtTamed(ct, player->GetMap(), player->GetPhaseMask()))
                    {
                        pet->Relocate(player->GetPositionX(), player->GetPositionY(), player->GetPositionZ(), player->GetOrientation());
                        pet->SetDisplayId(ct->Modelid1);

                        if (CreatureFamilyEntry const* cFamily = sCreatureFamilyStore.LookupEntry(ct->family))
                            pet->SetName(cFamily->Name[sWorld->GetDefaultDbcLocale()]);
                        else
                            pet->SetName(ct->Name);

                        if (player->InitTamedPet(pet, info->pet.level, 0))
                        {
                            pet->SetReactState(REACT_DEFENSIVE);
                            pet->GetMap()->AddToMap(pet->ToCreature());
                            pet->resetTalents();
                            player->SetMinion(pet, true);
                            pet->SavePetToDB(PET_SAVE_AS_CURRENT);
                            player->PetSpellInitialize();

                            for (auto itr = info->pet.talents.begin(); itr != info->pet.talents.end(); itr++)
                            {
                                player->LearnPetTalent(pet->GetGUID(), itr->id, itr->rank);
                            }
                        }
                        else
                            delete pet;
                    }
                    else
                        delete pet;
                }
            }


        }

        player->SaveToDB(false);
        return true;
    }

    static bool HandleTransferCommand(ChatHandler* handler, char const* args)
    {
        uint32 id;

        if (!CreatePlayer(args, id))
            return false;

        handler->PSendSysMessage("%u", id);
        return true;
    }

private:
    static void AddMailItem(std::unordered_map<uint32, uint32>& map, uint32 id, uint32 count)
    {
        auto it = map.find(id);

        uint32 addition = 0;

        if (it != map.end())
            addition = it->second;

        map[id] = addition + count;
    }
};

class transfer_worldScript : WorldScript
{
public:
    transfer_worldScript() : WorldScript("transfer_worldScript") { }

//    void OnStartup() override
//    {
//        auto result = CharacterDatabase.Query("SELECT MAX(id) FROM character_transfer");
//
//        if (result)
//            transferId = result->Fetch()->GetUInt32() + 1;
//        else
//            transferId = 1;
//    }
//
//    void OnUpdate(uint32 diff) override
//    {
//        for (auto itr = sessions.begin(); itr != sessions.end();)
//        {
//            ///- and remove not active sessions from the list
//            WorldSession* session = itr->second.first;
//            TransferInfo* info = itr->second.second;
//            WorldSessionFilter updater(session);
//
//            if (!session->Update(diff, updater))
//            {
//                itr = sessions.erase(itr);
//                delete session;
//
//                if (info)
//                    delete info;
//            }
//            else
//                itr++;
//        }
//
//        events.Update(diff);
//
//        while (uint32 accountId = events.ExecuteEvent())
//        {
//            auto itr = sessions.find(accountId);
//
//            if (itr != sessions.end())
//                transfer_commandScript::LoginPlayer(itr->second.second, itr->second.first);
//        }
//    }
};

class transfer_playerScript : PlayerScript
{
public:
    transfer_playerScript() : PlayerScript("transfer_playerScript") { }

//    void OnLogin(Player* player, bool first) override
//    {
//        if (first && player->GetSession()->IsFake())
//        {
//            auto itr = handlers.find(player->GetGUID().GetCounter());
//
//            if (itr != handlers.end())
//            {
//                itr->second(player);
//                handlers.erase(itr);
//            }
//        }
//    }
};

void AddSC_custom_commands()
{
    new transfer_commandScript();
    new transfer_worldScript();
    new transfer_playerScript();
}