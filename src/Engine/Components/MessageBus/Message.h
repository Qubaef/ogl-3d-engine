#pragma once

#include "IMessanger.h"

struct Message
{
	IMessanger* sender = nullptr;
	IMessanger* recipient = nullptr;


	// Force abstract
	virtual ~Message() = default;
};
