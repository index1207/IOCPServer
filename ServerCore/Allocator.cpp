#include "pch.h"
#include "Allocator.h"

void* BaseAllocator::Alloc(size_t size)
{
	return ::malloc(size);
}

void BaseAllocator::Release(void* ptr)
{
	free(ptr);
}
