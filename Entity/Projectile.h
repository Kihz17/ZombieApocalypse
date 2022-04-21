#pragma once

#include "Entity.h"

class ProjectileImpl;
class Projectile : public Entity
{
public:
	Projectile(unsigned int entityID, std::string modelName, float speed, int damage);
	virtual ~Projectile();

	virtual void Tick(float deltaTime);

	virtual void MoveTick(float deltaTime);

	virtual bool IsValid();

	virtual void OnCollide();

	virtual float GetDamage() const;

	virtual void SetPosition(glm::vec3 position);

	virtual unsigned int GetShooterID() const;
	virtual void SetShooterID(unsigned int id); // So the projectile doesn't collide with its shooter
private:
	ProjectileImpl* projectileImpl;
};