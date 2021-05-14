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
	if (mThreadQueue.container.empty())
	{
		return false;
	}
	job = mThreadQueue.container.front();
	mThreadQueue.container.pop();
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
	const size_t count = 8;
	
	{
		std::lock_guard<std::mutex> lock(mOutputQueue.mut);
		//mSocketTracker.reserve(16);

	}

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
		mThreadQueue.container.push(job);
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
	if (mOutputQueue.container.empty())
	{
		return false;
	}
	job = mOutputQueue.container.front();
	mOutputQueue.container.pop();
	return true;
}

void ThreadPool::PushOutputQueue(const OutputJob& job)
{
	std::lock_guard<std::mutex> lock(mOutputQueue.mut);
	mOutputQueue.container.push(job);
}

bool ThreadPool::PopInputQueue(InputJob& job)
{
	std::lock_guard<std::mutex> lock(mInputQueue.mut);
	if (mInputQueue.container.empty())
	{
		return false;
	}
	job = mInputQueue.container.front();
	mInputQueue.container.pop();
	return true;
}

void ThreadPool::PushInputQueue(const InputJob& job)
{
	std::lock_guard<std::mutex> lock(mInputQueue.mut);
	mInputQueue.container.push(job);
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

void ThreadPool::PushSocketTracker(SOCKET s)
{
	std::lock_guard<std::mutex> lock(mSocketTracker.mut);
	mSocketTracker.container.emplace_back(s);
}

void ThreadPool::PopSockerTracker(SOCKET s)
{
	std::lock_guard<std::mutex> lock(mSocketTracker.mut);
	auto it = std::find(mSocketTracker.container.begin(), mSocketTracker.container.end(), s);

	auto eee = mSocketTracker.container.end();
	assert(it != mSocketTracker.container.end());
	mSocketTracker.container.erase(it);

}

bool ThreadPool::SockerActive(SOCKET s)
{
	std::lock_guard<std::mutex> lock(mSocketTracker.mut);
	const auto it = std::find(mSocketTracker.container.begin(), mSocketTracker.container.end(), s);

	return it != mSocketTracker.container.end();
}

//ReversiSFML ThreadPool::GetMainData() const
//{
//	return *pMainData;
//}
