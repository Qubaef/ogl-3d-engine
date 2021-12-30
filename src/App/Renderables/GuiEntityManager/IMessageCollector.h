#pragma once
#include "Engine/MessageBus/Message.h"

class IMessageCollector
{
public:
	 virtual void addMessage(Message* message, const char* recipient) = 0;
};
