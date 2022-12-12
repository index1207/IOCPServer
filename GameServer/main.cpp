#include "pch.h"
#include "CorePch.h"

int main() {
	atomic<int> a = 5;
	int b = 10;
	int c = 20;

	cout << a.compare_exchange_strong(b, c) << '\n';
	cout << a.compare_exchange_strong(b, c) << '\n';
}