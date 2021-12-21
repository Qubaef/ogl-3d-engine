#pragma once
#include <cstdlib>
#include <string>
#include <algorithm>

#include "../../Engine/CpuPipeline/IProcessable.h"

class TestEntity : IProcessable
{
public:
	TestEntity(Engine* enginePtr);
	
	void process() override;

	~TestEntity() override = default;
};
