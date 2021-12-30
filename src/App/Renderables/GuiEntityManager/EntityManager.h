﻿#pragma once
#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <mutex>

#include "../../../Engine/CpuPipeline/IProcessable.h"
#include "Engine/MessageBus/IMessanger.h"

#include "GuiEntry/GuiEntry.h"
#include "EntityProperties/GuiProperty.h"
#include "IMessageCollector.h"


class EntityManager : public IProcessable, public IMessanger, public IMessageCollector
{
	const char* DISPLAY_NAME = "Entity manager";

	GuiEntry rootEntry;

	// Add entity to the entity manager
	// return true if property was added successfully
	bool addEntity(const char* parentPath, const char* entityName);

	// Add property to the entity manager
	// return true if property was added successfully
	bool addEntityProperty(const char* entityPath, GuiProperty* property);

	// Container for all pending messages from entities and properties
	std::vector<std::pair<Message*, const char*>> pendingMessages;
	// Mutex for pending messages
	std::mutex pendingMessagesMutex;
public:
	EntityManager(Engine* enginePtr);

	void preprocess() override;
	void process() override;
	void render() override;

	void addMessage(Message* message, const char* recipient) override;
};

#endif
