#include "pch.h"
#include "CoreGlobal.h"
#include "ThreadManager.h"
#include "Memory.h"
#include "DeadLockProfiler.h"
#include "SocketUtil.h"

ThreadManager* GThreadManager = nullptr;
Memory* GMemory = nullptr;
DeadLockProfiler* GDeadLockProfiler = nullptr;

class CoreGlobal
{
public:
	CoreGlobal()
	{
		GThreadManager = new ThreadManager();
		GMemory = new Memory();
		GDeadLockProfiler = new DeadLockProfiler();
		SocketUtil::Init();
	}
	~CoreGlobal()
	{
		delete GDeadLockProfiler;
		delete GMemory;
		delete GThreadManager;
		SocketUtil::Clear();
	}
} CoreGlobal{};
