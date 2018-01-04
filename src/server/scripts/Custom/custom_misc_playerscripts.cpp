#include "ScriptMgr.h"
#include "ObjectMgr.h"
#include "Channel.h"
#include "ChannelMgr.h"
#include "Player.h"
#include "AchievementMgr.h"
#include "ReputationMgr.h"
#include "Chat.h"
#include "RBAC.h"
#include "WorldSession.h"
#include "World.h"
#include "vip_account.h"
#include "GameTime.h"
#include "Pet.h"
#include "SpellHistory.h"
#include "SpellInfo.h"
#include "SpellMgr.h"

#define QUEST_BANG_A_GONG 8743
#define ACHIEVEMENT_SCARAB_LORD 416

class ScarabLord_PlayerScript : PlayerScript
{
public:
    ScarabLord_PlayerScript() : PlayerScript("scarab_lord_playerscript") { }

    void OnQuestStatusChange(Player* player, uint32 questId)
    {
        if (questId == QUEST_BANG_A_GONG && player->GetQuestStatus(questId) == QUEST_STATUS_REWARDED)
        {
            if (auto entry = sAchievementMgr->GetAchievement(ACHIEVEMENT_SCARAB_LORD))
            {
                player->CompletedAchievement(entry);
            }
        }
    }
};

#define QUEST_THE_RECKORNING 13047

class Reckorning_PlayerScript : PlayerScript
{
public:
    Reckorning_PlayerScript() : PlayerScript("the_reckorning_quest_playerScript") { }

    void OnQuestStatusChange(Player* player, uint32 questId) override
    {
        if (questId == QUEST_THE_RECKORNING && player->GetQuestStatus(questId) == QUEST_STATUS_COMPLETE)
        {
            if (auto faction = sFactionStore.LookupEntry(1119))
            {
                player->GetReputationMgr().SetAtWar(faction->reputationListID, false);
            }
        }
    }
};

typedef std::unordered_map<uint32, WorldLocation> RestrictedContainer;

RestrictedContainer restrictedZones;
RestrictedContainer restrictedAreas;

class RestrictedZones_PlayerScript : PlayerScript
{
public:
    RestrictedZones_PlayerScript() : PlayerScript("restricted_zones_playerScript") { }

    bool ProcessPlayer(Player* player, uint32 id, RestrictedContainer& items)
    {
        auto itr = items.find(id);

        if (itr == items.end())
            return false;

        ChatHandler(player->GetSession()).SendSysMessage("This zone is currently restricted!"); // TODO: Localize
        player->TeleportTo(itr->second);
        return true;
    }

    void OnUpdateZone(Player* player, uint32 newZone, uint32 newArea) override
    {
        if (player->IsGameMaster())
            return;

        if (!ProcessPlayer(player, newZone, restrictedZones))
            ProcessPlayer(player, newArea, restrictedAreas);
    }
};

class RestrictedZones_CommandScript : CommandScript
{
public:
    RestrictedZones_CommandScript() : CommandScript("restricted_zones_commandScript") { }

    std::vector<ChatCommand> GetCommands() const override
    {
        static std::vector<ChatCommand> restrictAddCommandTable =
        {
            { "area", rbac::RBAC_PERM_COMMAND_DEBUG_SEND_OPCODE, false, &HandleRestrictAddAreaCommand , "" },
            { "zone", rbac::RBAC_PERM_COMMAND_DEBUG_SEND_OPCODE, false, &HandleRestrictAddZoneCommand , "" },
        };

        static std::vector<ChatCommand> restrictDelCommandTable =
        {
            { "area", rbac::RBAC_PERM_COMMAND_DEBUG_SEND_OPCODE, false, &HandleRestrictDelAreaCommand , "" },
            { "zone", rbac::RBAC_PERM_COMMAND_DEBUG_SEND_OPCODE, false, &HandleRestrictDelZoneCommand , "" },
        };

        static std::vector<ChatCommand> restrictCommandTable =
        {
            { "add", rbac::RBAC_PERM_COMMAND_DEBUG_SEND_OPCODE, false, NULL, "", restrictAddCommandTable },
            { "del", rbac::RBAC_PERM_COMMAND_DEBUG_SEND_OPCODE, false, NULL, "", restrictDelCommandTable },
            { "list", rbac::RBAC_PERM_COMMAND_DEBUG_SEND_OPCODE, false, &HandleRestrictListCommand , "" },
            { "clear", rbac::RBAC_PERM_COMMAND_DEBUG_SEND_OPCODE, false, &HandleRestrictClearCommand , "" },
        };

        static std::vector<ChatCommand> commandTable =
        {
            { "restrict", rbac::RBAC_PERM_COMMAND_DEBUG_SEND_OPCODE,  false, NULL, "", restrictCommandTable },
        };

        return commandTable;
    }

