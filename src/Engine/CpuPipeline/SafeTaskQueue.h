#pragma once
#ifndef SAFETASKQUEUE_H
#define SAFETASKQUEUE_H

// Library includes
#include <queue>
#include <condition_variable>
#include <mutex>
#include <thread>

// Own includes
#include "Task.h"

class SafeTaskQueue
{
private:
	// Prime queue, which is being copied every frame to task Queue
	std::queue<Task> primaryQueue;
	
	// Current queue, which has to be emptied each frame
	std::queue<Task> currentQueue;
	
	// Temporary queue, containing tasks which are supposed to be played only once (not refresh every frame)
	std::queue<Task> temporaryQueue;

	
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

	// Start worker cycle on current process
	void startWorkerCycle(int threadId);

public:
	const int MaxQueueActiveThreads = 2;

	
	// Insert new given task to priority queue
		// * Permanently pushes to primaryQueue, so task will be performed in currentQueue every refresh 
		// * Temporary pushes to temporaryQueue, so task will be performed only once until finished;
		// also, tasks added to Temporary will be performed next cycle (when using workers)
	void pushTaskPermanently(Task task);
	void pushTaskTemporary(Task task);

	// Pop and return top task from the primaryQueue, so it won't be performed every refresh
	Task popTask();

	// Check if all taskQueue tasks were finished and if threads are waiting
	bool ifFinished();

	// Process queue's cycle regularly without mutlithreading
	void processQueue();

	// Process next queue's temporary task, if one was present. Return false if there was none.
	bool processNextTemporary();

	// Process all queue's temporary tasks
	void processTemporary();
	
	// Start given number of threads running in worker cycle
	void runWorkers(int numberOfWorkers);

	// Wait for all tasks to be finished and then refresh currentQueue and run back the threads
	void refreshCycle();
};

# endif SAFETASKQUEUE_H
