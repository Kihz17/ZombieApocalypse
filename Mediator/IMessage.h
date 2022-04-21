#pragma once

#include <glm/vec4.hpp>
#include <string>
#include <vector>

struct sMessage
{
	std::string command;
	std::vector<std::string> data;
	std::vector<glm::vec4> fData;
	std::vector<void*> pData;
};

class IMessage
{
public:
	virtual ~IMessage()
	{

	}

	// Asynchronous (don't return)
	virtual bool RecieveMessage(sMessage theMessage) = 0;
	virtual bool SetReciever(IMessage* pTheReciever) = 0;

	// Synchronous (do return with something)
	virtual bool RecieveMessage(sMessage theMessage, sMessage& messageReply) = 0;
};