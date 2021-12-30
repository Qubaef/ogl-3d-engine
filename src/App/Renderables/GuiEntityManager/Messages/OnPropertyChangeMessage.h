#pragma once

#ifndef ON_PROPERTY_CHANGE_MESSAGE_H
#define ON_PROPERTY_CHANGE_MESSAGE_H

#include "Engine/MessageBus/Message.h"

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

#endif