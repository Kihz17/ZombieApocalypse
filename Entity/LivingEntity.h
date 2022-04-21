#pragma once

#include "Entity.h"

class LivingEntity : public Entity
{
public:
	virtual ~LivingEntity();
	virtual bool TakeDamage(int damage);

	virtual bool IsValid();

protected:
	LivingEntity(unsigned int entityID, std::string modelName, int health);

private:
	int health;
};