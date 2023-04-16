#include "pch.h"
#include "NetAddress.h"

NetAddress::NetAddress(SOCKADDR_IN sockAddr) : addr(sockAddr)
{
}

NetAddress::NetAddress(wstring ip, uint16 port)
{
    ZeroMemory(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr = Ip2Address(ip.c_str());
    addr.sin_port = htons(port);
}

wstring NetAddress::GetIpAddress()
{
    WCHAR buf[32];
    ::InetNtop(AF_INET, &addr.sin_addr, buf, len32(buf));
    return buf;
}

IN_ADDR NetAddress::Ip2Address(const WCHAR* ip)
{
    IN_ADDR address;
    ::InetPtonW(AF_INET, ip, &address);
    return address;
}
