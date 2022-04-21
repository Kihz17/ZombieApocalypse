#pragma once

#include "Monster.h"

class SurvivorImpl
{
public:
	SurvivorImpl();
	~SurvivorImpl();

	Monster* target;
	float lastTargetCheckTime;
	float lastShootTime;
};