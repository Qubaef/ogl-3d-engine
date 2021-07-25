#include "ProcessableQueue.h"
#include "../Include/Common.h"

Task ProcessableQueue::getNextTask()
{
	ZoneScoped;

	// Lock access to the taskQueue
	queueModificationMutex.lock();

	// Get task from top
	Task nextTask;

	if (!tasksQueue.empty())
	{
		nextTask = tasksQueue.front();
		tasksQueue.pop();
	}
	else
	{
		nextTask = Task();
	}

	// Unlock access to the taskQueue
	queueModificationMutex.unlock();

	return nextTask;
}

void ProcessableQueue::refreshQueue()
{
	ZoneScoped;

	// Lock access to the taskQueue
	queueModificationMutex.lock();

	for (Processable* processable : processablesList)
	{
		switch (phase)
		{
		case PREPROCESS:
			if (processable->ifDefinedPreprocess())
			{
				tasksQueue.push(Task(processable, &Processable::preprocess));
			}
			break;
		case PROCESS:
			if (processable->ifDefinedProcess())
			{
				tasksQueue.push(Task(processable, &Processable::process));
			}
			break;
		case RENDER:
			if (processable->ifDefinedRender())
			{
				tasksQueue.push(Task(processable, &Processable::render));
			}
			break;
		case NONE: break;
		default:;
		};

	}

	// Lock access to the taskQueue
	queueModificationMutex.unlock();
}

void ProcessableQueue::startWorkerCycle(int threadId)
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

			if (!task.performable())
			{
				// printf("Thread %d: No tasks!\n", threadId);
				break;
			}

			task.perform();
		}
	}
}

ProcessableQueue::ProcessableQueue(int numberOfWorkers)
{
	ZoneScoped;

	// Initialize worker threads	
	int threadsToStart = glm::min(MaxWorkerThreads, numberOfWorkers) - activeWorkersVector.size();

	for (int i = 0; i < threadsToStart; i++)
	{
		activeWorkersVector.emplace_back(&ProcessableQueue::startWorkerCycle, this, activeWorkersVector.size());
	}
}

void ProcessableQueue::pushProcessable(Processable* processable)
{
	ZoneScoped;

	// Lock access to the taskQueue
	queueModificationMutex.lock();

	processablesList.push_back(processable);

	// Lock access to the taskQueue
	queueModificationMutex.unlock();
}

void ProcessableQueue::preprocess()
{
	ZoneScoped;

	// Wait for all threads to finish
	while (!ifFinished());

	// Set proper Phase
	phase = PREPROCESS;

	// Refresh queue
	refreshQueue();

	// Lock access to the taskQueue
	queueModificationMutex.lock();

	// Perform all tasks from primaryQueue
	while (!tasksQueue.empty())
	{
		tasksQueue.front().perform();
		tasksQueue.pop();
	}

	// Lock access to the taskQueue
	queueModificationMutex.unlock();
}

void ProcessableQueue::process()
{
	ZoneScoped;

	// Wait for all threads to finish
	while (!ifFinished());

	// Set proper Phase
	phase = PROCESS;

	std::unique_lock<std::mutex> mutexLockGuard(conditionalVariableMutex);

	// Refresh queue
	refreshQueue();

	// Notify all threads about the event
	mutexLockGuard.unlock();
	nextFrameConditionalVariable.notify_all();
}

void ProcessableQueue::render()
{
	ZoneScoped;

	// Wait for all threads to finish
	while (!ifFinished());

	// Set proper Phase
	phase = RENDER;

	// Refresh queue
	refreshQueue();

	// Lock access to the taskQueue
	queueModificationMutex.lock();

	// Perform all tasks from primaryQueue
	while (!tasksQueue.empty())
	{
		tasksQueue.front().perform();
		tasksQueue.pop();
	}

	// Lock access to the taskQueue
	queueModificationMutex.unlock();
}

void ProcessableQueue::processNext()
{
	ZoneScoped;

	Task task = getNextTask();

	if (task.performable())
	{
		task.perform();
	}
}

bool ProcessableQueue::ifFinished()
{
	return tasksQueue.empty() && threadsWaiting == activeWorkersVector.size();
}
