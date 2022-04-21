#include "EntityFactory.h"
#include "Skeleton.h"
#include "Zombie.h"
#include "Survivor.h"
#include "Projectile.h"

#include <iostream>

EntityFactory* EntityFactory::instance = NULL;
unsigned int EntityFactory::currentEntityID = 0;

EntityFactory::EntityFactory()
{

}

EntityFactory::~EntityFactory()
{

}

EntityFactory* EntityFactory::GetInstance()
{
	if (EntityFactory::instance == NULL)
	{
		EntityFactory::instance = new EntityFactory();
	}

	return instance;
}

IEntity* EntityFactory::MakeEntity(std::string entityToMake)
{
	if (entityToMake == "skeleton")
	{
		return new Skeleton(this->currentEntityID++);
	}
	else if (entityToMake == "zombie")
	{
		return new Zombie(this->currentEntityID++);
	}
	else if (entityToMake == "survivor")
	{
		return new Survivor(this->currentEntityID++, 1000);
	}
	else if (entityToMake == "bullet")
	{
		return new Projectile(this->currentEntityID++, "bullet", 50.0f, 100);
	}

	std::cout << "Invalid entity '" << entityToMake << "'." << std::endl;
	return NULL;
}