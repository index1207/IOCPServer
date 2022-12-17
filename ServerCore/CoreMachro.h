#pragma once

#define OUT

/* ---------
	 Lock
   --------- */

#define USE_MANY_LOCKS(count)	Lock _locks[count];
#define USE_LOCK				USE_MANY_LOCKS(1)
#define READ_LOCK_IDX(index)	ReadLockGuard readLockGuard_##index(_locks[index]);
#define READ_LOCK				READ_LOCK_IDX(0);
#define WRITE_LOCK_IDX(index)	WriteLockGuard writeLockGuard_##index(_locks[index]);
#define WRITE_LOCK				WRITE_LOCK_IDX(0);


/* ---------
     Crash
   --------- */

#define CRASH(cause)					 \
{										 \
	uint32* crash = nullptr;			 \
	__analysis_assume(crash != nullptr); \
	*crash = 0xDEADBEEF;			 	 \
}

#define ASSERT_CRASH(expr) \
{ \
	if (!(expr)) { \
		CRASH("ASSERT_CRASH"); \
		__analysis_assume(expr); \
	} \
}