#include "pch.h"

#include <iostream>

#include <winsock2.h>
#include <mswsock.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

const int BUFSIZE = 1024;

struct Session
{
	SOCKET sock = INVALID_SOCKET;
	char recvBuf[BUFSIZE] = "";
	int32 recvBytes = 0;
	int32 sendBytes = 0;
};

void HandleError(string cause)
{
	int32 errCode = ::WSAGetLastError();
	cout << "[ERROR] Code : " << errCode << '\n';
}

int main()
{
	WSADATA wsaData;
	if (::WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		HandleError("Init");
	}

	SOCKET sock = socket(PF_INET, SOCK_STREAM, 0);

	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(1225);

	::bind(sock, (SOCKADDR*)&addr, sizeof(addr));
	::listen(sock, SOMAXCONN);

	vector<Session> v;
	v.reserve(100);

	fd_set read;
	fd_set write;
	while (true)
	{
		FD_ZERO(&read);
		FD_ZERO(&write);

		FD_SET(sock, &read);

		for (Session& s : v)
		{
			if (s.recvBytes <= s.sendBytes)
				FD_SET(s.sock, &read);
			else FD_SET(s.sock, &write);
		}

		if (::select(NULL, &read, &write, nullptr, nullptr) == SOCKET_ERROR)
			break;
		
		if (FD_ISSET(sock, &read))
		{
			SOCKADDR_IN clientAddr;
			int32 addrlen = sizeof(clientAddr);
			SOCKET clientSock = ::accept(sock, (SOCKADDR*)&addrlen, &addrlen);
			if (clientSock != INVALID_SOCKET)
			{
				cout << "Client Connected!\n";
				v.push_back(Session{clientSock});
			}
		}

		for (Session& s : v)
		{
			if (FD_ISSET(s.sock, &read))
			{
				int32 recvlen = ::recv(s.sock, s.recvBuf, BUFSIZ, NULL);
				if (recvlen <= 0) continue;

				s.recvBytes = recvlen;
			}
			if (FD_ISSET(s.sock, &write))
			{
				int32 sendlen = ::send(s.sock, &s.recvBuf[s.recvBytes], s.recvBytes - s.sendBytes, NULL);
				if (sendlen == SOCKET_ERROR) continue;
				
				s.sendBytes += sendlen;
				if (s.recvBytes == s.sendBytes)
				{
					s.recvBytes = s.sendBytes = 0;
				}
			}
		}
	}

	::WSACleanup();
}