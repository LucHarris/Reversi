#include "ThreadPool.h"

#include <iostream> // std::cout // todo remove
#include <algorithm> // std::max
#include <mutex>


void ThreadPool::WorkThread()
{
	while (socketType > Type::END)
	{
		ThreadJob job;
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

bool ThreadPool::HasPopped(ThreadJob& job)
{
	std::lock_guard<std::mutex> lock(mThreadQueue.mut);
	if (mThreadQueue.jobQueue.empty())
	{
		return false;
	}
	job = mThreadQueue.jobQueue.front();
	mThreadQueue.jobQueue.pop();
	return true;
}

bool ThreadPool::ValidateThread(Type t)
{
	bool r = true;
	switch (t)
	{
	case Type::END:
	case Type::NONE:
		// type must be CLIENT_SOCKET, SERVER_LISTEN or SERVER_SOCKETS
		r = false;
		break;
	case Type::CLIENT_SOCKET:
		if (socketType > Type::CLIENT_SOCKET || socketType == Type::END)
		{
			r = false;
		}
		break;
	case Type::SERVER_LISTEN:
		if (socketType == Type::END || socketType == Type::SERVER_SOCKETS)
		{
			r = false;
		}
		break;

	case Type::SERVER_SOCKETS:
		if (socketType <= Type::CLIENT_SOCKET)
		{
			r = false;
		}
		break;
	}
	return r;
}

ThreadPool::ThreadPool(ReversiSFML* pld)
	:
	pMainData(pld)
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
	socketType = Type::END;

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

void ThreadPool::PushThreadQueue(const ThreadJob& job, Type t)
{

	if (ValidateThread(t))
	{
		socketType = t;
		std::lock_guard<std::mutex> lock(mThreadQueue.mut);
		mThreadQueue.jobQueue.push(job);
		condition.notify_one();
	}
	else
	{
		assert(false);
	}
}

bool ThreadPool::PopOutputQueue(OutputJob& job)
{
	std::lock_guard<std::mutex> lock(mOutputQueue.mut);
	if (mOutputQueue.jobQueue.empty())
	{
		return false;
	}
	job = mOutputQueue.jobQueue.front();
	mOutputQueue.jobQueue.pop();
	return true;
}

void ThreadPool::PushOutputQueue(const OutputJob& job)
{
	std::lock_guard<std::mutex> lock(mOutputQueue.mut);
	mOutputQueue.jobQueue.push(job);
}

bool ThreadPool::PopInputQueue(InputJob& job)
{
	std::lock_guard<std::mutex> lock(mInputQueue.mut);
	if (mInputQueue.jobQueue.empty())
	{
		return false;
	}
	job = mInputQueue.jobQueue.front();
	mInputQueue.jobQueue.pop();
	return true;
}

void ThreadPool::PushInputQueue(const InputJob& job)
{
	std::lock_guard<std::mutex> lock(mInputQueue.mut);
	mInputQueue.jobQueue.push(job);
}

void ThreadPool::UpdateServerData(const ServerSendData& l)
{
	std::lock_guard<std::mutex> lock(mServerDataMutex);
	mServerData = l;
}

ServerSendData ThreadPool::GetServerData()
{
	std::lock_guard<std::mutex> lock(mServerDataMutex);
	return mServerData;
}

//ReversiSFML ThreadPool::GetMainData() const
//{
//	return *pMainData;
//}
