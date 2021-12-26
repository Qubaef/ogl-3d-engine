#include "EntityManager.h"

#include "Engine/Engine.h"
#include "Engine/MessageBus/MessageBus.h"

#include "EntityFundamentalPropertyMessage.h"
#include "PropertyRegisterMessage.h"

bool EntityManager::addEntityProperty(const char* entityName, Property* property)
{
	std::vector<Property*>& propertiesList = entitiesPropertiesDict[entityName];

	// Check if property already exists in the list
	for (Property* propertyInList : propertiesList)
	{
		// Compare property names
		if (strcmp(propertyInList->name, property->name) == 0)
		{
			// Property already exists in the list
			return false;
		}
	}

	// Add property to the list
	propertiesList.push_back(property);
	return true;
}

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
	const auto messages = getMessages();
	
	for (Message* message : messages)
	{
		try
		{
			PropertyRegisterMessage<int>* propertyRegisterMessage = dynamic_cast<PropertyRegisterMessage<int>*>(message);
			Property* newProperty = new FundamentalProperty(propertyRegisterMessage->getProperty());
			addEntityProperty(propertyRegisterMessage->sender->getName(), newProperty);
			continue;
		}
		catch (std::bad_cast& e)
		{
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

	// Iterate over all entities properties
	for (auto& entityPropertiesPair : entitiesPropertiesDict)
	{
		const char* entityName = entityPropertiesPair.first;
		std::vector<Property*>& entityPropertiesList = entityPropertiesPair.second;

		// Entity name
		ImGui::Text(entityName);

		// Entity properties
		for (Property* entityProperty : entityPropertiesList)
		{
			entityProperty->display();
		}
	}

	ImGui::End();
}


