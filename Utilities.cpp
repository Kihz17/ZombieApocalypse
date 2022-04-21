#include "Utilities.h"

#include <glm/glm.hpp>

float getRandom()
{
	return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}

float getRandom(float high)
{
	return static_cast<float>(rand()) / (static_cast<float>(RAND_MAX) / high);
}

float getRandom(float low, float high)
{
	return low + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX) / (high - low));
}

glm::vec3 getRandomXVector()
{
	return glm::vec3(getRandom(0.1f, 1.f), 0.f, getRandom(0.1f, 1.f));
}

glm::vec3 getRandomZVector()
{
	return glm::vec3(-getRandom(0.1f, 1.f), 0.f, getRandom(0.1f, 1.f));
}

glm::vec2 ConvertDirectionToEuler(glm::vec3& direction)
{
	float yaw = 0.0f;
	float pitch = 0.0f;

	if (direction.x == 0.0f && direction.z == 0.0f) // Gimbal lock
	{
		pitch = direction.y > 0.0f ? -90.0f : 90.0f;
	}
	else
	{
		// Convert new directional vector to euler angles
		float theta = atan2(-direction.x, direction.z);
		yaw = glm::degrees(theta);
		float xz = sqrt((direction.x * direction.x) + (direction.z * direction.z));
		pitch = glm::degrees(atan(-direction.y / xz));
	}

	return glm::vec2(yaw, pitch);
}