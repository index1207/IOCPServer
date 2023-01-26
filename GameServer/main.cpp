#include "pch.h"
#include "CorePch.h"
#include "ThreadManager.h"
#include "memory.h"

class Knight
{
public:
	Knight()
	{
		cout << "Knight()\n";
	}
	~Knight()
	{
		cout << "~Knight()\n";
	}
};

int main()
{
	auto knight = xnew<Knight>();
	xdelete(knight);
}