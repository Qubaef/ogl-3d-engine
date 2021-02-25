#pragma once
#include "EntityType.h"

typedef void (EntityType::*TaskFunction) ();

struct Task
{
	EntityType* owner;
	TaskFunction taskFunction;

	Task()
	{
		owner = nullptr;
	}

	Task(EntityType* owner, TaskFunction taskFunction) :
		owner(owner), taskFunction(taskFunction)
	{
	}

	// Perform the task's function
	void perform()
	{
		(*owner.*taskFunction)();
	}
};