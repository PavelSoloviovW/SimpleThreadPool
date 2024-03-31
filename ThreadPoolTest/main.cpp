#include <iostream>
#include <Windows.h>

#include "ThreadSafeQueue.h"
#include "ThreadPool.h"

int main()
{
	auto tmp = []() {
		Sleep(2000);
		std::cout << "done\n";
		return false;
		};

	auto task = []() {
		Sleep(2000);
		return 123;
		};

	std::shared_ptr<std::packaged_task<int()>> sharedTask = std::make_shared<std::packaged_task<int()>>(std::move(task));
	sharedTask;

	ThreadPool::ThreadPool<bool()> TP;
	auto res = TP.AddTask(tmp);
	auto res_1 = TP.AddTask(tmp);
	auto res_2 = TP.AddTask(tmp);
	auto res_3 = TP.AddTask(tmp);

	Sleep(5000);
	auto tmp_0 = res.get();
	auto tmp_1 = res_1.get();
	auto tmp_2 = res_2.get();
	auto tmp_3 = res_3.get();
	tmp_0;
}