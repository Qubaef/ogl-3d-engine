#include "EntityManager.h"

#include "Engine/Engine.h"
#include "Engine/MessageBus/MessageBus.h"

EntityManager::EntityManager(Engine* enginePtr)
	: IProcessable(enginePtr), IMessanger(&enginePtr->getMessageBus(), "EntityManager")
{
	//MessageBus* messageBus = new MessageBus();
	//messageBus.


	//// Registration
	//enginePtr->messageBus.subscribe(this, "recipient");

	//// Download of messages
	//messagesList = enginePtr->messageBus.getMessages();

	//// Sending a message
	//Message message();
	//enginePtr->messageBus.emitMessage(message);
	//// Or
	//enginePtr->messageBus.sendMessage(message, "recipient");
}

void EntityManager::preprocess()
{
}

void EntityManager::process()
{
}

void EntityManager::render()
{
}


