#pragma once

#include "Engine/Components/MessageBus/Message.h"

class OnPropertyChangeMessage : public Message
{
	const char* name;
	int value;
public:
	OnPropertyChangeMessage(const char* name, int value)
		: name(name), value(value)
	{
	}

	~OnPropertyChangeMessage() override
	{
	}

	const char* getName()
	{
		return name;
	}

	int getValue()
	{
		return value;
	}
};
