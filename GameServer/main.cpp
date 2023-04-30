#include "pch.h"
#include "CorePch.h"
#include <iostream>

#include "ThreadManager.h"

#include "SocketUtil.h"
#include "Service.h"
#include "Session.h"
#include "Listener.h"

#pragma comment(lib, "ws2_32.lib")

int main()
{
	auto service = MakeShared<ServerService>(
		NetAddress(L"127.0.0.1", 7777),
		MakeShared<IocpCore>(),
		MakeShared<Session>,
		100
	);

	ASSERT_CRASH(service->Start());

	for (int32 i = 0; i < 4; ++i)
	{
		GThreadManager->Launch([&]() {
			while (true)
			{
				service->GetIocpCore()->Dispatch();
			}
		});
	}

	GThreadManager->Join();
	return 0;
}