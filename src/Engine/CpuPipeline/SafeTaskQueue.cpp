#include "SafeTaskQueue.h"
#include "../Include/Common.h"

Task SafeTaskQueue::getNextTask()
{
	ZoneScoped;
	
	// Lock access to the taskQueue
	queueModificationMutex.lock();

	// Get task from top
	Task nextTask;

	if (!currentQueue.empty())
	{
		nextTask = currentQueue.front();
		currentQueue.pop();
	}
	else if(!temporaryQueue.empty())
	{
		nextTask = temporaryQueue.front();
		temporaryQueue.pop();
	}

	// Unlock access to the taskQueue
	queueModificationMutex.unlock();

	return nextTask;
}

void SafeTaskQueue::startWorkerCycle(int threadId)
{
	ZoneScoped;
	tracy::SetThreadName("WorkerThread");
	// printf("Thread %d: Initialized\n", threadId);
	while (true)
	{
		// printf("Thread %d: Prelock. ThreadsWaiting %d\n", threadId, unsigned(threadsWaiting));
		std::unique_lock<std::mutex> mutexLockGuard(conditionalVariableMutex);

		// Wait for notification on new portion of tasks
		// printf("Thread %d: Going to wait! ThreadsWaiting %d\n", threadId, unsigned(threadsWaiting));

		++threadsWaiting;
		nextFrameConditionalVariable.wait(mutexLockGuard);
		mutexLockGuard.unlock();
		--threadsWaiting; 

		// printf("Thread %d: Continuing! ThreadsWaiting %d\n", threadId, unsigned(threadsWaiting));

		// Tasks performing loop
		while (true)
		{
			Task task = getNextTask();

			if (task.taskType == Task::NONE)
			{
				// printf("Thread %d: No tasks!\n", threadId);
				break;
			}

			task.perform();
		}
	}
}


void SafeTaskQueue::pushTaskPermanently(Task task)
{
	ZoneScoped;

	// Lock access to the taskQueue
	queueModificationMutex.lock();
	
	primaryQueue.push(task);

	// Lock access to the taskQueue
	queueModificationMutex.unlock();
}

void SafeTaskQueue::pushTaskTemporary(Task task)
{
	ZoneScoped;

	// Lock access to the taskQueue
	queueModificationMutex.lock();
	
	temporaryQueue.push(task);

	// Lock access to the taskQueue
	queueModificationMutex.unlock();
}

Task SafeTaskQueue::popTask()
{
	ZoneScoped;

	// Lock access to the taskQueue
	queueModificationMutex.lock();
	
	Task task = primaryQueue.front();
	primaryQueue.pop();

	// Lock access to the taskQueue
	queueModificationMutex.unlock();
	return task;
}


bool SafeTaskQueue::ifFinished()
{
	ZoneScoped;
	
	return currentQueue.empty() && temporaryQueue.empty() && (threadsWaiting == activeWorkersVector.size());
}


void SafeTaskQueue::processQueue()
{
	ZoneScoped;

	// Lock access to the taskQueue
	queueModificationMutex.lock();
	
	// Refresh task queue
	currentQueue = primaryQueue;

	// Perform all tasks from primaryQueue
	while (!currentQueue.empty())
	{
		currentQueue.front().perform();
		currentQueue.pop();
	}

	// Perform all tasks from temporaryQueue
	while (!temporaryQueue.empty())
	{
		temporaryQueue.front().perform();
		temporaryQueue.pop();
	}

	// Lock access to the taskQueue
	queueModificationMutex.unlock();
}

bool SafeTaskQueue::processNextTemporary()
{
	ZoneScoped;

	// Lock access to the taskQueue
	queueModificationMutex.lock();

	if(!temporaryQueue.empty())
	{
		temporaryQueue.front().perform();
		temporaryQueue.pop();

		// Lock access to the taskQueue
		queueModificationMutex.unlock();
		return true;
	}

	// Lock access to the taskQueue
	queueModificationMutex.unlock();

	return false;
}

void SafeTaskQueue::processTemporary()
{
	ZoneScoped;

	while (processNextTemporary());
}

void SafeTaskQueue::runWorkers(int numberOfWorkers)
{
	ZoneScoped;
	
	int threadsToStart = glm::min(MaxQueueActiveThreads, numberOfWorkers) - activeWorkersVector.size();

	for (int i = 0; i < threadsToStart; i++)
	{
		activeWorkersVector.emplace_back(&SafeTaskQueue::startWorkerCycle, this, activeWorkersVector.size());
	}
}

void SafeTaskQueue::refreshCycle()
{
	ZoneScoped;
	
	// Wait for all threads to finish
	while (!ifFinished());

	std::unique_lock<std::mutex> mutexLockGuard(conditionalVariableMutex);

	// Copy prime queue to taskQueue
	currentQueue = primaryQueue;

	// Notify all threads about the event
	mutexLockGuard.unlock();
	nextFrameConditionalVariable.notify_all();
}
