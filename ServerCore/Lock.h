#pragma once

/* ----------------------------------------
					RW Lock

    [WWWWWWWW][WWWWWWWW][RRRRRRR][RRRRRRR]
	 * 상위 16bit : Write Flag (Thread Id)
	 * 하위 16bit : Read Flag (Read Count)
   ---------------------------------------- */

class Lock
{
	enum : uint32
	{
		AQUIRE_TIMEOUT_TICK = 10000,
		MAX_SPIN_COUNT = 5000,
		WRITE_THREAD_MASK = 0xFFFF'0000,
		READ_COUNT_MASK = 0x0000'FFFF,
		EMPTY_FLAG = 0x0000'0000
	};
public:
	void WriteLock(const char* name);
	void WriteUnlock(const char* name);
	void ReadLock(const char* name);
	void ReadUnlock(const char* name);
private:
	Atomic<uint32> _lockFlag = EMPTY_FLAG;
	uint16 _writeCount = 0;
};

/* ----------------
	Lock Guards
    (RAII Pattern)
   ---------------- */

class ReadLockGuard
{
public:
	ReadLockGuard(Lock& lock, const char* name) : _lock(lock), _name(name)
	{
		_lock.ReadLock(name);
	}
	~ReadLockGuard()
	{
		_lock.ReadUnlock(_name);
	}
private:
	const char* _name;
	Lock& _lock;
};

class WriteLockGuard
{
public:
	WriteLockGuard(Lock& lock, const char* name) : _lock(lock), _name(name)
	{
		_lock.WriteLock(name);
	}
	~WriteLockGuard()
	{
		_lock.WriteUnlock(_name);
	}
private:
	const char* _name;
	Lock& _lock;
};