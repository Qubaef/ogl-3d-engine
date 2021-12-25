#include "IMessanger.h"

#include "Message.h"
#include "MessageBus.h"

IMessanger::IMessanger(MessageBus* messageBus, const char* name)
	: messageBus(messageBus), name(name)
{
	id = messageBus->registerMessanger(this);
}

void IMessanger::sendMessage(Message* message, const char* recipientName)
{
	messageBus->sendMessage(message, this, recipientName);
}

void IMessanger::sendMessage(Message* message, int recipientId)
{
	messageBus->sendMessage(message, this, recipientId);
}

void IMessanger::sendMessage(Message* message, IMessanger* recipient)
{
	messageBus->sendMessage(message, this, recipient);
}

void IMessanger::emitMessage(Message* message)
{
	messageBus->emitMessage(this, message);
}

std::vector<Message*> IMessanger::getMessages()
{
    return messageBus->getMessages(this);
}

bool IMessanger::subscribe(const char* targetMessangerName)
{
    return messageBus->subscribe(this, targetMessangerName);
}

bool IMessanger::subscribe(int targetMessangerId)
{
    return messageBus->subscribe(this, targetMessangerId);
}

bool IMessanger::subscribe(IMessanger* targetMessanger)
{
    return messageBus->subscribe(this, targetMessanger);
}

bool IMessanger::unsubscribe(const char* targetMessangerName)
{
    return messageBus->unsubscribe(this, targetMessangerName);
}

bool IMessanger::unsubscribe(int targetMessangerId)
{
    return messageBus->unsubscribe(this, targetMessangerId);
}

bool IMessanger::unsubscribe(IMessanger* targetMessanger)
{
    return messageBus->unsubscribe(this, targetMessanger);
}

int IMessanger::getId()
{
    return id;
}

const char* IMessanger::getName()
{
    return name;
}
