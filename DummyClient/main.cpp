#include "pch.h"
#include <iostream>

#include <winsock2.h>
#include <mswsock.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

void HandleError(string cause)
{
	int32 errCode = ::WSAGetLastError();
	cout << "[ERROR] Code : " << errCode << '\n';
}

int main()
{
	this_thread::sleep_for(100ms);
	WSADATA wsaData;
	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		return -1;
	}

	SOCKET s = socket(PF_INET, SOCK_STREAM, 0);
	
	u_long isBlocking = false;
	ioctlsocket(s, FIONBIO, &isBlocking);

	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
	addr.sin_port = htons(1225);
	
	while (true)
	{
		if (::connect(s, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR)
		{
			if (::WSAGetLastError() == WSAEWOULDBLOCK)
				continue;
			if (::WSAGetLastError() == WSAEISCONN)
				break;
		}
	}

	cout << "connected!\n";

	char buf[100] = "Hello World!";
	WSAOVERLAPPED overlap;
	WSAEVENT wsaEvent = ::WSACreateEvent();
	overlap.hEvent = wsaEvent;
	
	while (true)
	{
		WSABUF wsaBuf;
		wsaBuf.buf = buf;
		wsaBuf.len = 100;

		DWORD sendLen = 0;
		DWORD flags = 0;
		if (::WSASend(s, &wsaBuf, 1, &sendLen, flags, &overlap, nullptr) == SOCKET_ERROR)
		{
			if (::WSAGetLastError() == WSA_IO_PENDING)
			{
				::WSAWaitForMultipleEvents(1, &wsaEvent, true, WSA_INFINITE, false);
				::WSAGetOverlappedResult(s, &overlap, &sendLen, true, &flags);
			}
			else break;
		}

		cout << "Data Send Len! = " << sendLen << "\n";

		this_thread::sleep_for(1s);
	}

	::closesocket(s);
	::WSACleanup();
}