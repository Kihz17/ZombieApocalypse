#pragma once

#include "IEntity.h"
#include "IMessage.h"

#include <string>

class EntityImpl;
class Entity : public IEntity, public IMessage
{
public:
	virtual ~Entity();

	virtual void MoveTick(float deltaTime) = 0; // This class should never be instantiated so we dont have an impl for this function yet

	virtual void Tick(float deltaTime);

	virtual glm::vec3 GetDirection() const;
	virtual glm::vec3 GetPosition() const;
	virtual float GetYaw() const;
	virtual float GetPitch() const;
	virtual std::string GetModelName() const;
	virtual unsigned int GetEntityID() const;

	virtual void SetPosition(glm::vec3 position);
	virtual void SetYaw(float yaw);
	virtual void SetPitch(float pitch);

	virtual float GetRadius();

	virtual bool IsValid();

	virtual glm::vec3 GetScale();

	// Inherited from IMessage
	virtual bool RecieveMessage(sMessage theMessage);
	virtual bool SetReciever(IMessage* pTheReciever);
	virtual bool RecieveMessage(sMessage theMessage, sMessage& messageReply);
protected:
	Entity(unsigned int entityID, std::string modelName);

	IMessage* receiver;

private:
	EntityImpl* impl;
};