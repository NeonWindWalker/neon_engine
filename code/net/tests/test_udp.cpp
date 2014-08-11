#include "../include.h"

namespace NetLib
{

void udpTest()
{
	NetAddres senderAdders, reciverAddres;
	senderAdders.port = 32156;
	reciverAddres.port = 32157;

	UDPSocket sender(senderAdders);
	UDPSocket reciver(reciverAddres);

	Base::String netstr(_CS("hello net!"));
	sender.send(reciverAddres, netstr.first(), netstr.size());
	NetAddres inaddr;
	char buff[32] = {0};
	uint j=0;
	for(uint i=0; i<50; ++i)
	{
		if(Base::rand(0, 1000) & 1)
		{
			netstr.clear();
			netstr << _CS("hello net msg ") << print(j++);
			sender.send(reciverAddres, netstr.first(), netstr.size());
		}
		else
		{
			if(reciver.recive(buff, 32, inaddr))
				logInfo << "net send recive string " << buff;
			else
				logInfo << "empty";
		}
	}
}

}