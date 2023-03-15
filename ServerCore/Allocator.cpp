#include "pch.h"
#include "Allocator.h"
#include "Memory.h"


/*--------------------
	  BaseAllocator
  --------------------*/

void* BaseAllocator::Alloc(size_t size)
{
	return ::malloc(size);
}

void BaseAllocator::Release(void* ptr)
{
	free(ptr);
	ptr = nullptr;
}


/*------------------
	StompAllocator
  ------------------*/

void* StompAllocator::Alloc(size_t size)
{
	const int32 pageCount = (size + PAGE_SIZE - 1) / PAGE_SIZE;
	const int32 dataOffset = pageCount * PAGE_SIZE - size;

	void* objAddr = ::VirtualAlloc(NULL, pageCount * PAGE_SIZE, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	return static_cast<void*>(static_cast<int8*>(objAddr) + dataOffset);
}

void StompAllocator::Release(void* ptr)
{
	const int64 addr = reinterpret_cast<int64>(ptr);
	const int64 baseAddr = addr - (addr % PAGE_SIZE);
	::VirtualFree(reinterpret_cast<void*>(baseAddr), 0, MEM_RELEASE);
}


/*------------------
	PoolAllocator
  ------------------*/

void* PoolAllocator::Alloc(size_t size)
{
	return GMemory->Allocate(size);
}

void PoolAllocator::Release(void* ptr)
{
	GMemory->Release(ptr);
}