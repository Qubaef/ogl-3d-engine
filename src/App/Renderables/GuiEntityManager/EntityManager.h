#pragma once
#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <vector>
#include <map>

#include "../../../Engine/CpuPipeline/IProcessable.h"
#include "Engine/MessageBus/IMessanger.h"

#include "FundamentalProperty.h"


class EntityManager : public IProcessable, public IMessanger
{
	const char* DISPLAY_NAME = "Entity manager";

	// std::vector<FundamentalProperty<int>> entityPropertiesList;
	std::map<const char*, std::vector<Property*>> entitiesPropertiesDict;

	// Add property to the entity manager
	// return true if property was added successfully
	bool addEntityProperty(const char* entityName, Property* property);

public:
	EntityManager(Engine* enginePtr);

	void preprocess() override;
	void process() override;
	void render() override;
};

#endif
