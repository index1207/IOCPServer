#pragma once

/*--------------------
	  BaseAllocator
  --------------------*/

class BaseAllocator
{
public:
	static void* Alloc(size_t size);
	static void Release(void* ptr);
};

/*------------------
	StompAllocator
  ------------------*/

class StompAllocator
{
	enum
	{
		PAGE_SIZE = 0x1000
	};
public:
	static void* Alloc(size_t size);
	static void Release(void* ptr);
};

/*------------------
	 STL Allocator
  ------------------*/
template<typename T>
class STLAllocator
{
public:
	using value_type = T;

	STLAllocator() { };

	template<typename Other>
	STLAllocator(const STLAllocator<Other>&) { }

	T* allocate(size_t count)
	{
		const int32 size = count * sizeof(T);
		return static_cast<T*>(XALLOC(size));
	}

	void deallocate(T* ptr, size_t count)
	{
		XRELEASE(ptr);
	}
};