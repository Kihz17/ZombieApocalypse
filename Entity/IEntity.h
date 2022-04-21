#pragma once

#include <glm/vec3.hpp>
#include <string>

class IEntity
{
public:
	virtual ~IEntity();

	// Called every fram to update the entity
	virtual void Tick(float deltaTime) = 0;

	// Get the direciton the entity is facing as a vector
	virtual glm::vec3 GetDirection() const = 0;

	// Get the entity's current positon
	virtual glm::vec3 GetPosition() const = 0;

	// Get the entity's current yaw
	virtual float GetYaw() const = 0;

	// Get the entity's current pitch
	virtual float GetPitch() const = 0;

	// Get the entity's model to draw 
	virtual std::string GetModelName() const = 0;

	// Get the enitity's ID
	virtual unsigned int GetEntityID() const = 0;

	virtual float GetRadius() = 0;

	virtual void SetPosition(glm::vec3 position) = 0;
	virtual void SetYaw(float yaw) = 0;
	virtual void SetPitch(float pitch) = 0;

	virtual glm::vec3 GetScale() = 0;

	// Is this entity valid?
	virtual bool IsValid() = 0;
};