#pragma once

#include <vector>

#include "../../../Engine/CpuPipeline/IProcessable.h"
#include "Engine/MessageBus/IMessanger.h"
#include "Engine/MessageBus/Message.h"


class EntityManager : IProcessable, IMessanger
{
	const char* DISPLAY_NAME = "Entity manager";

	std::vector<Message*> messagesList;

public:
	EntityManager(Engine* enginePtr);

	void preprocess() override;
	void process() override;
	void render() override;
};
