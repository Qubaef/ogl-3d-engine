#pragma once
#include "../../Engine/CpuPipeline/Processable.h"
#include "../../Engine/Engine.h"

class BaseGui : public Processable
{
	static const int frameTimesLength = 100;
	float frameTimes[frameTimesLength] = {0};
public:
	BaseGui(Engine* enginePtr);

	void process() override;

	void preprocess() override;

	void render() override;

	~BaseGui() override = default;

};