    static bool HandleRestricted(ChatHandler* handler, char const* args, bool isArea, bool add)
    {
        if (!args || !*args || !handler->GetSession()->GetPlayer())
            return false;

        uint32 id = atol(args);

        if (!id)
            return false;

        auto& container = isArea ? restrictedAreas : restrictedZones;
        auto player = handler->GetSession()->GetPlayer();

        if (add)
        {
            if (isArea)
            {
                if (player->GetAreaId() == id)
                {
                    handler->PSendSysMessage("Can't add restricted area because you are in this area"); // TODO: Localize
                    handler->SetSentErrorMessage(true);
                    return false;
                }
            }
            else
            {
                if (player->GetZoneId() == id)
                {
                    handler->PSendSysMessage("Can't add restricted zone because you are in this zone"); // TODO: Localize
                    handler->SetSentErrorMessage(true);
                    return false;
                }
            }

            container[id] = player->GetWorldLocation();
            handler->PSendSysMessage("Restricted %s has beed added. Teleport location is your position", isArea ? "area" : "zone"); // TODO: Localize
        }
        else
        {
            auto itr = container.find(id);

            if (itr == container.end())
                handler->PSendSysMessage("Restricted %s %u is not found", isArea ? "area" : "zone", id); // TODO: Localize
            else
            {
                container.erase(itr);
                handler->PSendSysMessage("Restricted %s %u removed", isArea ? "area" : "zone", id); // TODO: Localize
            }
        }

        return true;
    }

    static bool HandleRestrictAddAreaCommand(ChatHandler* handler, char const* args)
    {
        return HandleRestricted(handler, args, true, true);
    }

    static bool HandleRestrictAddZoneCommand(ChatHandler* handler, char const* args)
    {
        return HandleRestricted(handler, args, false, true);
    }

    static bool HandleRestrictDelAreaCommand(ChatHandler* handler, char const* args)
    {
        return HandleRestricted(handler, args, true, false);
    }

    static bool HandleRestrictDelZoneCommand(ChatHandler* handler, char const* args)
    {
        return HandleRestricted(handler, args, false, false);
    }

    static void PrintList(ChatHandler* handler, const char* name, RestrictedContainer& items)
    {
        handler->PSendSysMessage("Restricted %s:", name); // TODO: Localize

        for (auto itr = items.begin(); itr != items.end(); itr++)
        {
            auto& loc = itr->second;

            handler->PSendSysMessage("%u: map: %u, x: %.2f, y: %.2f, z: %.2f", itr->first, 
                loc.GetMapId(), loc.GetPositionX(), loc.GetPositionY(), loc.GetPositionZ());
        }

        handler->SendSysMessage("\r\n");
    }

    static bool HandleRestrictListCommand(ChatHandler* handler, char const* args)
    {
        PrintList(handler, "areas", restrictedAreas);
        PrintList(handler, "zones", restrictedZones);

        return true;
    }

    static bool HandleRestrictClearCommand(ChatHandler* handler, char const* args)
    {
        restrictedZones.clear();
        restrictedAreas.clear();
        handler->SendSysMessage("Restricted zones/areas cleared"); // TODO: Localize

        return true;
    }
};

class grand_core_honor_system : public PlayerScript
{
public:
    grand_core_honor_system() : PlayerScript("grand_core_honor_system") { }

    void OnPVPKill(Player* killer, Player* killed) override
    {
        if (killer->GetGUID() == killed->GetGUID() || killer->GetMap()->IsBattlegroundOrArena() || killed->GetMap()->IsBattlegroundOrArena())
            return;

        //              ilvl     honor
        static std::map<uint16, int32> const itemLevelHonorStore =
        {
            { 213, 400 },{ 232, 600 },{ 251, 800 },{ 264, 1000 }
        };

        if (killer->GetAreaId() == 616 && killer->GetHonorPoints() >= 100000)
            return;

        int32 honor = 300;
        uint16 itemLevel = static_cast<uint16>(killed->GetAverageItemLevel());

        for (std::map<uint16, int32>::const_iterator itr = itemLevelHonorStore.begin(); itr != itemLevelHonorStore.end(); ++itr)
        {
            if (itemLevel >= itr->first)
                honor = itr->second;
        }

        honor = std::min(honor, int32(killed->GetHonorPoints()));
        killed->ModifyHonorPoints(-honor);
        killer->ModifyHonorPoints(honor);

        std::stringstream forKilled, forKiller;
        forKilled << killer->GetName() << " убил вас и забрал " << honor << " очков чести!";
        forKiller << "Вы убили " << killed->GetName() << " и забрали у " << (killed->getGender() == GENDER_MALE ? "него" : "нее") << " " << honor << " очков чести!";
        ChatHandler(killed->GetSession()).SendSysMessage(forKilled.str().c_str());
        ChatHandler(killer->GetSession()).SendSysMessage(forKiller.str().c_str());
    }
};

