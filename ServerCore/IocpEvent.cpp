#include "pch.h"
#include "IocpEvent.h"

IocpEvent::IocpEvent(EventType type) : eventType(type)
{
	Init();
}

void IocpEvent::Init()
{
	WSAOVERLAPPED::hEvent = 0;
	WSAOVERLAPPED::Internal = 0;
	WSAOVERLAPPED::InternalHigh = 0;
	WSAOVERLAPPED::Offset = 0;
	WSAOVERLAPPED::OffsetHigh = 0;
}
