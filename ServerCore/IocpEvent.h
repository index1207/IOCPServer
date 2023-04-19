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

// GetQueued...에서 Overlapped구조체 대신 받아올 이벤트 정보
// virtual사용 X, v-table로 채워짐
class IocpEvent : public WSAOVERLAPPED
{
public:
	IocpEvent(EventType type);

	void Init();
	EventType GetType() { return _type; }
protected:
	EventType _type;
};

/* ---------------
    Accept Event
   --------------- */

class AcceptEvent : public IocpEvent
{
public:
	AcceptEvent() : IocpEvent(EventType::Accept) {}

	void SetSession(Session* session) { _session = session; }
	Session* GetSession() { return _session; }
private:
	Session* _session = nullptr;
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