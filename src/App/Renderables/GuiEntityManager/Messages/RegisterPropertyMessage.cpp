#include "RegisterPropertyMessage.h"

RegisterPropertyMessage::RegisterPropertyMessage(const char* entityPath, GuiProperty* property)
	: entityPath(entityPath), property(property)
{
}

RegisterPropertyMessage::~RegisterPropertyMessage()
{
}

const char* RegisterPropertyMessage::getEntityPath() const
{
	return entityPath;
}

GuiProperty* RegisterPropertyMessage::getProperty() const
{
	return property;
}
