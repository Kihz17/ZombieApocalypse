#include "Projectile.h"
#include "ProjectileImpl.h"

#include <glm/glm.hpp>
#include <iostream>

Projectile::Projectile(unsigned int entityID, std::string modelName, float speed, int damage)
	: Entity(entityID, modelName), projectileImpl(new ProjectileImpl(speed, damage))
{

}

Projectile::~Projectile()
{
	delete this->projectileImpl;
}

void Projectile::Tick(float deltaTime)
{
	Entity::Tick(deltaTime);
}

void Projectile::MoveTick(float deltaTime)
{
	glm::vec3 position = this->GetPosition();
	position += (this->GetDirection() * this->projectileImpl->speed) * deltaTime;
	this->SetPosition(position);
}

void Projectile::OnCollide()
{
	
}

bool Projectile::IsValid()
{
	float distance = glm::distance(this->projectileImpl->originalPos, this->GetPosition());

	return distance <= 200 && !this->projectileImpl->collided;
}

float Projectile::GetDamage() const
{
	return this->projectileImpl->damage;
}

void Projectile::SetPosition(glm::vec3 position)
{
	Entity::SetPosition(position);
	if (!this->projectileImpl->setOriginalPos)
	{
		this->projectileImpl->setOriginalPos = true;
		this->projectileImpl->originalPos = position;
	}
}

void Projectile::SetShooterID(unsigned int id)
{
	this->projectileImpl->shooterID = id;
}

unsigned int Projectile::GetShooterID() const
{
	return this->projectileImpl->shooterID;
}