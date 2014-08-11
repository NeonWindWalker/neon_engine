#include "udp.h"
#include "loc.h"

namespace Base
{

void print(String& str, const NetLib::NetAddres& addr)
{
	print(str, addr.ip[0]);
	str.push('.');
	str << print(addr.ip[1]);
	str.push('.');
	str << print(addr.ip[2]);
	str.push('.');
	str << print(addr.ip[3]);
	str.push('.');
	str << print(addr.port);
}

boolean parse(NetLib::NetAddres& addr, ConstString str)
{
	const byte * d0 = str.find('.');
	if(d0 == str.end())return false;
	const byte * d1 = find(d0 + 1, str.end(), '.');
	if(d1 == str.end())return false;
	const byte * d2 = find(d1 + 1, str.end(), '.');
	if(d2 == str.end())return false;
	const byte * dd = find(d2 + 1, str.end(), '.');
	if(dd == str.end())return false;

	if(!parse(addr.ip[0], ConstString(str.first(), d0)))return false;
	if(!parse(addr.ip[1], ConstString(d0 + 1, d1)))return false;
	if(!parse(addr.ip[2], ConstString(d1 + 1, d2)))return false;
	if(!parse(addr.ip[3], ConstString(d2 + 1, dd)))return false;
	if(!parse(addr.port, ConstString(dd + 1, str.end())))return false;
	return true;
}

}

