#include "pch.h"
#include "CorePch.h"
#include <iostream>

#include "ThreadManager.h"

#include "SocketUtil.h"
#include "Service.h"
#include "Session.h"
#include "Listener.h"

#pragma comment(lib, "ws2_32.lib")


class GameSession : public Session
{
	virtual int32 OnRecv(BYTE* buffer, int32 len) override
	{
		cout << "Recv len = " << len << '\n';
		Send(buffer, len);

		return len;
	}

	virtual void OnSend(int32 len)
	{
		cout << "Send len = " << len << '\n';
	}
};

int main()
{
	auto service = MakeShared<ServerService>(
		NetAddress(L"127.0.0.1", 7777),
		MakeShared<IocpCore>(),
		MakeShared<GameSession>,
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