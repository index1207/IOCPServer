#pragma once

#include "NetAddress.h"

class SocketUtil
{
public:
	static LPFN_CONNECTEX ConnectEx;
	static LPFN_DISCONNECTEX DisconnectEx;
	static LPFN_ACCEPTEX AcceptEx;
public:
	static void Init();
	static void Clear();

	static bool BindWindowsFunction(SOCKET s, GUID guid, LPVOID* func);
	static SOCKET CreateSocket();

	static bool SetLinger(SOCKET s, uint16 onoff, uint16 linger);
	static bool SetReuseAddress(SOCKET s, bool flag);
	static bool SetRecvBufferSize(SOCKET s, int32 size);
	static bool SetSendBufferSize(SOCKET s, int32 size);
	static bool SetTcpNoDelay(SOCKET s, bool flag);
	static bool SetUpdateAcceptSocket(SOCKET s, SOCKET listenSocket);

	static bool Bind(SOCKET s, NetAddress netAddr);
	static bool Bind(SOCKET s, uint16 port);
	static bool Listen(SOCKET listenSocket, int32 backlog = SOMAXCONN);
	static void Close(SOCKET s);
};

template<typename T>
static inline bool SetSocketOption(SOCKET s, int32 level, int32 optName, T value)
{
	return SOCKET_ERROR != ::setsockopt(s, level, optName, reinterpret_cast<char*>(&value), sizeof(value));
}