#pragma once

#include <glm/vec3.hpp>

class ProjectileImpl
{
public:
	ProjectileImpl(float speed, int damage);
	~ProjectileImpl();

	float speed;
	bool collided;
	int damage;

	bool setOriginalPos;
	glm::vec3 originalPos;

	unsigned int shooterID;
};