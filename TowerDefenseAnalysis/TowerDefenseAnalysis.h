#pragma once
#include "InputTypes.h"
#include "InternalTypes.h"

AlienCount TowerDefenseAnalysis(const Battlefield&, const Turrets&, const IncomingWaves&);
void RunTD(const Battlefield&, const Turrets&, const IncomingWaves&);
