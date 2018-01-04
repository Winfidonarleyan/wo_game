#ifndef __PVP_ZONE_H__
#define __PVP_ZONE_H__

#include "Player.h"

bool isPVPZone(uint32 zoneId);
bool isInPVPZone(Player* player);
void teleportToRandomLocation(Player* player);

#endif
