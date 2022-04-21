#include "MonsterImpl.h"

MonsterImpl::MonsterImpl(float moveSpeed)
	: moveSpeed(moveSpeed), target(NULL), lastTargetCheckTime(0.0f)
{

}

MonsterImpl::~MonsterImpl()
{

}