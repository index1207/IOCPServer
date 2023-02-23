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
	Type* memory = static_cast<Type*>(XALLOC(sizeof(Type)));
	new(memory)Type(forward<Args>(args)...);
	return memory;
}

template<class Type>
inline void xdelete(Type* ptr)
{
	ptr->~Type();
	XRELEASE(ptr);
}