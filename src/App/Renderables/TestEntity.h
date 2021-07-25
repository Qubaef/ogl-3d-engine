#pragma once
#include <cstdlib>
#include <string>
#include <algorithm>

#include "../../Engine/CpuPipeline/Processable.h"

class TestEntity : Processable
{
public:
	TestEntity(Engine* enginePtr);
	
	void process() override;

	~TestEntity() override = default;
};
