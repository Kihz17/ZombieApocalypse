#include "ProjectileImpl.h"

ProjectileImpl::ProjectileImpl(float speed, int damage)
	: speed(speed), collided(false), damage(damage), setOriginalPos(false), originalPos(0.0f, 0.0f, 0.0f), shooterID(0)
{

}

ProjectileImpl::~ProjectileImpl()
{

}