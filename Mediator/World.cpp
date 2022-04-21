#include "World.h"
#include "ModelManager.h"
#include "EntityFactory.h"
#include "Skeleton.h"
#include "Zombie.h"
#include "Survivor.h"
#include "Projectile.h"

#include <glm/glm.hpp>
#include <iostream>

World::World()
{
	this->receiver = NULL;
}

World::~World()
{
	std::map<unsigned int, IEntity*>::iterator it;
	for (it = this->entityMap.begin(); it != this->entityMap.end(); it++)
	{
		delete it->second;
	}
}

void World::Tick(float deltaTime)
{
	std::vector<unsigned int> invalidEntities;
	std::map<unsigned int, IEntity*>::iterator it;
	for (it = this->entityMap.begin(); it != this->entityMap.end(); it++)
	{
		it->second->Tick(deltaTime);
		if (!it->second->IsValid())
		{
			invalidEntities.push_back(it->first);
		}
	}

	RemoveEntities(invalidEntities);
	invalidEntities.clear();

	std::map<unsigned int, Projectile*>::iterator projectileIt;
	for (projectileIt = this->projectiles.begin(); projectileIt != this->projectiles.end(); projectileIt++)
	{
		for (it = this->entityMap.begin(); it != this->entityMap.end(); it++)
		{
			if (projectileIt->second->GetShooterID() == it->second->GetEntityID())
			{
				continue; // This is our shooter, don't collide with them
			}

			float distance = glm::distance(projectileIt->second->GetPosition(), it->second->GetPosition());

			if (distance < (it->second->GetRadius() + projectileIt->second->GetRadius())) // We have collided
			{
				if (zombies.find(it->first) != zombies.end() || skeletons.find(it->first) != skeletons.end()) // The entity is living, deal damage
				{
					LivingEntity* le = static_cast<LivingEntity*>(it->second);
					le->TakeDamage(projectileIt->second->GetDamage());
				}

				projectileIt->second->OnCollide();

				if (!it->second->IsValid()) // Check if the entity we collided with is still valid
				{
					invalidEntities.push_back(it->first);
				}

				if (!projectileIt->second->IsValid()) // Check if the projectile is still valid
				{
					invalidEntities.push_back(projectileIt->first);
					break; // No longer valid, stop loopign for this projectile
				}
			}
		}
	}

	RemoveEntities(invalidEntities);
}

IEntity* World::MakeEntity(std::string entityToMake)
{
	IEntity* entity = NULL;
	if (entityToMake == "skeleton")
	{
		entity = EntityFactory::GetInstance()->MakeEntity("skeleton");

		// Set the entity's receiver to this world.
		Skeleton* skeleton = static_cast<Skeleton*>(entity);
		skeleton->SetReciever(this);

		AddSkeleton(skeleton);
	}
	else if (entityToMake == "zombie")
	{
		entity = EntityFactory::GetInstance()->MakeEntity("zombie");

		// Set the entity's receiver to this world.
		Zombie* zombie = static_cast<Zombie*>(entity);
		zombie->SetReciever(this);

		AddZombie(zombie);
	}
	else if (entityToMake == "survivor")
	{
		entity = EntityFactory::GetInstance()->MakeEntity("survivor");

		// Set the entity's receiver to this world.
		Survivor* survivor = static_cast<Survivor*>(entity);
		survivor->SetReciever(this);

		AddSurvivor(survivor);
	}
	else if (entityToMake == "bullet")
	{
		entity = EntityFactory::GetInstance()->MakeEntity("bullet");

		Projectile* projectile = static_cast<Projectile*>(entity);
		projectile->SetReciever(this);

		AddProjectile(projectile);
	}

	return entity;
}

void World::AddEntity(IEntity* entity)
{
	this->entityMap.insert(std::make_pair(entity->GetEntityID(), entity));
}


void World::DrawEntities(const CompiledShader& shader)
{
	std::map<unsigned int, IEntity*>::iterator it;
	for (it = this->entityMap.begin(); it != this->entityMap.end(); it++)
	{
		IEntity* entity = it->second;
		ModelManager::GetInstance()->Draw(entity->GetModelName(), shader, entity->GetPosition(), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), entity->GetScale());
	}
}

