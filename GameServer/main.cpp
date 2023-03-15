#include "pch.h"
#include "CorePch.h"
#include "ThreadManager.h"

#include "RefCounting.h"
#include "Memory.h"

struct Data
{
	int _id;
};

int main()
{
	Data* d[10];
	for (int i = 0; i < 10; ++i)
		d[i] = ObjectPool<Data>::Pop();
	
	for (int i = 0; i < 5; ++i)
		ObjectPool<Data>::Push(d[i]);
}