#pragma once
#include "stdfx.h"

#include "ThreadSafeQueue.h"
#include "ThreadPoolJoiner.h"

namespace ThreadPool
{
	template <typename FUNCTION>
	class ThreadPool
	{
	public:
		ThreadPool();
		auto AddTask(FUNCTION task) -> std::future<decltype(task())>;

	private:
		void WorkerThread();
		void ObtainAndExecuteTask();

	private:
		ThreadSafeContainers::ThreadSafeQueue<std::packaged_task<FUNCTION>> tasks;
		std::atomic<bool> stoped;
		std::vector<std::thread> threads;
		ThreadUtils::ThreadPoolJoiner threadsJoiner;
	};
}

namespace ThreadPool
{
	template <typename FUNCTION>
	ThreadPool<FUNCTION>::ThreadPool()
		: stoped(false),
		threadsJoiner(threads)
	{
		const int maxThreadsCount = std::thread::hardware_concurrency();
		for (int threadNumber = 0; threadNumber < maxThreadsCount; ++threadNumber)
		{
			threads.push_back(std::move(std::thread(&ThreadPool::WorkerThread, this)));
		}
	}

	template <typename FUNCTION>
	auto ThreadPool<FUNCTION>::AddTask(FUNCTION task) -> std::future<decltype(task())>
	{
		std::packaged_task<FUNCTION> packagedTask(task);
		auto future = packagedTask.get_future();
		tasks.Push(std::move(packagedTask));
		return future;
	}

	template <typename FUNCTION>
	void ThreadPool<FUNCTION>::ObtainAndExecuteTask()
	{
		if (auto threadTask = tasks.TryPop())
		{
			(*threadTask)();
		}
		else
		{
			std::this_thread::yield();
		}
	}

	template <typename FUNCTION>
	void ThreadPool<FUNCTION>::WorkerThread()
	{
		while (!stoped.load())
		{
			try
			{
				ObtainAndExecuteTask();
			}
			catch (const std::exception& ex)
			{
				stoped.store(true);
			}
		}
	}
}
