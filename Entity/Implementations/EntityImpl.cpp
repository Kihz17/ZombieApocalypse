#include "EntityImpl.h"

EntityImpl::EntityImpl(unsigned int entityID, std::string modelName)
	: entityID(entityID),
	yaw(0.0f),
	pitch(0.0f),
	position(0.0f, 0.0f, 0.0f),
	modelName(modelName)
{

}

EntityImpl::~EntityImpl()
{

}