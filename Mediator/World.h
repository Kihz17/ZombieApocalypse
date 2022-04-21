#pragma once

#include "IEntity.h"
#include "CompiledShader.h"
#include "IMessage.h"
#include "IEntityManagement.h"

#include <vector>
#include <map>

class World : public IMessage, public IEntityManagement
{
public:
	World();
	~World();

	void Tick(float deltaTime);

	void DrawEntities(const CompiledShader& shader); // This probably shouldn't be here, but for the sake of saving time im leaveing it

	IEntity* MakeEntity(std::string entityToMake);

	// Inherited from IMessage
	virtual bool RecieveMessage(sMessage theMessage);
	virtual bool SetReciever(IMessage* pTheReciever);
	virtual bool RecieveMessage(sMessage theMessage, sMessage& messageReply);

private:
	void AddEntity(IEntity* entity);
	void RemoveEntities(std::vector<unsigned int> ids);

	// Inherited from IObjectManagement
	virtual void AddZombie(Zombie* zombie);
	virtual void AddSkeleton(Skeleton* skeleton);
	virtual void AddSurvivor(Survivor* survivor);
	virtual void AddProjectile(Projectile* projectile);

	std::map<unsigned int, IEntity*> entityMap;
	std::map<unsigned int, Skeleton*> skeletons;
	std::map<unsigned int, Zombie*> zombies;
	std::map<unsigned int, Survivor*> survivors;
	std::map<unsigned int, Projectile*> projectiles;

	IMessage* receiver;
};