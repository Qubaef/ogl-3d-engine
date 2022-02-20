#pragma once

#include "App/Renderables/GuiEntityManager/GuiEntry/GuiEntry.h"
#include "App/Renderables/GuiEntityManager/IMessageCollector.h"

struct GuiProperty : public GuiEntry
{
	friend bool operator==(const GuiProperty& lhs, const GuiProperty& rhs)
	{
		return static_cast<const GuiEntry&>(lhs) == rhs;
	}

	friend bool operator!=(const GuiProperty& lhs, const GuiProperty& rhs)
	{
		return !(lhs == rhs);
	}

	GuiProperty(const GuiProperty& other)
		: GuiEntry(other),
		messageCollector(other.messageCollector)
	{
	}

	GuiProperty(GuiProperty&& other) noexcept
		: GuiEntry(std::move(other)),
		messageCollector(other.messageCollector)
	{
	}

	GuiProperty& operator=(const GuiProperty& other)
	{
		if (this == &other)
			return *this;
		GuiEntry::operator =(other);
		messageCollector = other.messageCollector;
		return *this;
	}

	GuiProperty& operator=(GuiProperty&& other) noexcept
	{
		if (this == &other)
			return *this;
		messageCollector = other.messageCollector;
		GuiEntry::operator =(std::move(other));
		return *this;
	}

	IMessageCollector* messageCollector = nullptr;

	GuiProperty(std::string name)
		: GuiEntry(std::move(name))
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
	if (messageCollector != nullptr)
	{
		messageCollector->addMessage(message, recipient);
	}
}
