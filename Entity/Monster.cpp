#include "Monster.h"
#include "Utilities.h"
#include "MonsterImpl.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>

Monster::Monster(unsigned int entityID, int health, float moveSpeed, std::string modelName)
	: LivingEntity(entityID, modelName, health), monsterImpl(new MonsterImpl(moveSpeed))
{
	
}

Monster::~Monster()
{
	delete this->monsterImpl;
}

bool Monster::HasTarget() const
{
	return this->monsterImpl->target != NULL;
}

void Monster::Tick(float deltaTime)
{
	if (!HasTarget())
	{
		// Check for target every 2 seconds
		float currentTime = glfwGetTime();
		float lastCheckTime = this->monsterImpl->lastTargetCheckTime;
		if (lastCheckTime == 0.0f || (currentTime - lastCheckTime) >= 2.0f)
		{
			// Ask world (mediator) to find us a target
			sMessage message;
			message.command = "FindSurvivor";
			glm::vec4 data;
			data.x = this->GetEntityID(); // Our id
			message.fData.push_back(data);

			this->receiver->RecieveMessage(message); // Send off to the mediator to find a target
		}
	}

	Entity::Tick(deltaTime);
}

bool Monster::RecieveMessage(sMessage theMessage)
{
	if (theMessage.command == "FoundSurvivor")
	{
		this->monsterImpl->target = static_cast<IEntity*>(theMessage.pData[0]);
		return true;
	}

	std::cout << "Message handler for '" << theMessage.command << "' not found." << std::endl;
	return false;
}

void Monster::MoveTick(float deltaTime) // Move towards target
{
	glm::vec3 position = this->GetPosition();

	if (this->HasTarget())
	{
		glm::vec3 targetPos = this->monsterImpl->target->GetPosition();
		glm::vec3 newDirection = glm::normalize(targetPos - position);

		glm::vec2 eulerAngles = ConvertDirectionToEuler(newDirection);
		this->SetYaw(eulerAngles.x);
		this->SetPitch(eulerAngles.y);

		position += (newDirection * this->monsterImpl->moveSpeed) * deltaTime;
		this->SetPosition(position);
	}
	else // Move around randomly
	{
		float currentYaw = this->GetYaw();
		this->SetYaw(getRandom(currentYaw - 10.0f, currentYaw + 10.0f));

		glm::vec3 direction = this->GetDirection();
		position += (direction * this->monsterImpl->moveSpeed) * deltaTime;

		this->SetPosition(position);
	}
}

void Monster::RemoveTarget(IEntity* entity)
{
	if (this->monsterImpl->target == entity)
	{
		this->monsterImpl->target = NULL;
	}
}