#include "pch.h"
#include "Lock.h"
#include "CoreTLS.h"

void Lock::WriteLock()
{
	const uint32 lockThreadId = (_lockFlag.load() & WRITE_THREAD_MASK) >> 16;
	if (LThreadId == lockThreadId) {
		_writeCount++;
		return; 
	}

	const auto beginTick = GetTickCount64();
	const uint32 desired = ((LThreadId << 16) & WRITE_THREAD_MASK);

	while (true) {
		for (uint32 spinCount = 0; spinCount < MAX_SPIN_COUNT; ++spinCount) {
			uint32 expected = EMPTY_FLAG;
			if (_lockFlag.compare_exchange_strong(OUT expected, desired)) {
				_writeCount++;
				return;
			}
		}
		if (GetTickCount64() - beginTick >= AQUIRE_TIMEOUT_TICK) {
			CRASH("LOCK_TIMEOUT");
		}
		this_thread::yield();
	}
}

void Lock::WriteUnlock()
{
}

void Lock::ReadLock()
{
}

void Lock::ReadUnlock()
{
}
