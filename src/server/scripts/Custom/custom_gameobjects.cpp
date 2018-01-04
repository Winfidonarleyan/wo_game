#include "ScriptMgr.h"
#include "GameObject.h"
#include "GameObjectAI.h"
#include "GameTime.h"
#include "BattlefieldMgr.h"
#include "ScriptMgr.h"
#include "InstanceScript.h"
#include "Map.h"
#include "MotionMaster.h"
#include "Player.h"
#include "ScriptedCreature.h"
#include "TemporarySummon.h"

class gurubashi_chest_GameObjectScript : GameObjectScript
{
public:
    gurubashi_chest_GameObjectScript() : GameObjectScript("gurubashi_chest") { }

    class gurubashi_chest_AI : public GameObjectAI
    {
    public:
        gurubashi_chest_AI(GameObject* go) : GameObjectAI(go), _lastUpdateTime(0) { }

        void Reset() override
        {
            me->SetPhaseMask(2147483648, true);
            _events.RescheduleEvent(1, 1000);
        }

        void UpdateAI(uint32 diff) override
        {
            if (!me)
                return;

            _events.Update(diff);

            if (_events.ExecuteEvent())
            {
                const auto timestamp = GameTime::GetGameTime();
                auto timeStruct = *localtime(&timestamp);

                if (correctHour(timeStruct.tm_hour) && _lastUpdateTime != timeStruct.tm_hour)
                {
                    _lastUpdateTime = timeStruct.tm_hour;
                    me->SetPhaseMask(1, true);
                }

                _events.ScheduleEvent(1, 1000);
            }
        }

        void OnLootStateChanged(uint32 state, Unit* who) override
        {
            if (state == 3)
            {
                me->SetRespawnTime(3);
            }
        }

        bool correctHour(uint32 hour)
        {
            return hour == 3 || hour == 6 || hour == 9 || hour == 12 || hour == 15 || hour == 18 || hour == 21;
        }

    private:
        uint32 _lastUpdateTime;
        EventMap _events;
    };

    GameObjectAI* GetAI(GameObject* go) const override
    {
        return new gurubashi_chest_AI(go);
    }
};

#define PHASEMASK 2147483648

class wintergrasp_dalaran_portal : GameObjectScript 
{
public:
    wintergrasp_dalaran_portal() : GameObjectScript("wintergrasp_dalaran_portal") { }

    class wintergrasp_dalaran_portalAI : public GameObjectAI
    {
    public:
        wintergrasp_dalaran_portalAI(GameObject* go) : GameObjectAI(go)
        {
            _team = TEAM_NEUTRAL;
        }
        
        void Reset() override
        {
            _events.Reset();
            _events.ScheduleEvent(1, 1000);
        }

        void UpdateAI(uint32 diff) override
        {
            if (!me)
                return;

            _events.Update(diff);

            if (_events.ExecuteEvent())
            {
                if (_team == TEAM_NEUTRAL)
                {
                    if (me && me->FindNearestCreature(32169, 20))
                        _team = TEAM_ALLIANCE;
                    else if (me && me->FindNearestCreature(32170, 20))
                        _team = TEAM_HORDE;
                }

                if (_team != TEAM_NEUTRAL)
                {
                    uint32 phaseMask = PHASEMASK;

                    if (auto wg = sBattlefieldMgr->GetBattlefieldByBattleId(BATTLEFIELD_BATTLEID_WG))
                    {
                        if (wg->GetDefenderTeam() == _team)
                            phaseMask = 1;
                    }

                    if (me && (me->GetPhaseMask() != phaseMask))
                        me->SetPhaseMask(phaseMask, true);
                }

                _events.ScheduleEvent(1, 1000);
            }
        }

    private:
        TeamId _team;
        EventMap _events;
    };

    GameObjectAI* GetAI(GameObject* go) const override
    {
        return new wintergrasp_dalaran_portalAI(go);
    }
};

void AddSC_custom_gameobjects()
{
    new gurubashi_chest_GameObjectScript();
    new wintergrasp_dalaran_portal();
}