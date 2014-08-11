#pragma once

#include "base.h"
#include "socket.h"

#ifdef _WIN32
#include <winsock2.h>
#endif

#if defined(__linux__)
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#endif