#pragma once

// Library includes
#include <queue>
#include <condition_variable>
#include <mutex>

// Own includes
#include "Task.h"

class SafeTaskQueue
{
private:
	// Prime queue, which is being copied every frame to task Queue
	std::priority_queue<Task> primeQueue;

	// Current task's queue, which has to be emptied each frame
	std::priority_queue<Task> tasksQueue;

	// Conditional variable used to hold threads until the new frame notification
	std::condition_variable nextFrameConditionalVariable;

	// Mutex used to make queue operations thread safe
	std::mutex queueModificationMutex;

	// Get next task with enables synchronization
	Task getNextTask()
	{
		// TODO:
	}

	// Check if there are any tasks left in the taskQueue
	bool isTaskQueueEmpty()
	{
		// TODO: 
	}

	// Start worker cycle on current process
	void startWorkerCycle()
	{
		// TODO:
	}


public:
	// Insert new given task to priority queue
	void pushTask(Task task)
	{
		primeQueue.push(task);
	}

	// Pop and return top task from the queue
	Task popTask()
	{
		Task task = primeQueue.top();
		primeQueue.pop();
		return task;
	}

	// Start given number of threads running in worker cycle
	void runWorkers(int numberOfWorkers)
	{
		// TODO:
	}

	// Notify all the workers and the queue about the new frame event
	void newFrameNotify()
	{
		// TODO: if all tasks were completed - rethink it - also implement in .cpp not here

		// Copy prime queue to taskQueue
		tasksQueue = primeQueue;

		// Notify all threads about the event
		nextFrameConditionalVariable.notify_all();
	}

	// Process queue's cycle regularly without mutlithreading (not practical)
	void processQueue()
	{
		// TODO:
	}
};
