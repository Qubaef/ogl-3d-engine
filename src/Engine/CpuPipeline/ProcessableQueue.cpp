#include "ProcessableQueue.h"

#include "Engine/Include/Common.h"

using namespace glm;

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
	// Lock access to the taskQueue
	queueModificationMutex.lock();

	for (IProcessable* processable : processablesList)
	{
		switch (phase)
		{
		case ProcessableType::PREPROCESS:
			if (processable->ifDefinedPreprocess())
			{
				tasksQueue.push(Task(processable, &IProcessable::preprocess));
			}
			break;
		case ProcessableType::PROCESS:
			if (processable->ifDefinedProcess())
			{
				tasksQueue.push(Task(processable, &IProcessable::process));
			}
			break;
		case ProcessableType::RENDER:
			if (processable->ifDefinedRender())
			{
				tasksQueue.push(Task(processable, &IProcessable::render));
			}
			break;
		case ProcessableType::NONE:
			break;
		default:;
		}
	}

	// Lock access to the taskQueue
	queueModificationMutex.unlock();
}

void ProcessableQueue::startWorkerCycle(int threadId)
{
	tracy::SetThreadName("WorkerThread");

	// printf("Thread %d: Initialized\n", threadId);
	while (!shouldClose)
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

ProcessableQueue::ProcessableQueue()
	: phase(ProcessableType::NONE)
{
}

ProcessableQueue::~ProcessableQueue()
{
	// Detach all threads
	for (std::thread& thread : activeWorkersVector)
	{
		thread.join();
	}
}

int ProcessableQueue::initWorkers(int numberOfWorkers)
{
	// Calculate number of threads to initialize
	const int threadsToStart = min(MAX_WORKER_THREADS, numberOfWorkers) - activeWorkersVector.size();

	// Initialize worker threads
	for (unsigned char i = 0; i < threadsToStart; i++)
	{
		activeWorkersVector.emplace_back(&ProcessableQueue::startWorkerCycle, this, activeWorkersVector.size());
	}

	return activeWorkersVector.size();
}

void ProcessableQueue::pushProcessable(IProcessable* processable)
{
	// Lock access to the taskQueue
	queueModificationMutex.lock();

	processablesList.push_back(processable);

	// Lock access to the taskQueue
	queueModificationMutex.unlock();
}

void ProcessableQueue::preprocess()
{
	// Wait for all threads to finish
	while (!ifFinished());

	// Set proper Phase
	phase = ProcessableType::PREPROCESS;

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
	// Wait for all threads to finish
	while (!ifFinished());

	// Set proper Phase
	phase = ProcessableType::PROCESS;

	std::unique_lock<std::mutex> mutexLockGuard(conditionalVariableMutex);

	// Refresh queue
	refreshQueue();

	// Notify all threads about the event
	mutexLockGuard.unlock();
	nextFrameConditionalVariable.notify_all();
}

void ProcessableQueue::render()
{
	// Wait for all threads to finish
	while (!ifFinished());

	// Set proper Phase
	phase = ProcessableType::RENDER;

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

void ProcessableQueue::cleanup()
{
	shouldClose = true;

	nextFrameConditionalVariable.notify_all();
}