bool World::RecieveMessage(sMessage theMessage)
{
	if (theMessage.command == "FindSurvivor")
	{
		unsigned int entityID = (unsigned int) theMessage.fData[0].x;

		std::map<unsigned int, IEntity*>::iterator it = this->entityMap.find(entityID);
		if (it != this->entityMap.end() && !this->survivors.empty()) // The entity that is requesting exists, and we have some survivors
		{
			Entity* entity = static_cast<Entity*>(it->second);
			glm::vec3 entityPos = entity->GetPosition();

			// Find the closest survivor
			float minDistance = -1.0f;
			IEntity* closestSurvivor = NULL;
			std::map<unsigned int, Survivor*>::iterator survivorIt;
			for (survivorIt = this->survivors.begin(); survivorIt != this->survivors.end(); survivorIt++)
			{
				float distance = glm::distance(survivorIt->second->GetPosition(), entityPos);
				if (minDistance == -1.0f || distance < minDistance)
				{
					closestSurvivor = survivorIt->second;
				}
			}

			sMessage message;
			message.command = "FoundSurvivor";
			message.pData.push_back((void*) closestSurvivor);

			entity->RecieveMessage(message); // Send the found survivor back to the entity looking
		}

		return true;
	}
	else if (theMessage.command == "FindMonster")
	{
		unsigned int entityID = (unsigned int)theMessage.fData[0].x;
		std::map<unsigned int, IEntity*>::iterator it = this->entityMap.find(entityID);
		if (it != this->entityMap.end() && (!this->zombies.empty() || !this->skeletons.empty())) // The entity that is requesting exists, and we have some monsters
		{
			Entity* entity = static_cast<Entity*>(it->second);
			glm::vec3 entityPos = entity->GetPosition();

			// Find the closest survivor
			float minDistance = -1.0f;
			IEntity* closestMonster = NULL;

			// Look for zombies
			std::map<unsigned int, Zombie*>::iterator zombieIt;
			for (zombieIt = this->zombies.begin(); zombieIt != this->zombies.end(); zombieIt++)
			{
				float distance = glm::distance(zombieIt->second->GetPosition(), entityPos);
				if (minDistance == -1.0f || distance < minDistance)
				{
					closestMonster = zombieIt->second;
				}
			}

			// Look for skeletons
			std::map<unsigned int, Skeleton*>::iterator skeletonIt;
			for (skeletonIt = this->skeletons.begin(); skeletonIt != this->skeletons.end(); skeletonIt++)
			{
				float distance = glm::distance(skeletonIt->second->GetPosition(), entityPos);
				if (minDistance == -1.0f || distance < minDistance)
				{
					closestMonster = skeletonIt->second;
				}
			}

			sMessage message;
			message.command = "FoundMonster";
			message.pData.push_back((void*) closestMonster);

			entity->RecieveMessage(message); // Send the found survivor back to the entity looking
		}

		return true;
	}
	else if (theMessage.command == "FireProjectile")
	{
		std::string projectileType = theMessage.data[0];
		float yaw = theMessage.fData[0].x;
		float pitch = theMessage.fData[0].y;
		glm::vec4 position = theMessage.fData[1];

		IEntity* entity = MakeEntity(projectileType);
		entity->SetYaw(yaw);
		entity->SetPitch(pitch);
		entity->SetPosition(glm::vec3(position));

		Projectile* proj = static_cast<Projectile*>(entity);
		proj->SetShooterID((unsigned int) theMessage.fData[0].z);

		return true;
	}

	std::cout << "Command '" << theMessage.command << "' does not have a handler in world!" << std::endl;
	return false;
}

bool World::SetReciever(IMessage* pTheReciever)
{
	this->receiver = pTheReciever;
	return true;
}

bool World::RecieveMessage(sMessage theMessage, sMessage& messageReply)
{
	return true;
}

void World::AddZombie(Zombie* zombie)
{
	this->zombies.insert(std::make_pair(zombie->GetEntityID(), zombie)); // Add to the zombie map
	AddEntity(zombie); // Add to our general tracking map
}

void World::AddSkeleton(Skeleton* skeleton)
{
	this->skeletons.insert(std::make_pair(skeleton->GetEntityID(), skeleton)); // Add to the skeleton map
	AddEntity(skeleton); // Add to our general tracking map
}

void World::AddSurvivor(Survivor* survivor)
{
	this->survivors.insert(std::make_pair(survivor->GetEntityID(), survivor)); // Add to the survivor map
	AddEntity(survivor); // Add to our general tracking map
}

void World::AddProjectile(Projectile* projectile)
{
	this->projectiles.insert(std::make_pair(projectile->GetEntityID(), projectile)); // Add to the projectile map
	AddEntity(projectile); // Add to our general tracking map
}

void World::RemoveEntities(std::vector<unsigned int> ids)
{
	for (unsigned int& id : ids)
	{
		std::map<unsigned int, IEntity*>::iterator it = this->entityMap.find(id);
		if (it != this->entityMap.end())
		{
			bool isMonster = skeletons.find(id) != skeletons.end() || zombies.find(id) != zombies.end();
			bool isSurvivor = survivors.find(id) != survivors.end();

			if (isMonster)
			{
				this->skeletons.erase(id);
				this->zombies.erase(id);

				std::map<unsigned int, Survivor*>::iterator survivorIt; // Check if a survivor has the invalid entity as a target
				for (survivorIt = this->survivors.begin(); survivorIt != this->survivors.end(); survivorIt++)
				{
					survivorIt->second->RemoveTarget(it->second);
				}
			}
			else if (isSurvivor)
			{
				this->survivors.erase(id);

				std::map<unsigned int, Skeleton*>::iterator skeletonIt;  // Check if a skeleton has the invalid entity as a target
				for (skeletonIt = this->skeletons.begin(); skeletonIt != this->skeletons.end(); skeletonIt++)
				{
					skeletonIt->second->RemoveTarget(it->second);
				}

				std::map<unsigned int, Zombie*>::iterator zombieIt;  // Check if a zombie has the invalid entity as a target
				for (zombieIt = this->zombies.begin(); zombieIt != this->zombies.end(); zombieIt++)
				{
					zombieIt->second->RemoveTarget(it->second);
				}
			}
			else
			{
				this->projectiles.erase(id);
			}
			
			delete it->second;
			this->entityMap.erase(id);
		}
	}
}