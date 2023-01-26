#pragma once

#define OUT

/* ---------
	 Lock
   --------- */

#define USE_MANY_LOCKS(count)	Lock _locks[count];
#define USE_LOCK				USE_MANY_LOCKS(1)
#define READ_LOCK_IDX(index)	ReadLockGuard readLockGuard_##index(_locks[index], typeid(this).name());
#define READ_LOCK				READ_LOCK_IDX(0);
#define WRITE_LOCK_IDX(index)	WriteLockGuard writeLockGuard_##index(_locks[index], typeid(this).name());
#define WRITE_LOCK				WRITE_LOCK_IDX(0);

/*-------------
	Allocate
---------------*/

#ifdef _DEBUG
#define	XALLOC(type) BaseAllocator::Alloc(sizeof(type))
#define	XRELEASE(object) BaseAllocator::Release(object)
#elif
#define	xalloc(type) BaseAllocator::Alloc(sizeof(type))
#define	xrelease(object) BaseAllocator::Release(object)
#endif


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