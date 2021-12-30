#pragma once

#include "App/Renderables/GuiEntityManager/IDisplayable.h"
#include "App/Renderables/GuiEntityManager/IMessageCollector.h"

struct GuiProperty : IDisplayable
{
	const char* name;
	IMessageCollector* messageCollector = nullptr;

	GuiProperty(const char* name)
		: name(name)
	{
	}

	void setMessageCollector(IMessageCollector* messageCollector);

	void display() override = 0;

	virtual ~GuiProperty() = default;

protected:
	void sendMessage(Message* message, const char* recipient);
};

inline void GuiProperty::setMessageCollector(IMessageCollector* messageCollector)
{
	this->messageCollector = messageCollector;
}

inline void GuiProperty::sendMessage(Message* message, const char* recipient)
{
	if(messageCollector != nullptr)
	{
		messageCollector->addMessage(message, recipient);
	}
}
