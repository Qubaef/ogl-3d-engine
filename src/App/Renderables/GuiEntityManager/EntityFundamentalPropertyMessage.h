#pragma once


#include "FundamentalProperty.h"
#include "Engine/MessageBus/Message.h"

template <typename T>
class EntityFundamentalPropertyMessage : public Message
{
public:
	FundamentalProperty<T> entityFundamentalProperty;

	EntityFundamentalPropertyMessage(const char* name, T minValue, T maxValue, T defaultValue, T value)
		: entityFundamentalProperty(name, minValue, maxValue, defaultValue, value)
	{
	}

	~EntityFundamentalPropertyMessage() override
	{
	}
};