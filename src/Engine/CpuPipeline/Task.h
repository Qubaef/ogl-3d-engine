#pragma once

#include <intrin.h>

#include "IProcessable.h"

struct Task
{
	// IProcessable method pointer
	typedef void (IProcessable::* ProcessableFunction) ();

	ProcessableFunction function;
	IProcessable* ownerPtr;

	Task() :
		function(nullptr), ownerPtr(nullptr)
	{
	}

	Task(IProcessable* ownerPtr, ProcessableFunction function) :
		function(function), ownerPtr(ownerPtr)
	{
	}

	~Task()
	{
		__nop();
	}

	// Check if task is performable
	bool performable()
	{
		return ownerPtr && function;
	}

	// Perform the task's function
	void perform()
	{
		((*ownerPtr).*(function))();
	}
};