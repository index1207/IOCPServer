#include "pch.h"
#include <iostream>
#include "CorePch.h"

#include <thread>
#include <mutex>
#include <atomic>
#include <future>
#include <Windows.h>

int main() {
	atomic<int64> num;
	cout << boolalpha << num.is_lock_free() << '\n';

	struct A {
		int32 a;
		int32 b;
		int32 c;
	};
	atomic<A> a;
	cout << boolalpha << a.is_lock_free() << '\n';
}