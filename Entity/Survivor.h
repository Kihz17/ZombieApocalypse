#include "LivingEntity.h"

class SurvivorImpl;
class Survivor : public LivingEntity
{
public:
	Survivor(unsigned int entityID, int health);
	virtual ~Survivor();

	virtual void Tick(float deltaTime);

	virtual void MoveTick(float deltaTime);

	virtual bool RecieveMessage(sMessage theMessage);

	virtual void RemoveTarget(IEntity* entity);

private:
	SurvivorImpl* survivorImpl;
};