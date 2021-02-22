#pragma once
#include "EntityType.h"

typedef void (EntityType::*TaskFunction) ();

struct Task
{
	EntityType* owner;
	TaskFunction taskFunction;
	int priority;
};