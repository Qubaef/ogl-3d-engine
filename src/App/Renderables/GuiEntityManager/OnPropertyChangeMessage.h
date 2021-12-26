#pragma once

#ifndef ON_PROPERTY_CHANGE_MESSAGE_H
#define ON_PROPERTY_CHANGE_MESSAGE_H

#include "FundamentalProperty.h"
#include "Engine/MessageBus/Message.h"

template <typename T>
class OnPropertyChangeMessage : public Message
{
	const char* name;
	T value;
public:
	OnPropertyChangeMessage(const char* name, T value)
		: name(name), value(value)
	{
		if (!std::is_fundamental_v<T>) {
			// Property has to be fundamental type
			_ASSERT(false);
		}
	}

	~OnPropertyChangeMessage() override
	{
	}

	const char* getName()
	{
		return name;
	}

	T getValue()
	{
		return value;
	}
};

#endif