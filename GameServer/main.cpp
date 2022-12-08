#include "pch.h"
#include <iostream>
#include "CorePch.h"

#include <thread>
#include <mutex>
#include <atomic>
#include <future>
#include <Windows.h>

#include "ThreadManager.h"

CoreGlobal core;

void ThreadMain() {
	while (true)
	{
		cout << "Hello. I\'m thread... " << LThreadId << '\n';
		this_thread::sleep_for(1s);
	}
}

int main() {
	for (int32 i = 0; i < 5; ++i)
	{
		GThreadManager->Launch(ThreadMain);
	}
	GThreadManager->Join();
}