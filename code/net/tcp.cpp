#include "tcp.h"
#include "loc.h"

namespace NetLib
{

TCPClient::TCPClient(NetAddres connectTo)
{
	soc = ::socket(PF_INET, SOCK_STREAM, 0);
	if(!soc.valid())
		return;

	struct ::sockaddr_in addr;
	convert(addr, connectTo);

	int res = ::connect(soc.native(), (struct sockaddr *)&addr, sizeof(addr));
	if(!isValidResult(res)){
		soc.close();
		return;
	}

	if(!setNonBlockingMode(soc.native())){
		soc.close();
		return;
	}
}

uint TCPClient::recive(void* buff, uint buffsize)
{
	int recvd = ::recv(soc.native(), (char*)buff, buffsize, 0);
	if(recvd >= 0)
		return recvd;

	soc.close();
	return 0;
}

boolean TCPClient::send(const void* buff, uint size)
{
	int count = ::send(soc.native(), (const char*)buff, size, 0);
	if(count < 0){
		soc.close();
		return false;
	}
	if(count < size)
		return send((const byte*)buff + count, size - count);
}


}