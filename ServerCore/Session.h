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
	/* ���� ���� */
	void SetNetAddress(NetAddress&& address) { _netAddr = std::move(address); }
	NetAddress GetNetAddress() const { return _netAddr; }
	SOCKET GetSocket() { return _socket; }

public:
	/* �������̽� ���� */
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

