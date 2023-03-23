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

	SOCKET listenSock = socket(PF_INET, SOCK_STREAM, 0);

	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(1225);

	::bind(listenSock, (SOCKADDR*)&addr, sizeof(addr));
	::listen(listenSock, SOMAXCONN);

	vector<WSAEVENT> events;
	vector<Session> sessions;
	sessions.reserve(100);

	WSAEVENT listenEvent = ::WSACreateEvent();
	events.push_back(listenEvent);
	sessions.push_back({ listenSock });
	/*
		FD_ACCEPT  : 접속한 클라가 있음
		FD_READ    : 데이터 수신 가능
		FD_WRITE   : 데이터 송신 가능
		FD_CLOSE   : 상대가 접속 종료
		FD_CONNECT : 통신을 위한 연결 절차 완료
		FD_OOB     : OOB 데이터
	*/
	if (::WSAEventSelect(listenSock, listenEvent, FD_ACCEPT | FD_CLOSE) == SOCKET_ERROR)
	{
		return -1;
	}

	while (true)
	{
		int32 index = ::WSAWaitForMultipleEvents(events.size(), &events[0], false, WSA_INFINITE, false);
		index -= WSA_WAIT_EVENT_0;

		//::WSAResetEvent(events[index]);

		WSANETWORKEVENTS networkEvent;
		if (::WSAEnumNetworkEvents(sessions[index].sock, events[index], &networkEvent) == SOCKET_ERROR)
			continue;

		if (networkEvent.lNetworkEvents & FD_ACCEPT)
		{	
			if (networkEvent.iErrorCode[FD_ACCEPT_BIT] != 0)
				continue;
			
			SOCKADDR_IN clientAddr;
			int32 addrlen = sizeof(clientAddr);
			ZeroMemory(&clientAddr, addrlen);
			SOCKET clientSock = accept(listenSock, (SOCKADDR*)&clientAddr, &addrlen);
			if (clientSock != INVALID_SOCKET)
			{
				cout << "Connected to client!\n";
				
				WSAEVENT clientEvent = ::WSACreateEvent();
				events.push_back(clientEvent);
				sessions.push_back(Session{ clientSock });
				if (::WSAEventSelect(clientSock, clientEvent, FD_READ | FD_WRITE | FD_CLOSE) == SOCKET_ERROR)
				{
					return 0;
				}
			}
		}
		if (networkEvent.lNetworkEvents & FD_READ || networkEvent.lNetworkEvents & FD_WRITE)
		{
			if ((networkEvent.lNetworkEvents & FD_READ) && 
				networkEvent.iErrorCode[FD_READ_BIT] != 0)
				continue;
			if ((networkEvent.lNetworkEvents & FD_WRITE) &&
				networkEvent.iErrorCode[FD_WRITE_BIT] != 0)
				continue;

			Session& s = sessions[index];

			if (s.recvBytes == 0)
			{
				int32 recvlen = ::recv(s.sock, s.recvBuf, BUFSIZE, NULL);
				if (recvlen == SOCKET_ERROR && WSAGetLastError() != WSAEWOULDBLOCK)
				{
					// 세션 삭제
					continue;
				}
				s.recvBytes = recvlen;
				cout << "Receive Data : " << recvlen << '\n';
			}
			if (s.sendBytes < s.recvBytes)
			{
				int32 sendlen = ::send(s.sock, &s.recvBuf[s.sendBytes], s.recvBytes - s.sendBytes, NULL);
				if (sendlen == SOCKET_ERROR && WSAGetLastError() != WSAEWOULDBLOCK)
				{
					// 세션 삭제
					continue;
				}
				s.sendBytes += sendlen;
				if (s.sendBytes == s.recvBytes)
				{
					s.sendBytes = s.recvBytes = 0;
				}
				cout << "Send Data : " << sendlen << '\n';
			}
		}
		if (networkEvent.lNetworkEvents & FD_CLOSE)
		{
			
		}
	}

	::WSACleanup();
}