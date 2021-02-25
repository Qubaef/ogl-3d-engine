#include "SafeTaskQueue.h"

Task SafeTaskQueue::getNextTask()
{
	// Lock access to the taskQueue
	queueModificationMutex.lock();

	// Get task from top
	Task nextTask;

	if (!tasksQueue.empty())
	{
		nextTask = tasksQueue.front();
		tasksQueue.pop();
	}

	// Unlock access to the taskQueue
	queueModificationMutex.unlock();

	return nextTask;
}


bool SafeTaskQueue::isTaskQueueEmpty()
{
	// Lock access to the taskQueue
	queueModificationMutex.lock();

	// Check if queue is empty
	bool ifEmpty = tasksQueue.empty();

	// Unlock access to the taskQueue
	queueModificationMutex.unlock();

	return ifEmpty;
}


void SafeTaskQueue::startWorkerCycle(int threadId)
{
	printf("Thread %d: initialized\n", threadId);
	while (true)
	{
		// printf("Thread %d: preLockGuard\n", threadId);
		std::unique_lock<std::mutex> mutexLockGuard(conditionalVariableMutex);

		// Wait for notification on new portion of tasks
		threadsWaiting++;
		// printf("time diff 1: %d time diff 2: %d\n", std::chrono::duration_cast<std::chrono::microseconds>(t3 - t1).count(), std::chrono::duration_cast<std::chrono::microseconds>(t3 - t1).count());
		// printf("Thread %d: waiting!\n", threadId);
		t1 = std::chrono::high_resolution_clock::now();
		nextFrameConditionalVariable.wait(mutexLockGuard);
		mutexLockGuard.unlock();
		t3 = std::chrono::high_resolution_clock::now();
		// printf("Thread %d: unlocked!\n", threadId);
		threadsWaiting--;

		// Tasks performing loop
		while (true)
		{
			// printf("Thread %d: getting new task\n", threadId);
			Task task = getNextTask();

			if (task.owner == nullptr)
			{
				printf("Thread %d: No tasks\n", threadId);
				break;
			}

			task.perform();
			// printf("Thread %d: task completed\n", threadId);
		}
	}
}


void SafeTaskQueue::pushTask(Task task)
{
	primeQueue.push(task);
}


Task SafeTaskQueue::popTask()
{
	Task task = primeQueue.front();
	primeQueue.pop();
	return task;
}


void SafeTaskQueue::runWorkers(int numberOfWorkers)
{
	int threadsToStart = glm::min(MaxQueueActiveThreads, numberOfWorkers) - activeWorkersVector.size();

	for (int i = 0; i < threadsToStart; i++)
	{
		activeWorkersVector.emplace_back(&SafeTaskQueue::startWorkerCycle, this, activeWorkersVector.size());
	}
}


void SafeTaskQueue::newFrameNotify()
{
	// Wait for all threads to finish
	while (threadsWaiting != activeWorkersVector.size());

	std::unique_lock<std::mutex> mutexLockGuard(conditionalVariableMutex);
	
	// Copy prime queue to taskQueue
	tasksQueue = primeQueue;

	// Notify all threads about the event
	mutexLockGuard.unlock();
	t2 = std::chrono::high_resolution_clock::now();
	nextFrameConditionalVariable.notify_all();
}


void SafeTaskQueue::processQueue()
{
	while(!tasksQueue.empty())
	{
		Task nextTask = tasksQueue.front();
		tasksQueue.pop();

		nextTask.perform();
	}
}


bool SafeTaskQueue::ifTaskFinished()
{
	return tasksQueue.empty() && (threadsWaiting == activeWorkersVector.size());
}
