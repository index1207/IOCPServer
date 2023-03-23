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

	while (true)
	{
		if (::send(s, buf, sizeof(buf), NULL) == SOCKET_ERROR)
		{
			if (::WSAGetLastError() == WSAEWOULDBLOCK)
				continue;

			// Error
			break;
		}
		cout << "Send Data! (" << sizeof(buf) << ")\n";

		while (true)
		{
			char recvBuf[1000] = "";
			int32 recvlen = ::recv(s, recvBuf, sizeof(recvBuf), NULL);
			if (recvlen == SOCKET_ERROR)
			{
				if (::WSAGetLastError() == WSAEWOULDBLOCK)
				{
					continue;
				}

				// Error
				break;
			}
			else if (recvlen == 0)
			{
				// Disconnected
				break;
			}
			cout << "Recv Data! (" << recvlen << ")\n";
			break;
		}

		this_thread::sleep_for(1s);
	}

	::closesocket(s);
	::WSACleanup();
}