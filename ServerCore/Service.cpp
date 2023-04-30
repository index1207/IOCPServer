#include "pch.h"
#include "Service.h"
#include "Session.h"

/* ---------------
	   Service
   --------------- */

Service::Service(ServiceType type, NetAddress address, IocpCorePtr iocp, SessionFactory factory, int32 maxSessionCount)
	: _type(type), _netAddr(address), _iocpCore(iocp), _sessionFactory(factory), _maxSessionCount(maxSessionCount)
{
	
}

Service::~Service()
{

}

SessionPtr Service::CreateSession()
{
	SessionPtr session = _sessionFactory();
	session->SetService(shared_from_this());

	if (_iocpCore->Register(session) == false)
		return nullptr;
	return session;
}

void Service::AddSession(SessionPtr session)
{
	WRITE_LOCK;
	_sessions.insert(session);
	_sessionCount++;
}

void Service::ReleaseSession(SessionPtr session)
{
	WRITE_LOCK;
	ASSERT_CRASH(_sessions.erase(session) != 0);
	_sessionCount--;
}

/* ----------------
	Client Service
   ---------------- */

ClientService::ClientService(NetAddress targetAddress, IocpCorePtr iocp, SessionFactory factory, int32 maxSessionCount)
	: Service(ServiceType::Client, targetAddress, iocp, factory, maxSessionCount)
{
}

ClientService::~ClientService()
{
}

bool ClientService::Start()
{
	return false;
}

/* ----------------
	Server Service
   ---------------- */

ServerService::ServerService(NetAddress targetAddress, IocpCorePtr iocp, SessionFactory factory, int32 maxSessionCount)
	: Service(ServiceType::Server, targetAddress, iocp, factory, maxSessionCount)
{

}

ServerService::~ServerService()
{
}

bool ServerService::Start()
{
	if (!CanStart()) return false;
	
	_listener = MakeShared<Listener>();
	if (_listener == nullptr)
		return false;

	ServerServicePtr service = static_pointer_cast<ServerService>(shared_from_this());
	if (!_listener->StartAccept(service)) return false;

	return true;
}

void ServerService::CloseService()
{
	//Service::CloseService();
}
