#pragma once

#ifndef REGISTER_PROPERTY_MESSAGE_H
#define REGISTER_PROPERTY_MESSAGE_H

#include "App/Renderables/GuiEntityManager/EntityProperties/GuiProperty.h"
#include "Engine/MessageBus/Message.h"

class RegisterPropertyMessage : public Message
{
	const char* entityPath;
	GuiProperty* property;
public:

	RegisterPropertyMessage(const char* entityPath, GuiProperty* property);

	~RegisterPropertyMessage() override;

	const char* getEntityPath() const;

	GuiProperty* getProperty() const;
};

#endif