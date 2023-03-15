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
		Type* memory = static_cast<Type*>(MemoryHeader::AttachHeader(s_pool.pop(), s_allocSize));
		new(memory)Type(std::forward<Args>(args)...);
		return memory;
	}

	static void Push(Type* ptr)
	{
		ptr->~Type();
		MemoryHeader::DetachHeader(ptr);
	}

	static std::shared_ptr<Type> MakeShared()
	{
		return std::shared_ptr<Type>{ Pop(), Push };
	}
private:
	static int32 s_allocSize;
	static MemoryPool s_pool;
};

template<typename Type>
int32 ObjectPool<Type>::s_allocSize = sizeof(Type) + sizeof(MemoryHeader);

template<typename Type>
MemoryPool ObjectPool<Type>::s_pool { s_allocSize };