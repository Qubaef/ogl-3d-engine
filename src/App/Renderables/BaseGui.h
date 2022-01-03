#pragma once
#include "../../Engine/CpuPipeline/IProcessable.h"
#include "../../Engine/Engine.h"

class BaseGui : public IProcessable
{
	static const int frameTimesLength = 100;
	float frameTimes[frameTimesLength] = {0};

	static const int allocatedMemoryLength = 100;
	float allocatedMemory[frameTimesLength] = {0};
public:
	BaseGui(Engine* enginePtr);

	void process() override;

	void preprocess() override;

	void render() override;

	~BaseGui() override = default;

};
