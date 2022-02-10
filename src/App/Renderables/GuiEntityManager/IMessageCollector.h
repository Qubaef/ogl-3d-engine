#pragma once
#include "Engine/Components/MessageBus/Message.h"

class IMessageCollector
{
public:
	virtual void addMessage(Message* message, const char* recipient) = 0;

	virtual ~IMessageCollector() = default;
};
