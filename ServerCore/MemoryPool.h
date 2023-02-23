#pragma once

/*------------------
	 MemoryHeader
  ------------------*/

struct MemoryHeader
{
	MemoryHeader(int32 size) : allocSize(size) {  }

	static void* AttachHeader(MemoryHeader* header, int32 size)
	{
		new(header)MemoryHeader(size);
		return reinterpret_cast<void*>(++header);
	}
	
	static MemoryHeader* DetachHeader(void* ptr)
	{
		MemoryHeader* header = reinterpret_cast<MemoryHeader*>(ptr) - 1;
		return header;
	}
	int32 allocSize;
};

/*------------------
	  MemoryPool
  ------------------*/

class MemoryPool
{
	USE_LOCK
public:
	MemoryPool(int32 size);
	~MemoryPool();

	void push(MemoryHeader* ptr);
	MemoryHeader* pop();
private:
	int32 _allocSize;
	atomic<int32> _allocCount;

	queue<MemoryHeader*> _queue;
};

