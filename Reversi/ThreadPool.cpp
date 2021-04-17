#include "ThreadPool.h"

#include <iostream> // std::cout // todo remove
#include <algorithm> // std::max
#include <mutex>


void ThreadPool::WorkThread()
{
	while (!producerDone || producerCount != consumerCount)
	{
		Job job;
		if (HasPopped(job))
		{
			job();
		}
		else
		{
			std::this_thread::yield();
		}
	}
}

bool ThreadPool::HasPopped(Job& job)
{
	std::lock_guard<std::mutex> lock(mut);
	if (jobQueue.empty())
	{
		return false;
	}
	job = jobQueue.front();
	jobQueue.pop();
	return true;
}

ThreadPool::ThreadPool()
	:
	producerDone(false),
	producerCount(0),
	consumerCount(0)
{
	// define minimum number of threads if hardware_concurrency value cannot be computed 
	const size_t count = std::max(std::thread::hardware_concurrency(), 4U);

	threads.reserve(count);

	for (size_t i = 0; i < count; ++i)
	{
		threads.emplace_back(std::thread(&ThreadPool::WorkThread, this));
	}
}

ThreadPool::~ThreadPool()
{
	std::cout << "\nthread pool destructor called";

	// delays joining trhead until conditions are met 
	while (!producerDone || producerCount != consumerCount)
	{
	}

	std::cout << "\nthread pool destructor count loop ended";

	producerDone = true;

	std::cout << "\nthread pool destructor done = true";

	// joiner

	for (auto& t : threads)
	{
		std::cout << "\nthread pool for auto t ";
		if (t.joinable())
		{
			t.join();
			std::cout << "\njoining";
		}
	}
}

void ThreadPool::submit(Job&& job)
{
	std::lock_guard<std::mutex> lock(mut);
	jobQueue.push(job);
	condition.notify_one();
}