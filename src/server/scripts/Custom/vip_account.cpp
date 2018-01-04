#include "vip_account.h"
#include "GameTime.h"
#include "DatabaseEnv.h"
#include "Channel.h"
#include "WorldSession.h"
#include "Arena.h"
#include "Chat.h"
#include "RBAC.h"
#include "SpellMgr.h"
#include "World.h"

typedef std::unordered_map<uint32, uint32> VipMap;

static VipMap vipMap;

bool IsVip(Player* player)
{
    auto itr = vipMap.find(player->GetSession()->GetAccountId());

    if (itr == vipMap.end())
        return false;

    return GameTime::GetGameTime() <= itr->second;
}

bool IsVip(ObjectGuid guid)
{
    if (auto player = ObjectAccessor::FindPlayer(guid))
        return IsVip(player);

    auto result = CharacterDatabase.PQuery("SELECT account FROM characters WHERE guid = %u", guid.GetCounter());

    if (!result)
        return false;

    uint32 accountId = result->Fetch()->GetUInt32();
    result = LoginDatabase.PQuery("SELECT expires FROM account_vip WHERE id = %u", accountId);

    if (!result)
        return false;

    return result->Fetch()->GetUInt32() > GameTime::GetGameTime();
}

class VipAccount_PlayerScript : PlayerScript
{

public:
    VipAccount_PlayerScript() : PlayerScript("vip_account") { }

    void OnLogin(Player* player, bool /* firstLogin */) override
    {
        auto accountId = player->GetSession()->GetAccountId();
        auto result = LoginDatabase.PQuery("SELECT expires FROM account_vip WHERE id = %u", accountId);

        if (result)
        {
            auto fields = result->Fetch();
            auto expires = fields[0].GetUInt32();

            if (expires > GameTime::GetGameTime())
                vipMap[accountId] = expires;
        }
    }

    /*void OnLogout(Player* player) override
    {
    auto itr = vipMap.find(player->GetSession()->GetAccountId());

    if (itr != vipMap.end())
    vipMap.erase(itr);
    }*/

    void OnGiveXP(Player* player, uint32& amount, Unit*) override
    {
        if (!IsVip(player))
            return;

        auto rate = sWorld->getRate(RATE_VIP_XP);

        if (rate > 0.01f)
            amount *= rate;
    }

    void OnGiveHonorPoints(Player* player, float& points, float& bonusPoints, Unit*) override
    {
        if (!IsVip(player) || points < 0.01f)
            return;

        auto rate = sWorld->getRate(RATE_VIP_HONOR_POINTS);

        if (rate > 0.01f)
            bonusPoints += points * rate;
    }

    void OnGiveArenaPoints(Player* player, float& points, float& bonusPoints) override
    {
        if (!IsVip(player))
            return;

        bonusPoints = points * 2; // Config this?
    }

    void OnGiveArenaPoints(ObjectGuid guid, float& points, float& bonusPoints) override
    {
        if (!IsVip(guid))
            return;

        bonusPoints = points * 2; // Config this?
    }

    void OnReputationChange(Player* player, uint32 /*factionId*/, int32& standing, bool incremental) override
    {
        if (!IsVip(player) || !incremental)
            return;

        auto rate = sWorld->getRate(RATE_VIP_REPUTATION);

        if (rate >= 0.01f)
            standing *= rate;
    }
};

class vip_account_CommandScript : CommandScript
{
public:
    vip_account_CommandScript() : CommandScript("vip_account_commandScript") { }

    std::vector<ChatCommand> GetCommands() const override
    {
        static std::vector<ChatCommand> vipCommandTable =
        {
            { "bank",   rbac::RBAC_PERM_COMMAND_VIP_BANK, false, &HandleVipBankCommand,   "" },
            { "home",   rbac::RBAC_PERM_COMMAND_VIP_HOME, false, &HandleVipHomeCommand,   "" },
            { "debuff", rbac::RBAC_PERM_COMMAND_VIP_DEBUFF, false, &HandleVipDebuffCommand, "" },
        };

        static std::vector<ChatCommand> commandTable =
        {
            { "vip", rbac::RBAC_PERM_COMMAND_VIP,  false, NULL, "", vipCommandTable },
        };

        return commandTable;
    }

    static bool HandleVipBankCommand(ChatHandler* handler, char const* /*args*/)
    {
        if (auto session = handler->GetSession())
        {
            if (auto player = session->GetPlayer())
            {
                if (IsVip(player))
                {
                    session->SendShowBank(player->GetGUID());
                    return true;
                }
            }
        }

        return false;
    }

    static bool HandleVipHomeCommand(ChatHandler* handler, char const* /*args*/)
    {
        if (auto session = handler->GetSession())
        {
            if (auto player = session->GetPlayer())
            {
                if (IsVip(player))
                {
                    player->CastSpell(player, 54318, TriggerCastFlags(TRIGGERED_NONE));
                    return true;
                }
            }
        }

        return false;
    }

    static bool HandleVipDebuffCommand(ChatHandler* handler, char const* /*args*/)
    {
        if (auto session = handler->GetSession())
        {
            if (auto player = session->GetPlayer())
            {
                if (IsVip(player))
                {
                    player->RemoveAurasDueToSpell(26013); // Deserter
                    player->RemoveAurasDueToSpell(15007); // Sickness
                    return true;
                }
            }
        }

        return false;
    }
};

// INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES ('54318', 'spell_vip_hearthstone')
class vip_hearthstone_SpellScriptLoader : SpellScriptLoader
{
public:
    vip_hearthstone_SpellScriptLoader() : SpellScriptLoader("spell_vip_hearthstone") { }

    class vip_hearthstone_SpellScript : public SpellScript
    {
    public:

        PrepareSpellScript(vip_hearthstone_SpellScript);

        bool Validate(SpellInfo const*) override
        {
            return sSpellMgr->GetSpellInfo(54318) != nullptr;
        }

        void Handle(SpellEffIndex)
        {
            if (auto caster = GetCaster())
            {
                if (auto player = caster->ToPlayer())
                {
                    if (IsVip(player))
                        player->TeleportTo(player->m_homebindMapId, player->m_homebindX, player->m_homebindY, player->m_homebindZ, player->GetOrientation());
                }
            }
        }

        void Register() override
        {
            OnEffectHit += SpellEffectFn(vip_hearthstone_SpellScript::Handle, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new vip_hearthstone_SpellScript();
    }
};

void AddSC_vip_account()
{
    new VipAccount_PlayerScript();
    new vip_account_CommandScript();
    new vip_hearthstone_SpellScriptLoader();
}
