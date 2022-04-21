#pragma once

#include <glm/vec3.hpp>
#include <string>

class EntityImpl
{
public:
	EntityImpl(unsigned int entityID, std::string modelName);
	virtual ~EntityImpl();

	const unsigned int entityID;
	float yaw;
	float pitch;
	glm::vec3 position;
	const std::string modelName;

};