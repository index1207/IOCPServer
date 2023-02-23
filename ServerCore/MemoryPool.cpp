#include "pch.h"
#include "MemoryPool.h"

MemoryPool::MemoryPool(int32 size) : _allocSize(size)
{
}

MemoryPool::~MemoryPool()
{
	while (!_queue.empty()) {
		MemoryHeader* header = _queue.front();
		_queue.pop();
		::free(header);
	}
}

void MemoryPool::push(MemoryHeader* ptr)
{
	WRITE_LOCK;
	ptr->allocSize = 0;

	_queue.push(ptr);
	_allocCount.fetch_sub(1);
}

MemoryHeader* MemoryPool::pop()
{
	MemoryHeader* header = nullptr;
	{
		WRITE_LOCK;

		if (!_queue.empty())
		{
			header = _queue.front();
			_queue.pop();
		}
	}

	if (header == nullptr)
	{
		header = reinterpret_cast<MemoryHeader*>(::malloc(_allocSize));
	}
	else
	{
		ASSERT_CRASH(header->allocSize == 0);
	}

	_allocCount.fetch_add(1);

	return nullptr;
}
