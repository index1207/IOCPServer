#pragma once

class Session;

enum class EventType : uint8
{
	Connect,
	Accept,
	// PreRecv,
	Recv,
	Send
};

/* -------------- 
	 Iocp Event
   --------------*/

// GetQueued...���� Overlapped����ü ��� �޾ƿ� �̺�Ʈ ����
// virtual��� X, v-table�� ä����
class IocpEvent : public WSAOVERLAPPED
{
public:
	IocpEvent(EventType type);

	void Init();
public:
	EventType eventType;
	IocpObjectPtr owner;
};

/* ---------------
    Accept Event
   --------------- */

class AcceptEvent : public IocpEvent
{
public:
	AcceptEvent() : IocpEvent(EventType::Accept) {}

public:
	SessionPtr session = nullptr;
};

/* ---------------
	  Recv Event
   --------------- */

class RecvEvent : public IocpEvent
{
public:
	RecvEvent() : IocpEvent(EventType::Recv) {}
};

/* ---------------
	  Send Event
   --------------- */

class SendEvent : public IocpEvent
{
public:
	SendEvent() : IocpEvent(EventType::Send) {}
};