#pragma once
#ifndef TASK_H
#define TASK_H
#include "IProcessable.h"
#include "Renderable.h"

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

	// Check if task is performable
	bool performable()
	{
		return ownerPtr && function;
	}

	// Perform the task's function
	void perform()
	{
		ZoneScoped;

		((*ownerPtr).*(function))();

		//switch (taskType)
		//{
		//case PROCESSABLE:
		//	(*owner.processablePtr.*(taskFunction.processableFunction))();
		//	//owner.processablePtr->process();
		//	break;
		//case RENDERABLE:
		//	// BUG
		//	(*owner.renderablePtr.*(taskFunction.renderableFunction))();
		//	//owner.renderablePtr->process();
		//	break;
		//default:
		//	break;
		//}
	}
};

#endif