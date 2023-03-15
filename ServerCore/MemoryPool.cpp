#include "pch.h"
#include "MemoryPool.h"

/*---------------
	Memory Pool
  ---------------*/

MemoryPool::MemoryPool(int32 size) : _allocSize(size)
{
	::InitializeSListHead(&this->_header);
}

MemoryPool::~MemoryPool()
{
	while (MemoryHeader* memory = static_cast<MemoryHeader*>(InterlockedPopEntrySList(&_header)))
	{
		::_aligned_free(memory);
	}
}

void MemoryPool::push(MemoryHeader* ptr)
{
	ptr->allocSize = 0;

	::InterlockedPushEntrySList(&_header, static_cast<SLIST_ENTRY*>(ptr));

	_useCount.fetch_sub(1);
	_reserveCount.fetch_add(1);
}

MemoryHeader* MemoryPool::pop()
{
	MemoryHeader* memory = static_cast<MemoryHeader*>(::InterlockedPopEntrySList(&_header));

	if (memory == nullptr)
	{
		memory = reinterpret_cast<MemoryHeader*>(::_aligned_malloc(_allocSize, SLIST_ALIGNMENT));
	}
	else
	{
		ASSERT_CRASH(memory->allocSize == 0);
		_reserveCount.fetch_sub(1);
	}

	_useCount.fetch_add(1);

	return memory;
}
