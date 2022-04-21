#pragma once

#include "IEntity.h"
#include <string>

class EntityFactory
{
public:
	~EntityFactory();

	static EntityFactory* GetInstance();

	IEntity* MakeEntity(std::string entityToMake);

private:
	EntityFactory();
	static EntityFactory* instance;
	static unsigned int currentEntityID;
};