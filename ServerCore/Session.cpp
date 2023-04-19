#include "pch.h"
#include "Session.h"
#include "SocketUtil.h"

Session::Session()
{
	_socket = SocketUtil::CreateSocket();
}

Session::~Session()
{
	SocketUtil::Close(_socket);
}

HANDLE Session::GetHandle()
{
	return reinterpret_cast<HANDLE>(_socket);
}

void Session::Dispatch(IocpEvent* iocpEvent, int32 numOfBytes)
{
	// TODO: 
}
