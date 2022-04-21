#include "Entity.h"
#include "EntityImpl.h"

#include <glm/glm.hpp>

Entity::Entity(unsigned int entityID, std::string modelName)
	: impl(new EntityImpl(entityID, modelName))
{
	this->receiver = NULL;
}

Entity::~Entity()
{
	delete this->impl;
}

glm::vec3 Entity::GetDirection() const
{
	glm::vec3 direction;
	float xz = cos(glm::radians(this->impl->pitch));
	direction.x = -xz * sin(glm::radians(this->impl->yaw));
	direction.y = -sin(glm::radians(this->impl->pitch));
	direction.z = xz * cos(glm::radians(this->impl->yaw));
	glm::normalize(direction);
	return direction;
}

glm::vec3 Entity::GetPosition() const
{
	return this->impl->position;
}

float Entity::GetYaw() const
{
	return this->impl->yaw;
}

float Entity::GetPitch() const
{
	return this->impl->pitch;
}

std::string Entity::GetModelName() const
{
	return this->impl->modelName;
}

unsigned int Entity::GetEntityID() const
{
	return this->impl->entityID;
}

void Entity::Tick(float deltaTime)
{
	this->MoveTick(deltaTime);
}

bool Entity::IsValid()
{
	return true;
}

void Entity::SetPosition(glm::vec3 position)
{
	this->impl->position = position;
}

void Entity::SetYaw(float yaw)
{
	this->impl->yaw = yaw;
}

void Entity::SetPitch(float pitch)
{
	this->impl->pitch = pitch;
}

bool Entity::RecieveMessage(sMessage theMessage)
{
	return true;
}

bool Entity::SetReciever(IMessage* pTheReciever)
{
	this->receiver = pTheReciever;
	return true;
}

bool Entity::RecieveMessage(sMessage theMessage, sMessage& messageReply)
{
	return true;
}

float Entity::GetRadius()
{
	return 1.0f;
}

glm::vec3 Entity::GetScale()
{
	return glm::vec3(0.5f, 0.5f, 0.5f);
}