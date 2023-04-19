#include "pch.h"
#include "CorePch.h"
#include <iostream>

#include "ThreadManager.h"

#include "SocketUtil.h"
#include "Listener.h"

#pragma comment(lib, "ws2_32.lib")

int main()
{
	Listener listener;
	listener.StartAccept(NetAddress(L"127.0.0.1", 7777));

	for (int32 i = 0; i < 5; ++i)
	{
		GThreadManager->Launch([=]() {
			while (true)
			{
				GIocpCore.Dispatch();
			}
		});
	}
	GThreadManager->Join();

	return 0;
}