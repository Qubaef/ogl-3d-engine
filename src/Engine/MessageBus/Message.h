#pragma once

#ifndef MESSAGE_H
#define MESSAGE_H

#include "IMessanger.h"

struct Message
{
	IMessanger* sender = nullptr;
	IMessanger* recipient = nullptr;


	// Force abstract
	virtual ~Message() = 0;
};

#endif

