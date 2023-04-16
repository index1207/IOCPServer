#pragma once
class NetAddress
{
public:
	NetAddress() = default;
	NetAddress(SOCKADDR_IN sockAddr);
	NetAddress(wstring ip, uint16 port);

	SOCKADDR_IN& GetSocketAddress() { return addr; };
	wstring GetIpAddress();
	uint16 GetPort() { return ntohs(addr.sin_port); }
public:
	static IN_ADDR Ip2Address(const WCHAR* ip);
private:
	SOCKADDR_IN addr = {};
};

