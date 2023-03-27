#include "pch.h"

#include <iostream>

#include <winsock2.h>
#include <mswsock.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#include "Memory.h"
#include "ThreadManager.h"

const int BUFSIZE = 1024;

enum IO_TYPE
{
	READ,
	WRITE,
	ACCEPT,
	CONNECT
};

struct Session
{
	SOCKET sock = INVALID_SOCKET;
	char recvBuf[BUFSIZE] = "";
	int32 recvBytes = 0;
};

struct OverlappedEx
{
	WSAOVERLAPPED overlapped = {};
	int32 type = 0;
};

void WorkerThreadMain(HANDLE iocpHandle)
{
	while (true)
	{
		DWORD bytesTransferred = 0;
		Session* session = nullptr;
		OverlappedEx* overlap = nullptr;
		BOOL retval = ::GetQueuedCompletionStatus(iocpHandle, &bytesTransferred, (PULONG_PTR)&session, (LPOVERLAPPED*)&overlap, INFINITE);
		if (retval == FALSE || bytesTransferred == 0)
			continue;

		ASSERT_CRASH(overlap->type == IO_TYPE::READ);

		cout << "Recv Data IOCP : " << bytesTransferred << '\n';

		WSABUF wsaBuf;
		wsaBuf.buf = session->recvBuf;
		wsaBuf.len = BUFSIZE;
		
		DWORD recvLen = 0;
		DWORD flag = 0;
		::WSARecv(session->sock, &wsaBuf, 1, &recvLen, &flag, &overlap->overlapped, NULL);
	}
}

int main()
{
	WSADATA wsaData;
	if (::WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		return -1;
	}

	SOCKET listenSock = socket(PF_INET, SOCK_STREAM, 0);

	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(1225);

	::bind(listenSock, (SOCKADDR*)&addr, sizeof(addr));
	::listen(listenSock, SOMAXCONN);


	vector<Session*> sessionManager;

	// CP 생성
	HANDLE iocpHandle = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

	for (int i = 0; i < 4; ++i)
	{
		GThreadManager->Launch([=]() { WorkerThreadMain(iocpHandle); });
	}

	// Main Thread (Accept 담당)
	while (true)
	{
		SOCKADDR_IN clientAddr;
		int32 addrlen = sizeof(clientAddr);
		SOCKET clientSock = ::accept(listenSock, (SOCKADDR*)&clientAddr, &addrlen);
		if (clientSock == INVALID_SOCKET)
			return 0;

		auto session = xnew<Session>();
		session->sock = clientSock;
		sessionManager.push_back(session);

		cout << "Client Connected!\n";

		// 소켓을 CP에 등록
		::CreateIoCompletionPort((HANDLE)clientSock, iocpHandle, (ULONG_PTR)session, 0);
		
		WSABUF wsaBuf;
		wsaBuf.buf = session->recvBuf;
		wsaBuf.len = BUFSIZE;

		OverlappedEx* overlap = new OverlappedEx();
		overlap->type = IO_TYPE::READ;

		DWORD recvLen = 0;
		DWORD flag = 0;
		::WSARecv(clientSock, &wsaBuf, 1, &recvLen, &flag, &overlap->overlapped, NULL);

		/*auto* back = sessionManager.back();
		sessionManager.pop_back();
		xdelete(back);*/

		//closesocket(session.sock);
		//WSACloseEvent(wsaEvent);
	}

	//GThreadManager->Join();

	::WSACleanup();
}