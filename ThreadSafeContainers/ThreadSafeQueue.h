#pragma once
#include <queue>
#include <mutex>
#include <memory>
#include <thread>
#include <future>
#include <type_traits>

namespace ThreadSafeContainers
{
	template<typename T>
	class ThreadSafeQueue
	{
	public:
		ThreadSafeQueue();

		void Push(T&&);
		std::shared_ptr<T> WaitAndPop();
		std::shared_ptr<T> TryPop();
		bool Empty() const;

	private:
		std::queue<std::shared_ptr<T>> data;
		std::mutex mutex;
		std::condition_variable condVar;
	};
}

namespace ThreadSafeContainers
{
	template<typename T>
	ThreadSafeQueue<T>::ThreadSafeQueue()
	{
	}

	template<typename T>
	void ThreadSafeQueue<T>::Push(T&& newItem)
	{
		std::shared_ptr<T> item_ptr = std::make_shared<T>(std::move(newItem));
		std::lock_guard<std::mutex> lock(mutex);
		data.push(item_ptr);
		condVar.notify_one();
	}

	template<typename T>
	std::shared_ptr<T> ThreadSafeQueue<T>::WaitAndPop()
	{
		std::unique_lock<std::mutex> condLock(mutex);
		condVar.wait(condLock, [this]() {return !this->data.empty();});

		auto outValue = data.front();
		data.pop();
		return outValue;
	}

	template<typename T>
	std::shared_ptr <T> ThreadSafeQueue<T>::TryPop()
	{
		std::lock_guard<std::mutex> lock(mutex);
		if (data.empty())
		{
			throw std::runtime_error("can not get varible from ThreadSafeQueue queue is empty");
		}
		const auto outValue = data.front();
		data.pop();
		return outValue;
	}

	template<typename T>
	bool ThreadSafeQueue<T>::Empty() const
	{
		std::lock_guard<std::mutex> lock(mutex);
		return data.empty();
	}
}