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
	PoolAllocator
  ------------------*/

class PoolAllocator
{
public:
	static void* Alloc(size_t size);
	static void Release(void* ptr);
};


/*------------------
	 STL Allocator
  ------------------*/
template<typename Type>
class STLAllocator
{
public:
	using value_type = Type;

	STLAllocator() { };

	template<typename Other>
	STLAllocator(const STLAllocator<Other>&) { }

	Type* allocate(size_t count)
	{
		const int32 size = count * sizeof(Type);
		return static_cast<Type*>(XALLOC(size));
	}

	void deallocate(Type* ptr, size_t count)
	{
		XRELEASE(ptr);
	}
};