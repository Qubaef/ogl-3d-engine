#include "EntityManager.h"

#include "Engine/Engine.h"
#include "Engine/MessageBus/MessageBus.h"

#include "Messages/RegisterEntityMessage.h"
#include "Messages/RegisterPropertyMessage.h"
#include "Messages/OnPropertyChangeMessage.h"

bool EntityManager::addEntity(const char* parentPath, const char* entityName)
{
	// Add entity to the gui entries tree
	GuiEntry& parent = rootEntry.createBranch(parentPath);
	parent.addChild(new GuiEntry(entityName));

	return true;
}

bool EntityManager::addEntityProperty(const char* entityPath, GuiProperty* property)
{
	// Set message collector to entity manager
	property->setMessageCollector(this);
	
	// Find given entity
	GuiEntry* entry = rootEntry.findEntry(entityPath);

	if (entry != nullptr)
	{
		entry->addChild(property);
		return true;
	}

	return false;
}

EntityManager::EntityManager(Engine* enginePtr)
	: IProcessable(enginePtr),
	IMessanger(&enginePtr->getMessageBus(), "EntityManager"),
	rootEntry(GuiEntry("Entities", ROOT))
{
}

void EntityManager::preprocess()
{
}

void EntityManager::process()
{
	// Send pending messages to the message bus
	pendingMessagesMutex.lock();

	for (const auto messageWRecipient : pendingMessages)
	{
		sendMessage(
			messageWRecipient.first,
			messageWRecipient.second);
	}

	pendingMessages.clear();

	pendingMessagesMutex.unlock();

	// Acquire all messages from the message bus
	const auto messages = getMessages();

	for (Message* message : messages)
	{
		if (RegisterEntityMessage* registerEntityMessage = dynamic_cast<RegisterEntityMessage*>(message))
		{
			addEntity(registerEntityMessage->getParentPath(), registerEntityMessage->sender->getName());
			continue;
		}

		if (RegisterPropertyMessage* registerPropertyMessage = dynamic_cast<RegisterPropertyMessage*>(message))
		{
			addEntityProperty(registerPropertyMessage->getEntityPath(), registerPropertyMessage->getProperty());
			continue;
		}
	}
}

void EntityManager::render()
{
	//
	// Entity editor
	//
	const float entityEditorWidth = enginePtr->getConstProperties().windowWidth * 0.25;
	const float entityEditorHeight = enginePtr->getConstProperties().windowHeight;

	ImGui::SetNextWindowSize(ImVec2(entityEditorWidth, entityEditorHeight), ImGuiCond_Once);
	ImGui::SetNextWindowPos(ImVec2(enginePtr->getConstProperties().windowWidth - entityEditorWidth, 0), ImGuiCond_Once);
	ImGui::Begin("Entity editor", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

	rootEntry.display();

	ImGui::End();
}

void EntityManager::addMessage(Message* message, const char* recipient)
{
	// Lock mutex
	pendingMessagesMutex.lock();

	// Add message to pending messages
	pendingMessages.emplace_back(message, recipient);

	// Unlock mutex
	pendingMessagesMutex.unlock();
}


