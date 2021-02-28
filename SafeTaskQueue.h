﻿#pragma once

// Library includes
#include <queue>
#include <condition_variable>
#include <mutex>
#include <thread>

// Own includes
#include "ObligatoryHeaders.h"
#include "Task.h"


class SafeTaskQueue
{
private:
	// Prime queue, which is being copied every frame to task Queue
	std::queue<Task> primeQueue;

	// Current task's queue, which has to be emptied each frame
	std::queue<Task> tasksQueue;

	// Conditional variable used to hold threads until the new frame notification
	std::mutex conditionalVariableMutex;
	std::condition_variable nextFrameConditionalVariable;

	// Mutex used to make queue operations thread safe
	std::mutex queueModificationMutex;

	// Active threads vector
	std::vector<std::thread> activeWorkersVector;

	// Number of threads waiting for notification
	std::atomic_int threadsWaiting = 0;

	// Get next task with enabled synchronization
	Task getNextTask();

	// Check if there are any tasks left in the taskQueue
	bool isTaskQueueEmpty();

	// Start worker cycle on current process
	void startWorkerCycle(int threadId);

public:
	const int MaxQueueActiveThreads = 2;
	
	// Insert new given task to priority queue
	void pushTask(Task task);

	// Pop and return top task from the queue
	Task popTask();

	// Start given number of threads running in worker cycle
	void runWorkers(int numberOfWorkers);

	// Notify all the workers and the queue about the new frame event
	void newFrameNotify();

	// Process queue's cycle regularly without mutlithreading
	void processQueue();

	// Check if all taskQueue tasks were finished and if threads are waiting
	bool ifFinished();
};