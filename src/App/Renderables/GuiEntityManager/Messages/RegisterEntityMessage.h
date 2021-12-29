#pragma once

#ifndef REGISTER_ENTITY_MESSAGE_H
#define REGISTER_ENTITY_MESSAGE_H

#include "Engine/MessageBus/Message.h"

class RegisterEntityMessage : public Message
{
	const char* parentPath;
public:

	RegisterEntityMessage(const char* parentPath);

	~RegisterEntityMessage() override;

	const char* getParentPath() const;
};

#endif
