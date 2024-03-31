#include "stdfx.h"
#include "ThreadPoolJoiner.h"

namespace ThreadUtils
{
	ThreadPoolJoiner::ThreadPoolJoiner(std::vector<std::thread>& workingThreadsToJoin)
		: threadsToJoin(workingThreadsToJoin)
	{
	}

	ThreadPoolJoiner::~ThreadPoolJoiner()
	{
		for (auto& thread : threadsToJoin)
		{
			thread.join();
		}
	}
}