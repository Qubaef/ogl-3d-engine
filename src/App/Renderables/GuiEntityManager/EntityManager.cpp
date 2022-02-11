#include "EntityManager.h"

#include "Engine/Engine.h"
#include "Engine/Components/MessageBus/MessageBus.h"

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

void EntityManager::sendPendingMessages()
{
	pendingMessagesMutex.lock();

	for (const auto& [message, recipient] : pendingMessages)
	{
		sendMessage(message, recipient);
	}

	pendingMessages.clear();

	pendingMessagesMutex.unlock();
}

EntityManager::EntityManager(Engine& engine)
	: IProcessable(engine),
	IMessanger(&engine.getMessageBus(), "EntityManager"),
	rootEntry(GuiEntry("Entities", ENTRY_TYPE::ROOT))
{
}

void EntityManager::preprocess()
{
}

void EntityManager::process()
{
	// Send pending messages to the recipients
	sendPendingMessages();

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
	if (engine.getShaderGlobalData().data.displayMode == static_cast<int>(ShaderGlobalData::DisplayMode::DEPTH))
	{
		return;
	}

	//
	// Entity editor
	//
	const float entityEditorWidth = engine.props.consts.windowWidth * 0.25f;
	const float entityEditorHeight = engine.props.consts.windowHeight;

	ImGui::SetNextWindowSize(ImVec2(entityEditorWidth, entityEditorHeight), ImGuiCond_Once);
	ImGui::SetNextWindowPos(ImVec2(engine.props.consts.windowWidth - entityEditorWidth, 0), ImGuiCond_Once);
	ImGui::Begin(DISPLAY_NAME, nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

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


