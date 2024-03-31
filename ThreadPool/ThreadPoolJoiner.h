#pragma once

namespace ThreadUtils
{
	class ThreadPoolJoiner
	{
	public:
		ThreadPoolJoiner(std::vector<std::thread>& workingThreadsToJoin);
		~ThreadPoolJoiner();

	private:
		std::vector<std::thread>& threadsToJoin;
	};
}