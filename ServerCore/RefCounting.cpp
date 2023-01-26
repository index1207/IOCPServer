#include "pch.h"
#include "RefCounting.h"

int32 RefCountable::GetRefCount()
{
	return _refCount;
}

int32 RefCountable::AddRef()
{
	return ++_refCount;
}

int32 RefCountable::ReleaseRef()
{
	int32 refCount = --_refCount;
	if (refCount == 0)
	{
		delete this;
	}
	return refCount;
}
