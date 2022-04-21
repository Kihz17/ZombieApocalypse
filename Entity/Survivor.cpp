#include "Survivor.h"
#include "SurvivorImpl.h"
#include "Utilities.h"
#include "Monster.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>

Survivor::Survivor(unsigned int entityID, int health)
	: LivingEntity(entityID, "survivor", health), survivorImpl(new SurvivorImpl())
{

}

Survivor::~Survivor()
{

}

void Survivor::Tick(float deltaTime)
{
	if (this->survivorImpl->target == NULL)
	{
		// Check for target every 2 seconds
		float currentTime = glfwGetTime();
		float lastCheckTime = this->survivorImpl->lastTargetCheckTime;
		if (lastCheckTime == 0.0f || (currentTime - lastCheckTime) >= 2.0f)
		{
			this->survivorImpl->lastTargetCheckTime = currentTime;
			// Ask world (mediator) to find us a target
			sMessage message;
			message.command = "FindMonster";
			glm::vec4 data;
			data.x = this->GetEntityID(); // Our id
			message.fData.push_back(data);

			this->receiver->RecieveMessage(message); // Send off to the mediator to find a target
		}
	}
	else // We have a target, shoot at it
	{
		float currentTime = glfwGetTime();
		float lastShootTime = this->survivorImpl->lastShootTime;
		if (lastShootTime == 0.0f || (currentTime - lastShootTime) >= 1.0f) // Shoot projectiles every second
		{
			this->survivorImpl->lastShootTime = currentTime;
			glm::vec3 targetPos = this->survivorImpl->target->GetPosition();
			glm::vec3 direction = glm::normalize(targetPos - this->GetPosition());

			// Look in the direction we are shooting
			glm::vec2 eulerAngles = ConvertDirectionToEuler(direction);
			this->SetYaw(eulerAngles.x);
			this->SetYaw(eulerAngles.y);

			sMessage message;
			message.command = "FireProjectile";
			message.data.push_back("bullet"); // Tells mediator what type of projectile we are shooting
			message.fData.push_back(glm::vec4(eulerAngles.x, eulerAngles.y, this->GetEntityID(), 1.0f)); // Tells what direction to shoot in
			message.fData.push_back(glm::vec4(this->GetPosition(), 1.0f)); // Tells what start position is

			this->receiver->RecieveMessage(message);
		}
	}
}

void Survivor::MoveTick(float deltaTime)
{

}

bool Survivor::RecieveMessage(sMessage theMessage)
{
	if (theMessage.command == "FoundMonster")
	{
		this->survivorImpl->target = static_cast<Monster*>(theMessage.pData[0]);
		return true;
	}

	std::cout << "Message handler for '" << theMessage.command << "' not found." << std::endl;
	return false;
}

void Survivor::RemoveTarget(IEntity* entity)
{
	if (this->survivorImpl->target == entity)
	{
		this->survivorImpl->target = NULL;
	}
}