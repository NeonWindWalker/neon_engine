#include "socket.h"
#include "loc.h"


namespace NetLib
{

boolean isValidSocket(Socket socket)
{
#ifdef _WIN32
	return socket != SOCKET_ERROR;
#else
	return socket >= 0;
#endif
}

extern boolean isValidResult(int res)
{
#ifdef _WIN32
	return res != SOCKET_ERROR;
#else
	return res >= 0;
#endif
}

boolean setNonBlockingMode(Socket socket)
{
#ifdef _WIN32
	u_long async = 1;
	if(ioctlsocket(socket, FIONBIO, &async)){
		logError << "ioctlsocket - cn set asyncron mode";
		return false;
	}
	return true;
#else
	if( fcntl(_socket, F_SETFL, O_NONBLOCK) == -1){
		logError << "could not set fcntl(_socket, F_SETFL, O_NONBLOCK)";
		return false;
	}
	return true;
	/*if( fcntl(_socket, F_SETFL, O_ASYNC) == -1){
		logError("could not set fcntl(_socket, F_SETFL, O_ASYNC)");
		return false;
	}*/
#endif
}

SocketHolder::SocketHolder()
{
#ifdef _WIN32
	socket = INVALID_SOCKET;
#else
	socket = -1;
#endif
}

void SocketHolder::close()
{
#ifdef _WIN32
	if(socket != INVALID_SOCKET){
		closesocket(socket);
		socket = INVALID_SOCKET;
	}
#else
	if(socket >= 0){
		close(socket);
		socket = -1;
	}
#endif
}

}
