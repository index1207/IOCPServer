#pragma once

#include "NetAddress.h"
#include "IocpCore.h"
#include "Listener.h"
#include <functional>

enum class ServiceType : uint8
{
	Server,
	Client
};

using SessionFactory = std::function<SessionPtr(void)>;

/* ---------------
	   Service
   --------------- */

class Service : public enable_shared_from_this<Service>
{
public:
	Service(ServiceType type, NetAddress address, IocpCorePtr iocp, SessionFactory factory, int32 maxSessionCount = 1);
	virtual ~Service();

	virtual bool Start() abstract;
	inline bool CanStart() { return _sessionFactory != nullptr; }

	virtual void CloseService() abstract;
	inline void SetSessionFactory(SessionFactory factory) { _sessionFactory = factory; }

	SessionPtr CreateSession();
	void AddSession(SessionPtr session);
	void ReleaseSession(SessionPtr session);

	inline int32 GetCurrentSessionCount() const { return _sessionCount; };
	inline int32 GetMaxSessionCount() const { return _maxSessionCount; };
public:
	inline ServiceType GetServiceType() { return _type; }
	inline NetAddress GetNetAddress() { return _netAddr; }
	inline IocpCorePtr& GetIocpCore() { return _iocpCore; }
protected:
	USE_LOCK;
	ServiceType _type;
	NetAddress _netAddr {};
	IocpCorePtr _iocpCore;
	
	Set<SessionPtr> _sessions;
	int32 _sessionCount = 0;
	int32 _maxSessionCount = 0;
	SessionFactory _sessionFactory;
};

/* ----------------
    Client Service
   ---------------- */

class ClientService : public Service
{
public:
	ClientService(NetAddress targetAddress, IocpCorePtr iocp, SessionFactory factory, int32 maxSessionCount = 1);
	virtual ~ClientService() override;

	virtual bool Start() override;
};

/* ----------------
	Server Service
   ---------------- */

class ServerService : public Service
{
public:
	ServerService(NetAddress targetAddress, IocpCorePtr iocp, SessionFactory factory, int32 maxSessionCount = 1);
	virtual ~ServerService() override;

	virtual bool Start() override;
	virtual void CloseService() override;
private:
	ListenerPtr _listener = nullptr;
};