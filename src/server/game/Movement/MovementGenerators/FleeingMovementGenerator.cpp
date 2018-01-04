/*
 * Copyright (C) 2008-2017 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "VMapFactory.h"
#include "CreatureAI.h"
#include "ObjectAccessor.h"
#include "Creature.h"
#include "Player.h"
#include "PathGenerator.h"
#include "MoveSplineInit.h"
#include "MoveSpline.h"
#include "FleeingMovementGenerator.h"

#define MIN_QUIET_DISTANCE 25.0f
#define MAX_QUIET_DISTANCE 40.0f

template<class T>
FleeingMovementGenerator<T>::~FleeingMovementGenerator()
{
    delete _path;
}

template<class T>
void FleeingMovementGenerator<T>::DoInitialize(T* owner)
{
    if (!owner)
        return;

    owner->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_FLEEING);
    owner->AddUnitState(UNIT_STATE_FLEEING);
    owner->StopMoving();
}

template<class T>
void FleeingMovementGenerator<T>::DoFinalize(T *)
{
}

template<>
void FleeingMovementGenerator<Player>::DoFinalize(Player* owner)
{
    owner->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_FLEEING);
    owner->ClearUnitState(UNIT_STATE_FLEEING);
    owner->StopMoving();
    owner->DisableSpline();
}

template<>
void FleeingMovementGenerator<Creature>::DoFinalize(Creature* owner)
{
    owner->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_FLEEING);
    owner->ClearUnitState(UNIT_STATE_FLEEING | UNIT_STATE_FLEEING_MOVE);
    if (owner->GetVictim())
        owner->SetTarget(owner->EnsureVictim()->GetGUID());
}

template<class T>
void FleeingMovementGenerator<T>::DoReset(T* owner)
{
    DoInitialize(owner);
}

template<class T>
bool FleeingMovementGenerator<T>::DoUpdate(T* owner, uint32 diff)
{
    if (!owner || !owner->IsAlive())
        return false;
    
    if (owner->HasUnitState(UNIT_STATE_ROOT | UNIT_STATE_STUNNED))
    {
        owner->ClearUnitState(UNIT_STATE_FLEEING_MOVE);
        return true;
    }

    _timer.Update(diff);
    SetTargetLocation(owner, diff);

    return true;
}

template<class T>
void FleeingMovementGenerator<T>::SetTargetLocation(T* owner, uint32 time_diff)
{
    if (!owner)
        return;

    if (owner->HasUnitState(UNIT_STATE_ROOT | UNIT_STATE_STUNNED))
        return;

    bool checkMove = false;
    if (_timer.Passed())
    {
        if (!owner->HasUnitState(UNIT_STATE_FLEEING_MOVE))
            owner->AddUnitState(UNIT_STATE_FLEEING_MOVE);

        if (owner->movespline->Finalized())
            _timer.Reset(urand(800, 1500));
        return;
    }
    else
    {
        _nextMoveTime.Update(time_diff);
        checkMove = _nextMoveTime.Passed(); // check next move
    }
    
    if (!checkMove)
        return;

    Position destination = owner->GetPosition();
    GetPoint(owner, destination);

    // Add LOS check for target point
    Position currentPosition = owner->GetPosition();
    if (!VMAP::VMapFactory::createOrGetVMapManager()->isInLineOfSight(owner->GetMapId(), currentPosition.m_positionX, currentPosition.m_positionY, currentPosition.m_positionZ + 1.0f, destination.GetPositionX(), destination.GetPositionY(), destination.GetPositionZ() + 1.0f, VMAP::ModelIgnoreFlags::Nothing))
    {
        _timer.Reset(200);
        return;
    }

    if (!_path)
        _path = new PathGenerator(owner);

    _path->SetPathLengthLimit(30.0f);
    bool result = _path->CalculatePath(destination.GetPositionX(), destination.GetPositionY(), destination.GetPositionZ());
    if (!result || (_path->GetPathType() & PATHFIND_NOPATH))
    {
        _timer.Reset(100);
        return;
    }

    Movement::MoveSplineInit init(owner);
    init.MovebyPath(_path->GetPath());
    init.SetWalk(false);
    int32 traveltime = init.Launch();
    //_timer.Reset(traveltime + urand(800, 1500));
    _nextMoveTime.Reset(urand(500, 700));
    
    owner->UpdateSpeed(MOVE_RUN);
    G3D::Vector3 dest = _path->GetActualEndPosition();
    owner->UpdatePosition(dest.x, dest.y, dest.z, owner->GetOrientation());
}

template<class T>
void FleeingMovementGenerator<T>::GetPoint(T* owner, Position &position)
{
    float casterDistance, casterAngle;
    if (Unit* fleeTarget = ObjectAccessor::GetUnit(*owner, _fleeTargetGUID))
    {
        casterDistance = fleeTarget->GetDistance(owner);
        if (casterDistance > 0.2f)
            casterAngle = fleeTarget->GetAngle(owner);
        else
            casterAngle = frand(0.0f, 2.0f * float(M_PI));
    }
    else
    {
        casterDistance = 0.0f;
        casterAngle = frand(0.0f, 2.0f * float(M_PI));
    }

    float distance, angle;
    if (casterDistance < MIN_QUIET_DISTANCE)
    {
        distance = frand(0.4f, 1.3f) * (MIN_QUIET_DISTANCE - casterDistance);
        angle = casterAngle + frand(-float(M_PI) / 8.0f, float(M_PI) / 8.0f);
    }
    else if (casterDistance > MAX_QUIET_DISTANCE)
    {
        distance = frand(0.4f, 1.0f) * (MAX_QUIET_DISTANCE - MIN_QUIET_DISTANCE);
        angle = -casterAngle + frand(-float(M_PI) / 4.0f, float(M_PI) / 4.0f);
    }
    else    // we are inside quiet range
    {
        distance = frand(0.6f, 1.2f) * (MAX_QUIET_DISTANCE - MIN_QUIET_DISTANCE);
        angle = frand(0.0f, 2.0f * float(M_PI));
    }

    owner->MovePositionToFirstCollision(position, distance, angle);
}

template FleeingMovementGenerator<Player>::~FleeingMovementGenerator();
template FleeingMovementGenerator<Creature>::~FleeingMovementGenerator();
template void FleeingMovementGenerator<Player>::DoInitialize(Player*);
template void FleeingMovementGenerator<Creature>::DoInitialize(Creature*);
template void FleeingMovementGenerator<Player>::DoReset(Player*);
template void FleeingMovementGenerator<Creature>::DoReset(Creature*);
template bool FleeingMovementGenerator<Player>::DoUpdate(Player*, uint32);
template bool FleeingMovementGenerator<Creature>::DoUpdate(Creature*, uint32);
template void FleeingMovementGenerator<Player>::SetTargetLocation(Player*, uint32);
template void FleeingMovementGenerator<Creature>::SetTargetLocation(Creature*, uint32);
template void FleeingMovementGenerator<Player>::GetPoint(Player*, Position &);
template void FleeingMovementGenerator<Creature>::GetPoint(Creature*, Position &);

//---- TimedFleeingMovementGenerator

bool TimedFleeingMovementGenerator::Update(Unit* owner, uint32 time_diff)
{
    if (!owner->IsAlive())
        return false;

    _totalFleeTime.Update(time_diff);
    if (_totalFleeTime.Passed())
        return false;

    // This calls grant-parent Update method hiden by FleeingMovementGenerator::Update(Creature &, uint32) version
    // This is done instead of casting Unit& to Creature& and call parent method, then we can use Unit directly
    return MovementGeneratorMedium< Creature, FleeingMovementGenerator<Creature> >::Update(owner, time_diff);
}

void TimedFleeingMovementGenerator::Finalize(Unit* owner)
{
    owner->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_FLEEING);
    owner->ClearUnitState(UNIT_STATE_FLEEING);
    owner->StopMoving();
    if (Unit* victim = owner->GetVictim())
    {
        if (owner->IsAlive())
        {
            owner->AttackStop();
            owner->ToCreature()->AI()->AttackStart(victim);
        }
    }
}