namespace NetLib
{

void convert(sockaddr_in& sa, const NetAddres& addr)
{
	memset(&sa, 0, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = htonl((uint32(addr.ip[0])<<24) | (uint32(addr.ip[1])<<16) | (uint32(addr.ip[2])<<8) | addr.ip[3]);
	sa.sin_port = htons(addr.port);
}

void convert(NetAddres& addr, const sockaddr_in& sa)
{
	uint32 ip = ntohl(sa.sin_addr.s_addr);
	addr.ip[0] = byte ((ip >> 24) & 255);
	addr.ip[1] = byte ((ip >> 16) & 255);
	addr.ip[2] = byte ((ip >> 8) & 255);
	addr.ip[3] = byte (ip & 255);
	addr.port = ntohs(sa.sin_port);
}

UDPSocket* UDPSocket::open(const NetAddres& addres)
{
	UDPSocket* s = new UDPSocket(addres);
	if(!s->valid()){
		delete s;
		return 0;
	}
	return s;
}

#ifdef _WIN32
static boolean gWinSockIsInited = false;

boolean initWinSock()
{
	if(gWinSockIsInited)
		return true;

	WORD wVersionRequested;
    WSADATA wsaData;

    wVersionRequested = MAKEWORD(2, 2);

    gWinSockIsInited = WSAStartup(wVersionRequested, &wsaData) == 0;
	return gWinSockIsInited;
}

boolean getNetAddresByDomainName(NetAddres& addr, ConstString name)
{
	initWinSock();

	hostent* h = gethostbyname(String(name).c_str());

	if(!h)
		return false;

	addr = NetAddres();
	uint32 ip = ntohl(*(u_long*)h->h_addr_list[0]);
	addr.ip[0] = byte ((ip >> 24) & 255);
	addr.ip[1] = byte ((ip >> 16) & 255);
	addr.ip[2] = byte ((ip >> 8) & 255);
	addr.ip[3] = byte (ip & 255);
	return true;
}

UDPSocket::UDPSocket(const NetAddres& addres)
{
	initWinSock();
	
	_socket = socket(PF_INET, SOCK_DGRAM, 0);
	if (_socket == SOCKET_ERROR){
		logError << "could not create a socket";
		return;
	}

	u_long async = 1;
	if(ioctlsocket(_socket, FIONBIO, &async))
	{
		logError << "ioctlsocket - cn set asyncron mode";
		closesocket(_socket);
		_socket = 0;
		return;
	}

	struct sockaddr_in addr;
	convert(addr, addres);

	int res = bind(_socket, reinterpret_cast <sockaddr *> (&addr), sizeof (struct sockaddr_in));
	if (res == SOCKET_ERROR)
		logError << "bind - could not bind socket to a provided name";
}

UDPSocket::~UDPSocket()
{
	int error = closesocket(_socket);
	if(error == SOCKET_ERROR)
		logError << "could not close socket properly";
}

boolean UDPSocket::valid()const
{
	return _socket != SOCKET_ERROR;
}

uint UDPSocket::recive(void* buff, uint buffsize, NetAddres& addres)
{
	int dest_len = sizeof (struct sockaddr_in);
	struct sockaddr_in addr;
	int res = recvfrom(_socket, (char*)buff, buffsize, 0, reinterpret_cast <sockaddr *> (&addr), &dest_len);
	if (res == SOCKET_ERROR){
		//logError("recv - could not receive data");
		return 0;
	}

	convert(addres, addr);
	return res;
}

boolean UDPSocket::send(const NetAddres& addres, const void* buff, uint size)
{
	struct sockaddr_in addr;
	convert(addr, addres);

	int res = sendto(_socket, (const char*)buff, size, 0, reinterpret_cast <sockaddr *> (&addr), sizeof (struct sockaddr_in));
	if (res == SOCKET_ERROR){
		logError << "send - could not send data";
		return false;
	}
	return true;
}
#endif


#if defined(__linux__)

boolean getNetAddresByDomainName(NetAddres& addr, ConstString name)
{
	hostent* h = gethostbyname(String(name).c_str());

	if(!h)
		return false;

	addr = NetAddres();
	uint32 ip = ntohl(*(u_long*)h->h_addr_list[0]);
	addr.ip[0] = byte ((ip >> 24) & 255);
	addr.ip[1] = byte ((ip >> 16) & 255);
	addr.ip[2] = byte ((ip >> 8) & 255);
	addr.ip[3] = byte (ip & 255);
	return true;
}

UDPSocket::UDPSocket(const NetAddres& addres)
{
	_socket = socket(PF_INET, SOCK_DGRAM, 0);
	if (_socket < 0){
		logError << "could not create a socket";
		return;
	}

	if( fcntl(_socket, F_SETFL, O_NONBLOCK) == -1){
		close(_socket);
		_socket = 0;
		logError("could not set fcntl(_socket, F_SETFL, O_NONBLOCK)");
		return;
	}

	/*if( fcntl(_socket, F_SETFL, O_ASYNC) == -1){
		close(_socket);
		_socket = 0;
		logError("could not set fcntl(_socket, F_SETFL, O_ASYNC)");
		return;
	}*/

	sockaddr_in addr;
	convert(addr, addres);

	int res = bind(_socket, reinterpret_cast <sockaddr *> (&addr), sizeof (struct sockaddr_in));
	if (res < 0)
		logError << "bind - could not bind socket to a provided name";
}

UDPSocket::~UDPSocket()
{
	int error = close(_socket);
	if(error < 0)
		logError << "could not close socket properly";
}

boolean UDPSocket::valid()const
{
	return _socket >= 0;
}

uint UDPSocket::recive(void* buff, uint buffsize, NetAddres& addres)
{
	socklen_t dest_len = sizeof (struct sockaddr_in);
	sockaddr_in addr;
	int res = recvfrom(_socket, (char*)buff, buffsize, 0, reinterpret_cast <sockaddr *> (&addr), &dest_len);
	if (res < 0){
		//logError("recv - could not receive data");
		return 0;
	}

	convert(addres, addr);
	return res;
}

boolean UDPSocket::send(const NetAddres& addres, const void* buff, uint size)
{
	sockaddr_in addr;
	convert(addr, addres);

	int res = sendto(_socket, (const char*)buff, size, 0, reinterpret_cast <sockaddr *> (&addr), sizeof (struct sockaddr_in));
	if (res < 0){
		logError << "send - could not send data";
		return false;
	}
	return true;
}
#endif


}
