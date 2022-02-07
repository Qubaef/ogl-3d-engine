#pragma once

#include <mutex>
#include <condition_variable>
#include <thread>
#include <vector>
#include <queue>

#include "IProcessable.h"
#include "Task.h"

class ProcessableQueue
{
	const int MAX_WORKER_THREADS = 4;

	// Main vector
	std::vector<IProcessable*> processablesList;

	// Queue of tasks to perform
	std::queue<Task> tasksQueue;

	// Task type defining which function does it keep
	enum class ProcessableType
	{
		PREPROCESS,
		PROCESS,
		RENDER,
		NONE
	} phase;

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

	// Refresh queue of Tasks to perform
	void refreshQueue();

	// Start worker cycle on current process
	void startWorkerCycle(int threadId);

public:
	ProcessableQueue();

	// Initialize queue workers, return current number of active workers
	int initWorkers(int numberOfWorkers);

	// Add new processable to the queue
	void pushProcessable(IProcessable* processable);
	
	// Execute and finish Preprocess phase
	void preprocess();

	// Start Process phase; To process as worker thread, use processNext()
	void process();

	// Execute and finish Render phase 
	void render();

	// Process next task
	void processNext();

	// Check if all taskQueue tasks were finished and if threads are waiting
	bool ifFinished();
};