class PVP_PlayerScript : public PlayerScript
{
private:
    void AddItem(Player* player, uint32 itemId, uint32 count)
    {
        uint32 noSpaceForCount = 0;
        ItemPosCountVec dest;
        InventoryResult msg = player->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, itemId, count, &noSpaceForCount);
        if (msg != EQUIP_ERR_OK)                               // convert to possible store amount
            count -= noSpaceForCount;

        if (count == 0 || dest.empty())                         // can't add any
            return;

        Item* item = player->StoreNewItem(dest, itemId, true);
        if (count > 0 && item)
            player->SendNewItem(item, count, false, false);
    }

public:
    PVP_PlayerScript() : PlayerScript("pvp_playerscript") { }

    void OnPVPKill(Player* killer, Player* killed) override
    {
        if (!killer->GetMap()->IsBattleArena() || !killer->GetGUID() == killed->GetGUID())
            return;

        uint32 tokenId = sWorld->getIntConfig(CONFIG_PVP_ARENA_TOKEN_ID);
        uint32 tokenCount = sWorld->getIntConfig(CONFIG_PVP_ARENA_TOKEN_COUNT);

        if (tokenId && tokenCount)
        {
            if (IsVip(killer))
                tokenCount *= 2; // Config this?

            AddItem(killer, tokenId, tokenCount);
        }
    }
};

class duel_zone_PlayerScript : public PlayerScript
{
public:
    duel_zone_PlayerScript() : PlayerScript("duel_zone") { }

    void OnDuelEnd(Player* p1, Player* p2, DuelCompleteType type)
    {
        if (p1->GetZoneId() == 215)
            ResetSpellCooldowns(p1);

        if (p2->GetZoneId() == 215)
            ResetSpellCooldowns(p2);
    }

    static void ResetSpellCooldowns(Player* player)
    {
        // remove cooldowns on spells that have < 10 min CD > 30 sec and has no onHold
        player->GetSpellHistory()->ResetCooldowns([player](SpellHistory::CooldownStorageType::iterator itr) -> bool
        {
            SpellInfo const* spellInfo = sSpellMgr->AssertSpellInfo(itr->first);
            uint32 remainingCooldown = player->GetSpellHistory()->GetRemainingCooldown(spellInfo);
            int32 totalCooldown = spellInfo->RecoveryTime;
            int32 categoryCooldown = spellInfo->CategoryRecoveryTime;

            player->ApplySpellMod(spellInfo->Id, SPELLMOD_COOLDOWN, totalCooldown, nullptr);

            if (int32 cooldownMod = player->GetTotalAuraModifier(SPELL_AURA_MOD_COOLDOWN))
                totalCooldown += cooldownMod * IN_MILLISECONDS;

            if (!spellInfo->HasAttribute(SPELL_ATTR6_IGNORE_CATEGORY_COOLDOWN_MODS))
                player->ApplySpellMod(spellInfo->Id, SPELLMOD_COOLDOWN, categoryCooldown, nullptr);

            return remainingCooldown > 0
                && !itr->second.OnHold
                && Milliseconds(totalCooldown) < Minutes(10)
                && Milliseconds(categoryCooldown) < Minutes(10)
                && Milliseconds(remainingCooldown) < Minutes(10);
        }, true);

        // pet cooldowns
        if (Pet* pet = player->GetPet())
            pet->GetSpellHistory()->ResetAllCooldowns();
    }
};

void AddSC_custom_misc_playerscripts()
{
    new ScarabLord_PlayerScript();
    new Reckorning_PlayerScript();
    new RestrictedZones_PlayerScript();
    new RestrictedZones_CommandScript();
    new grand_core_honor_system();
    new PVP_PlayerScript();
    new duel_zone_PlayerScript();
}
