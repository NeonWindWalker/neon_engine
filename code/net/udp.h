#pragma once

#include "base.h"

namespace NetLib 
{

const static uint gcMaxDatagramSize = 65535;

class UDPSocket
{
public:
	UDPSocket(const NetAddres& addres);
	~UDPSocket();
	boolean valid()const;
	static UDPSocket* open(const NetAddres& addres);

	uint recive(void* buff, uint buffsize, NetAddres& addres);
	boolean send(const NetAddres& addres, const void* buff, uint size);

protected:

#ifdef _WIN32
	typedef uint Socket_t;
#else
	typedef int Socket_t;
#endif

	Socket_t _socket;
};



}