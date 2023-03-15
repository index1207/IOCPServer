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

template<class Type>
class SharedPtr
{
public:
	SharedPtr() {}
	SharedPtr(Type* ptr) { Set(ptr); }

	SharedPtr(const SharedPtr& rhs) { Set(rhs._ptr); }

	SharedPtr(SharedPtr&& rhs) 
	{ 
		_ptr = rhs._ptr;
		rhs._ptr = nullptr;
	}

	template<class U>
	SharedPtr(const SharedPtr<U>& rhs) { Set(static_cast<Type>(rhs._ptr)); }

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
	bool operator==(Type* ptr) const
	{
		return _ptr == ptr;
	}
	bool operator!=(const SharedPtr& rhs) const
	{
		return _ptr != rhs._ptr;
	}
	bool operator!=(Type* ptr) const
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
	Type* operator*() { return _ptr; }
	const Type* operator*() const { return _ptr; }
	operator Type* () const { _ptr; }
	Type* operator->() { return _ptr; }
	const Type* operator->() { return _ptr; }
public:
	bool isNull() { return _ptr == nullptr; }
private:
	inline void Set(Type* ptr)
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
	Type* _ptr = nullptr;
};