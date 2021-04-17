#include <mutex>		// std::mutex std::condition_variable std::atomic std::thread
#include <queue>		// std::queue std::vector
#include<functional>	// std::function
//#include <thread>

typedef std::function<void()> Job;

class ThreadPool
{
private:
	// for thread safe queue
	std::mutex mut;
	// notify one thread when job is available
	std::condition_variable condition;
	//for queue
	std::queue<Job> jobQueue;
	// a thread in the container is nofified when a job becomes available
	std::vector<std::thread> threads;
public:
	// mod in producer
	std::atomic_int producerCount;
	// on last consumer of each chain
	std::atomic_int consumerCount;
	std::atomic_bool producerDone;
private:
	// Thread takes a job from the queue and executes it
	void WorkThread();
	// attempts to pop a job off the queue with lock guard. Returns if successed
	bool HasPopped(Job& job);
public:
	// constructs threads based on thread::hardware_concurrency
	ThreadPool();
	//	:
	//	producerDone(false),
	//	producerCount(0),
	//	consumerCount(0)
	//{
	//	// define minimum number of threads if hardware_concurrency value cannot be computed 
	//	const size_t count = std::max(std::thread::hardware_concurrency(), 4U);

	//	threads.reserve(count);

	//	for (size_t i = 0; i < count; ++i)
	//	{
	//		threads.emplace_back(std::thread(&ThreadPool::WorkThread, this));
	//	}
	//}
		
	// joins threads when jobs have been completed
	~ThreadPool();
	// adds rvalue jobs to the queue and notifies an available thread
	void submit(Job&& f);
};

