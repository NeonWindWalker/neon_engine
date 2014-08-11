#pragma once

#include "socket.h"

namespace NetLib 
{

class TCPClient
{
public:
	TCPClient(NetAddres connectTo);
	boolean valid()const { return soc.valid(); }

	uint recive(void* buff, uint buffsize);
	boolean send(const void* buff, uint size);

protected:
	SocketHolder soc;
};

class TCPServerConnection
{
public:
	TCPServerConnection()
	{
	
	}

	uint recive(void* buff, uint buffsize);
	boolean send(const void* buff, uint size);

protected:
	SocketHolder soc;
};

class TCPServer
{
public:
	TCPServer(NetAddres listenAddr);
	boolean valid()const { return listenSoc.valid(); }

	void processBegin();
	void process();
	void processEnd();

protected:
	SocketHolder listenSoc;
	
	Mutex connectionsMutex;
	Array<Socket> connections;
	volatile int currectConnection;

	Thread* listenThread;
};

}