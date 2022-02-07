#pragma once

#include "Engine/Components/MessageBus/Message.h"

class RegisterEntityMessage : public Message
{
	const char* parentPath;
public:

	RegisterEntityMessage(const char* parentPath);

	~RegisterEntityMessage() override;

	const char* getParentPath() const;
};
