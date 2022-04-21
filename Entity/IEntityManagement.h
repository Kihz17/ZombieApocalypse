#pragma once

#include "IEntity.h"

class Zombie;
class Skeleton;
class Survivor;
class Projectile;
class IEntityManagement
{
public:
	virtual ~IEntityManagement()
	{

	}

	virtual void AddZombie(Zombie* zombie) = 0;
	virtual void AddSkeleton(Skeleton* skeleton) = 0;
	virtual void AddSurvivor(Survivor* survivor) = 0;
	virtual void AddProjectile(Projectile* projectile) = 0;
};