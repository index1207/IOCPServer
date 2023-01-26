#pragma once

#include "Allocator.h"

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