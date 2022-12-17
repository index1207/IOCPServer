#include "pch.h"
#include "CorePch.h"
#include "ThreadManager.h"

class TestLock
{
	USE_LOCK;
public:
	TestLock()
	{
		srand(time(nullptr));
	}
	int32 ReadTest()
	{
		READ_LOCK;
		if (_que.empty()) return -1;
		return _que.front();
	}
	void PushTest()
	{
		WRITE_LOCK;
		_que.push(rand() % 100);
	}
	void PopTest()
	{
		WRITE_LOCK;
		if (!_que.empty())
			_que.pop();
	}
private:
	queue<int32> _que;
};

TestLock t;

void WriteThread()
{
	while (true)
	{
		t.PushTest();
		this_thread::sleep_for(1ms);
		t.PopTest();
	}
}

void ReadThread()
{
	while (true)
	{
		auto value = t.ReadTest();
		cout << value << '\n';
		this_thread::sleep_for(1ms);
	}
}

int main()
{
	for (int32 i = 0; i < 2; ++i)
	{
		GThreadManager->Launch(WriteThread);
	}
	for (int32 i = 0; i < 5; ++i)
	{
		GThreadManager->Launch(ReadThread);
	}
	GThreadManager->Join();
}