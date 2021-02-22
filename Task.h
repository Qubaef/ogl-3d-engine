#pragma once
#include "EntityType.h"

typedef void (EntityType::*TaskFunction) ();

struct Task
{
	EntityType* owner;
	TaskFunction taskFunction;
	int priority;

    // Overloaded < operator for priority comparison
	bool operator<(const Task& task) const
	{
		return (priority < task.priority);
	}
};