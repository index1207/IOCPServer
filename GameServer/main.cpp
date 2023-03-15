#include "pch.h"
#include "CorePch.h"
#include "ThreadManager.h"

#include "RefCounting.h"
#include "Memory.h"

struct Data
{
	Data() {
		_id++;
	}
	static int _id;
	char str[12];
};

int Data::_id = 0;

int main()
{
	auto ptr1 = MakeShared<Data>();
	cout << ptr1->_id << '\n';
	auto ptr2 = ObjectPool<Data>::Pop();
	cout << ptr2->_id << '\n';
}