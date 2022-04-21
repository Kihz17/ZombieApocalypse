#pragma once

#include "LivingEntity.h"

class MonsterImpl;
class Monster : public LivingEntity
{
public:
	virtual ~Monster();

	virtual void Tick(float deltaTime);

	virtual void MoveTick(float deltaTime);

	virtual bool HasTarget() const;

	virtual bool RecieveMessage(sMessage theMessage);

	virtual void RemoveTarget(IEntity* entity);

protected:
	Monster(unsigned int entityID, int health, float moveSpeed, std::string modelName);

private:
	MonsterImpl* monsterImpl;
};