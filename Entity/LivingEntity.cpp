#include "LivingEntity.h"

#include <iostream>

LivingEntity::LivingEntity(unsigned int entityID, std::string modelName, int health)
	: Entity(entityID, modelName), health(health)
{

}

LivingEntity::~LivingEntity()
{

}

bool LivingEntity::TakeDamage(int damage)
{
	this->health = std::max(0, this->health - damage);
	return true;
}

bool LivingEntity::IsValid()
{
	return this->health > 0;
}