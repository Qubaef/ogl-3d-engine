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
	// printf("Thread %d: Initialized\n", threadId);
	while (true)
	{
		//printf("Thread %d: Prelock. ThreadsWaiting %d\n", threadId, threadsWaiting);
		std::unique_lock<std::mutex> mutexLockGuard(conditionalVariableMutex);

		// Wait for notification on new portion of tasks
		// printf("Thread %d: Going to wait! ThreadsWaiting %d\n", threadId, threadsWaiting);

		++threadsWaiting;
		nextFrameConditionalVariable.wait(mutexLockGuard);
		mutexLockGuard.unlock();
		--threadsWaiting;

		// printf("Thread %d: Continuing! ThreadsWaiting %d\n", threadId, threadsWaiting);

		// Tasks performing loop
		while (true)
		{
			Task task = getNextTask();

			if (task.owner == nullptr)
			{
				// printf("Thread %d: No tasks!\n", threadId);
				break;
			}

			task.perform();
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
	nextFrameConditionalVariable.notify_all();
}


void SafeTaskQueue::processQueue()
{
	// Refresh task queue
	tasksQueue = primeQueue;

	while (!tasksQueue.empty())
	{
		tasksQueue.front().perform();
		tasksQueue.pop();
	}
}


bool SafeTaskQueue::ifFinished()
{
	return tasksQueue.empty() && (threadsWaiting == activeWorkersVector.size());
}
