#include <mutex>		// std::mutex std::condition_variable std::atomic std::thread
#include <queue>		// std::queue std::vector
#include<functional>	// std::function
#include "OutputJobs.h"
#include <WinSock2.h>


class ReversiSFML;

typedef std::function<void()> ThreadJob;
// process functors in host
typedef std::function<void(ReversiSFML*)> OutputJob;
// send data from client to server
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
	// thread safe queue for thread management
	MutexQueue<ThreadJob> mThreadQueue;
	// thread safe queue for updating host
	MutexQueue<OutputJob> mOutputQueue;
	// thread safe queue for passing data to client socket
	MutexQueue<ClientSendData> mInputQueue;
	// notify one thread when job is available
	std::condition_variable condition;
	// a thread in the container is nofified when a job becomes available
	std::vector<std::thread> threads;
	// connected sockets available to main thread
	MutexVector<SOCKET> mSocketTracker;
	// access main app structure
	ReversiSFML* const pMainData;
	std::mutex mServerDataMutex;
	// updates client 
	ServerSendData mServerData;
public:
	SOCKET mListener;

	enum Type{END = -1, NONE = 0,CLIENT_SOCKET,SERVER_LISTEN,SERVER_SOCKETS};
	enum {MAX_PLAYERS = 4, THREAD_SIZE = 8};
	// 
	std::atomic_int socketType = Type::NONE;
	std::atomic_int socketCount = 0;
	

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
	// adds rvalue jobs to the queue and notifies an available thread to input queue
	void PushThreadQueue(const ThreadJob& f, Type t);
	// from outputQueue to be used in main thread 
	bool PopOutputQueue(OutputJob& job);
	// to outputQueue to be used in thread jobs for exporting data to main thread
	void PushOutputQueue(const OutputJob& job);
	// takes font job off the input queue
	bool PopInputQueue(InputJob& job);
	// adds jobs to input queue
	void PushInputQueue(const InputJob& job);
	// updates client
	void UpdateServerData(const ServerSendData& l);
	// copy of host data
	ServerSendData GetServerData();
	// server thread pool new client tracker
	void PushSocketTracker(SOCKET s);
	// server thread pool client tracker removed
	void PopSockerTracker(SOCKET s);
	// returns if the socket parameter is active
	bool SocketActive(SOCKET s);
};

