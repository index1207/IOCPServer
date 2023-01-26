#pragma once

/*----------------
	RefCountable
------------------*/

class RefCountable
{
public:
	RefCountable() : _refCount(1) {  }
	virtual ~RefCountable() {  }

	int32 GetRefCount();
	int32 AddRef();
	int32 ReleaseRef();
protected:
	atomic<int32> _refCount;
};

/*-----------------
	  SharedPtr
-------------------*/

template<class T>
class SharedPtr
{
public:
	SharedPtr() {}
	SharedPtr(T* ptr) { Set(ptr); }

	SharedPtr(const SharedPtr& rhs) { Set(rhs._ptr); }

	SharedPtr(SharedPtr&& rhs) 
	{ 
		_ptr = rhs._ptr;
		rhs._ptr = nullptr;
	}

	template<class U>
	SharedPtr(const SharedPtr<U>& rhs) { Set(static_cast<T>(rhs._ptr)); }

	~SharedPtr() { Release(); }
public:
	SharedPtr& operator=(const SharedPtr& rhs)
	{
		if (_ptr != rhs._ptr)
		{
			Release();
			Set(rhs._ptr);
		}
		return *this;
	}
	SharedPtr& operator=(SharedPtr&& rhs)
	{
		Release();
		_ptr = rhs._ptr;
		rhs._ptr = nullptr;
		return *this;
	}
	
	bool operator==(const SharedPtr& rhs) const
	{
		return _ptr == rhs._ptr;
	}
	bool operator==(T* ptr) const
	{
		return _ptr == ptr;
	}
	bool operator!=(const SharedPtr& rhs) const
	{
		return _ptr != rhs._ptr;
	}
	bool operator!=(T* ptr) const
	{
		return _ptr != ptr;
	}
	bool operator<(const SharedPtr& rhs) const
	{
		return _ptr < rhs._ptr;
	}
	bool operator>(const SharedPtr& rhs) const
	{
		return _ptr > rhs._ptr;
	}
	T* operator*() { return _ptr; }
	const T* operator*() const { return _ptr; }
	operator T* () const { _ptr; }
	T* operator->() { return _ptr; }
	const T* operator->() { return _ptr; }
public:
	bool isNull() { return _ptr == nullptr; }
private:
	inline void Set(T* ptr)
	{
		_ptr = ptr;
		if (ptr) ptr->AddRef();
	}
	inline void Release()
	{
		if (_ptr != nullptr)
		{
			_ptr->ReleaseRef();
			_ptr = nullptr;
		}
	}
private:
	T* _ptr = nullptr;
};