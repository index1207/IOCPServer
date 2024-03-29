#pragma once
#include "Types.h"
#include "MemoryPool.h"

template<typename Type>
class ObjectPool
{
public:
	template<typename ...Args>
	static Type* Pop(Args&& ...args)
	{
#ifdef _STOMP
		MemoryHeader* ptr = reinterpret_cast<MemoryHeader*>(StompAllocator::Alloc(s_allocSize));
		Type* memory = static_cast<Type*>(MemoryHeader::AttachHeader(ptr, s_allocSize));
#else
		Type* memory = static_cast<Type*>(MemoryHeader::AttachHeader(s_pool.pop(), s_allocSize));
#endif // _STOMP

		new(memory)Type(std::forward<Args>(args)...);
		return memory;
	}

	static void Push(Type* ptr)
	{
		ptr->~Type();
#ifdef _STOMP
		StompAllocator::Release(MemoryHeader::DetachHeader(ptr));
#else
		MemoryHeader::DetachHeader(ptr);
#endif // _STOMP
	}

	template<class... Args>
	static inline std::shared_ptr<Type> MakeShared(Args&&... args)
	{
		return std::shared_ptr<Type>{ Pop(std::forward(args)...), Push };
	}
private:
	static int32 s_allocSize;
	static MemoryPool s_pool;
};

template<typename Type>
int32 ObjectPool<Type>::s_allocSize = sizeof(Type) + sizeof(MemoryHeader);

template<typename Type>
MemoryPool ObjectPool<Type>::s_pool { s_allocSize };