#pragma once

#include "base.h"

namespace NetLib 
{

#ifdef _WIN32
	typedef uint Socket;
#else
	typedef int Socket;
#endif

extern boolean isValidResult(int res);
extern boolean isValidSocket(Socket sk);
extern boolean setNonBlockingMode(Socket sk);

class SocketHolder
{
public:
	SocketHolder();
	SocketHolder(Socket s){ socket = s; }
	void operator = (Socket s){ close(); socket = s; }
	~SocketHolder(){ close(); }
	void close();

	//operator bool ()const { return isValidSocket(socket); }
	//bool operator ! ()const { return !isValidSocket(socket); }
	//operator Socket () { return socket; }
	Socket native()const { return socket; }
	boolean valid()const { return isValidSocket(socket); }

private:
	Socket socket;
	SocketHolder(SocketHolder& other){}
	void operator = (SocketHolder& other){}
};

}