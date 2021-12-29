#pragma once
#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include "../../../Engine/CpuPipeline/IProcessable.h"
#include "Engine/MessageBus/IMessanger.h"

#include "GuiEntry/GuiEntry.h"


struct GuiProperty;

class EntityManager : public IProcessable, public IMessanger
{
	const char* DISPLAY_NAME = "Entity manager";

	GuiEntry rootEntry;

	// Add entity to the entity manager
	// return true if property was added successfully
	bool addEntity(const char* parentPath, const char* entityName);

	// Add property to the entity manager
	// return true if property was added successfully
	bool addEntityProperty(const char* entityPath, GuiProperty* property);

	// TODO: container for all pending messages from entities and properties
	// std::vector<Message*> pendingMessages;
	//
	// TODO: all entities and properties receive callback to function to insert message to pendingMessages
	// TODO: modification of pendingMessages is synchronized 

public:
	EntityManager(Engine* enginePtr);

	void preprocess() override;
	void process() override;
	void render() override;
};

#endif
