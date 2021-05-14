#include <mutex>		// std::mutex std::condition_variable std::atomic std::thread
#include <queue>		// std::queue std::vector
#include<functional>	// std::function
#include "OutputJobs.h"
//#include "ReversiSFML.h"
//#include <thread>
#include <WinSock2.h>


class ReversiSFML;

typedef std::function<void()> ThreadJob;
typedef std::function<void(ReversiSFML*)> OutputJob;
//typedef std::function<void(ReversiSFML*)> InputJob;
typedef ClientSendData InputJob;


template<class J>
struct MutexQueue
{
	std::mutex mut;
	std::queue<J> container;
};

template<class J>
struct MutexVector
{
	std::mutex mut;
	std::vector<J> container;
};

class ThreadPool
{
private:
	// for thread safe queue

	MutexQueue<ThreadJob> mThreadQueue;
	MutexQueue<OutputJob> mOutputQueue;
	MutexQueue<ClientSendData> mInputQueue;
	//std::mutex mut;
	// notify one thread when job is available
	std::condition_variable condition;
	//for queue
	//std::queue<ThreadJob> container;
	// a thread in the container is nofified when a job becomes available
	std::vector<std::thread> threads;
	MutexVector<SOCKET> mSocketTracker;
	ReversiSFML* const pMainData;

	std::mutex mServerDataMutex;
	ServerSendData mServerData;
public:

	// mod in producer
	//std::atomic_int producerCount;
	// on last consumer of each chain
	//std::atomic_int consumerCount;
	//std::atomic_bool producerDone;


	enum Type{END = -1, NONE = 0,CLIENT_SOCKET,SERVER_LISTEN,SERVER_SOCKETS};
	// 
	std::atomic_int socketType = Type::NONE;

private:
	// Thread takes a job from the queue and executes it
	void WorkThread();
	// attempts to PopOutputQueue a job off the queue with lock guard. Returns if successed
	// from input queue
	bool HasPopped(ThreadJob& job);

	bool ValidateThread(Type t);

public:
	// constructs threads based on thread::hardware_concurrency
	ThreadPool(ReversiSFML* pld);
		
	// joins threads when jobs have been completed
	~ThreadPool();
	// adds rvalue jobs to the queue and notifies an available thread
	// to input queue
	void PushThreadQueue(const ThreadJob& f, Type t);

	// from outputQueue to be used in main thread 
	bool PopOutputQueue(OutputJob& job);

	// to outputQueue to be used in thread jobs for exporting data to main thread
	void PushOutputQueue(const OutputJob& job);


	bool PopInputQueue(InputJob& job);

	void PushInputQueue(const InputJob& job);

	void UpdateServerData(const ServerSendData& l);

	ServerSendData GetServerData();

	// server thread pool new client tracker
	void PushSocketTracker(SOCKET s);
	// server thread pool client tracker removed
	void PopSockerTracker(SOCKET s);

	bool SockerActive(SOCKET s);
};

