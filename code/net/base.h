#pragma once

#include "../base/include.h"

namespace NetLib
{

USING_BASE_LIB
USING_BASE_MATH

struct NetAddres
{
	byte ip[4];
	uint16 port;

	INLINE NetAddres()
	{
		ip[0] = 127;
		ip[1] = 0;
		ip[2] = 0;
		ip[3] = 1;
		port = 16000;
	}
};

extern boolean getNetAddresByDomainName(NetAddres& addr, ConstString name);
extern void convert(struct ::sockaddr_in& sa, const NetAddres& addr);
extern void convert(NetAddres& addr, const struct ::sockaddr_in& sa);

}

namespace Base
{
	extern boolean parse(NetLib::NetAddres& addr, ConstString name);
	extern void print(String& str, const NetLib::NetAddres& addr);
}
