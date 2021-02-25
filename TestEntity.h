#pragma once
#include <cstdlib>
#include <string>
#include <algorithm>
#include "EntityType.h"

class TestEntity : EntityType
{
public:
	void processPerFrame() override
	{
		int testSize = 1000;

		for(int i = 0; i < testSize; i++)
		{
			float newVal = rand() / RAND_MAX;
			float res = newVal * newVal + i / newVal;

			std::string newString = "testValue";
			std::string copy(newString);
			std::reverse(copy.begin(), copy.end());
		}

		// printf("Test performed!\n");
	}
};
