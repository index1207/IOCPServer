#pragma once

#include "Allocator.h"

class MemoryPool;


/*------------
	 Memory
  ------------*/

class Memory
{
	enum
	{
		/* ~1024까지 32단위, ~2048까지 128단위, 4096까지 256단위 */
		POOL_COUNT = (1024 / 32) + (1024 / 128) + (2048 / 256),
		MAX_ALLOC_SIZE = 4096
	};
public:
	Memory();
	~Memory();

	void* Allocate(int32 size);
	void  Release(void* ptr);
private:
	vector<MemoryPool*> _pools;

	MemoryPool* _poolTable[MAX_ALLOC_SIZE + 1];
};

template<class Type, class ...Args>
inline Type* xnew(Args&&... args)
{
#ifdef _STOMP
	Type* memory = static_cast<Type*>(StompAllocator::Alloc(sizeof(Type)));
#else
	Type* memory = static_cast<Type*>(PoolAllocator::Alloc(sizeof(Type)));
#endif
	new(memory)Type(forward<Args>(args)...);
	return memory;
}

template<class Type>
inline void xdelete(Type* ptr)
{
	ptr->~Type();
#ifdef _STOMP
	StompAllocator::Release(ptr);
#else
	PoolAllocator::Release(ptr);
#endif // _STOMP
}

template<class Type>
inline shared_ptr<Type> MakeShared()
{
	return shared_ptr<Type>{xnew<Type>(), xdelete<Type>};
}