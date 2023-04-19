#pragma once
#include "IocpCore.h"
#include "IocpEvent.h"
#include "NetAddress.h"

class Session : public IocpObject
{
public:
	Session();
	virtual ~Session();
public:
	/* 정보 관련 */
	void SetNetAddress(NetAddress&& address) { _netAddr = std::move(address); }
	NetAddress GetNetAddress() const { return _netAddr; }
	SOCKET GetSocket() { return _socket; }

public:
	/* 인터페이스 구현 */
	virtual HANDLE GetHandle() override;
	virtual void Dispatch(class IocpEvent* iocpEvent, int32 numOfBytes = 0) override;
public:
	/* TEMP */
	char _recvBuffer[1000];
private:
	SOCKET _socket = INVALID_SOCKET;
	NetAddress _netAddr{};
	Atomic<bool> _connected{ false };
};

