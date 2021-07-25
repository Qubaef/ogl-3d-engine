#pragma once
#ifndef TASK_H
#define TASK_H
#include "Processable.h"
#include "Renderable.h"

struct Task
{
	// Processable method pointer
	typedef void (Processable::* ProcessableFunction) ();

	ProcessableFunction function;
	Processable* ownerPtr;

	Task() :
		function(nullptr), ownerPtr(nullptr)
	{
	}

	Task(Processable* ownerPtr, ProcessableFunction function) :
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