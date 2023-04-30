#include "pch.h"
#include "Listener.h"
#include "SocketUtil.h"
#include "IocpEvent.h"
#include "Session.h"
#include "Service.h"

Listener::~Listener()
{
	SocketUtil::Close(_socket);

	for (auto* acceptEvent : _acptEvnts)
	{
		// TODO:

		xdelete(acceptEvent);
	}
}

bool Listener::StartAccept(ServerServicePtr service)
{
	_service = service;
	if (_service == nullptr) return false;

	_socket = SocketUtil::CreateSocket();
	if (_socket == INVALID_SOCKET)
		return false;
	
	if (_service->GetIocpCore()->Register(shared_from_this()) == false)
		return false;
	if (SocketUtil::SetReuseAddress(_socket, true) == false)
		return false;
	if (SocketUtil::SetLinger(_socket, 0, 0) == false)
		return false;
	if (SocketUtil::Bind(_socket, service->GetNetAddress()) == false)
		return false;
	if (SocketUtil::Listen(_socket) == false)
		return false;

	const int32 acceptCount = service->GetMaxSessionCount();
	for (int32 i = 0; i < acceptCount; ++i)
	{
		auto acceptEvent = xnew<AcceptEvent>();
		acceptEvent->owner = shared_from_this();
		_acptEvnts.push_back(acceptEvent);
		RegisterAccept(acceptEvent);
	}

	return true;
}

void Listener::CloseSocket()
{
	SocketUtil::Close(_socket);
}

HANDLE Listener::GetHandle()
{
	return reinterpret_cast<HANDLE>(_socket);
}

void Listener::Dispatch(class IocpEvent* iocpEvent, int32 numOfBytes)
{
	ASSERT_CRASH(iocpEvent->eventType == EventType::Accept);

	auto acceptEvent = static_cast<AcceptEvent*>(iocpEvent);
	ProcessAccept(acceptEvent);
}

void Listener::RegisterAccept(AcceptEvent* acceptEvent)
{
	SessionPtr session = _service->CreateSession();

	acceptEvent->Init();
	acceptEvent->session = session;

	DWORD dwBytesReceived = 0;
	if (!SocketUtil::AcceptEx(_socket, session->GetSocket(), session->_recvBuffer, 0, sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, &dwBytesReceived, static_cast<LPOVERLAPPED>(acceptEvent)))
	{
		const int32 errCode = WSAGetLastError();
		if (errCode != WSA_IO_PENDING)
		{
			RegisterAccept(acceptEvent);
		}
	}
}

void Listener::ProcessAccept(AcceptEvent* acceptEvent)
{
	auto session = acceptEvent->session;

	if (!SocketUtil::SetUpdateAcceptSocket(session->GetSocket(), _socket))
	{
		RegisterAccept(acceptEvent);
		return;
	}

	SOCKADDR_IN clientAddress{};
	int32 addressSize = sizeof(clientAddress);
	if (SOCKET_ERROR == ::getpeername(session->GetSocket(), reinterpret_cast<SOCKADDR*>(&clientAddress), &addressSize))
	{
		RegisterAccept(acceptEvent);
		return;
	}

	session->SetNetAddress(NetAddress(clientAddress));

	cout << "Client Connected!\n";

	RegisterAccept(acceptEvent);
}