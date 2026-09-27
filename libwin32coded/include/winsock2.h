#pragma once
#include "windows.h"

#ifndef __WIN32
#include <cerrno>
#include <fcntl.h>
#include <poll.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>

using SOCKET = int;
using WSAPOLLFD = struct pollfd;
struct WSADATA {};
#define INVALID_SOCKET (-1)
#define SOCKET_ERROR (-1)
#ifndef POLLRDNORM
#define POLLRDNORM POLLIN
#endif
#ifndef POLLWRNORM
#define POLLWRNORM POLLOUT
#endif
#ifndef POLLRDBAND
#define POLLRDBAND POLLPRI
#endif
#define WSAEWOULDBLOCK EWOULDBLOCK
#define MAKEWORD(low, high) 0
inline int WSAStartup(unsigned short, WSADATA*) { return 0; }
inline int WSACleanup() { return 0; }
inline int WSAGetLastError() { return errno; }
inline int WSAPoll(WSAPOLLFD* fds, unsigned long count, int timeout) {
    return poll(fds, count, timeout);
}
inline int closesocket(SOCKET s) { return close(s); }
inline int ioctlsocket(SOCKET s, long request, unsigned long* value) {
    return ioctl(s, request, value);
}
#endif
