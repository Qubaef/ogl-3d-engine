#pragma once

#include "Engine/CpuPipeline/IProcessable.h"

class TestEntity : IProcessable
{
public:
	TestEntity(Engine& engine);
	
	void process() override;

	~TestEntity() override = default;
};
