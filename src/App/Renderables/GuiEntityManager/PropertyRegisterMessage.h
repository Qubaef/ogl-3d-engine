#pragma once

#ifndef PROPERTY_REGISTER_MESSAGE_H
#define PROPERTY_REGISTER_MESSAGE_H

#include "FundamentalProperty.h"
#include "Engine/MessageBus/Message.h"

template <typename T>
class PropertyRegisterMessage : public Message
{
	FundamentalProperty<T> fundamentalProperty;
public:

	PropertyRegisterMessage(const char* name, T minValue, T maxValue, T defaultValue, T value)
		: fundamentalProperty(name, minValue, maxValue, defaultValue, value)
	{
		if (!std::is_fundamental_v<T>) {
			// Property has to be fundamental type
			_ASSERT(false);
		}
	}

	~PropertyRegisterMessage() override
	{
	}

	FundamentalProperty<T> getProperty()
	{
		return fundamentalProperty;
	}
};

#endif