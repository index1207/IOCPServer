#pragma once
#include "IocpCore.h"
#include "IocpEvent.h"
#include "NetAddress.h"

class Service;

class Session : public IocpObject
{
	friend class Listener;
	friend class IocpCore;
	friend class Service;

public:
	Session();
	virtual ~Session();
public:
	void Send(BYTE* buffer, int32 len);
	void Disconnect(const WCHAR* cause);
	inline shared_ptr<Service> GetService() { return _service.lock(); }
	inline void SetService(shared_ptr<Service> service) { _service = service; }
public:
	/* 정보 관련 */
	inline void SetNetAddress(NetAddress&& address) { _netAddr = std::move(address); }
	inline NetAddress GetNetAddress() const { return _netAddr; }
	inline SOCKET GetSocket() { return _socket; }
	inline bool isConnected() { return _connected; }
	inline SessionPtr GetSessionPtr() { return static_pointer_cast<Session>(shared_from_this()); }
private:
	/* 인터페이스 구현 */
	virtual HANDLE GetHandle() override;
	virtual void Dispatch(class IocpEvent* iocpEvent, int32 numOfBytes = 0) override;
private:
	void RegisterConnect();
	void RegisterRecv();
	void RegisterSend();

	void ProcessConnect();
	void ProcessRecv(int32 numOfBytes);
	void ProcessSend(int32 numOfBytes);

	void HandleError(int32 errorCode);
protected:
	virtual void OnConnected() { };
	virtual int32 OnRecv(BYTE* buffer, int32 len) { return len; }
	virtual void OnSend(int32 len) { };
	virtual void OnDisconnected() { };
public:
	/* TEMP */
	char _recvBuffer[1000];
private:
	weak_ptr<Service> _service;
	SOCKET _socket = INVALID_SOCKET;
	NetAddress _netAddr{};
	Atomic<bool> _connected{ false };
private:
	USE_LOCK;
private:
	RecvEvent _recvEvent; 
};

