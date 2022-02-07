#pragma once

#include "Engine/Components/MessageBus/Message.h"
#include "App/Renderables/GuiEntityManager/EntityProperties/GuiProperty.h"

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
