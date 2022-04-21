#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

float getRandom();
float getRandom(float high);
float getRandom(float low, float high);
glm::vec3 getRandomXVector();
glm::vec3 getRandomZVector();

glm::vec2 ConvertDirectionToEuler(glm::vec3& direction);
