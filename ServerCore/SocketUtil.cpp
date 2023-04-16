#include "pch.h"
#include "SocketUtil.h"

LPFN_CONNECTEX SocketUtil::ConnectEx = nullptr;
LPFN_DISCONNECTEX SocketUtil::DisconnectEx = nullptr;
LPFN_ACCEPTEX SocketUtil::AcceptEx = nullptr;

void SocketUtil::Init()
{
	WSADATA wsaData;
	ASSERT_CRASH(::WSAStartup(MAKEWORD(2, 2), &wsaData) == 0);

	SOCKET dummy = CreateSocket();
	ASSERT_CRASH(BindWindowsFunction(dummy, WSAID_CONNECTEX, reinterpret_cast<LPVOID*>(&ConnectEx)));
	ASSERT_CRASH(BindWindowsFunction(dummy, WSAID_DISCONNECTEX, reinterpret_cast<LPVOID*>(&DisconnectEx)));
	ASSERT_CRASH(BindWindowsFunction(dummy, WSAID_ACCEPTEX, reinterpret_cast<LPVOID*>(&AcceptEx)));
	Close(dummy);
}

void SocketUtil::Clear()
{
	::WSACleanup();
}

bool SocketUtil::BindWindowsFunction(SOCKET s, GUID guid, LPVOID* func)
{
	DWORD bytes;
	return SOCKET_ERROR != WSAIoctl(s, SIO_GET_EXTENSION_FUNCTION_POINTER, &guid, sizeof(guid), func, sizeof(*func), &bytes, NULL, NULL);
}

SOCKET SocketUtil::CreateSocket()
{
	return ::WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
}

bool SocketUtil::SetLinger(SOCKET s, uint16 onoff, uint16 linger)
{
	LINGER option;
	option.l_onoff = onoff;
	option.l_linger = linger;
	return SetSocketOption(s, SOL_SOCKET, SO_LINGER, option);
}

bool SocketUtil::SetReuseAddress(SOCKET s, bool flag)
{
	return SetSocketOption(s, SOL_SOCKET, SO_REUSEADDR, flag);
}

bool SocketUtil::SetRecvBufferSize(SOCKET s, int32 size)
{
	return SetSocketOption(s, SOL_SOCKET, SO_RCVBUF, size);
}

bool SocketUtil::SetSendBufferSize(SOCKET s, int32 size)
{
	return SetSocketOption(s, SOL_SOCKET, SO_SNDBUF, size);
}

bool SocketUtil::SetTcpNoDelay(SOCKET s, bool flag)
{
	return SetSocketOption(s, SOL_SOCKET, TCP_NODELAY, flag);
}

bool SocketUtil::SetUpdateAcceptSocket(SOCKET s, SOCKET listenSocket)
{
	return SetSocketOption(s, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, listenSocket);
}

bool SocketUtil::Bind(SOCKET s, NetAddress netAddr)
{
	return SOCKET_ERROR != ::bind(s, reinterpret_cast<SOCKADDR*>(&netAddr.GetSocketAddress()), sizeof(SOCKADDR_IN));
}

bool SocketUtil::Bind(SOCKET s, uint16 port)
{
	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	return SOCKET_ERROR != ::bind(s, reinterpret_cast<SOCKADDR*>(&addr), sizeof(addr));
}

bool SocketUtil::Listen(SOCKET listenSocket, int32 backlog)
{
	return SOCKET_ERROR != ::listen(listenSocket, backlog);
}

void SocketUtil::Close(SOCKET s)
{
	if (s != INVALID_SOCKET)
		closesocket(s);
	s = INVALID_SOCKET;
}
