#pragma once

#include "IEntity.h"

class MonsterImpl
{
public:
	MonsterImpl(float moveSpeed);
	virtual ~MonsterImpl();

	float moveSpeed;
	IEntity* target;
	float lastTargetCheckTime;
};