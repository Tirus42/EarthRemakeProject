#ifndef NETWORK_H_INCLUDED
#define NETWORK_H_INCLUDED

#ifdef WIN32
	#include <windows.h>
#else
	#include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>

    #include <fcntl.h>
    #include <netdb.h>
    #include <memory.h>

    #define NO_ERROR 0
    #define SOCKET_ERROR -1
#endif

#include <string>
#include <iostream>
#include <stdint.h>

bool InitNetwork();

int CreateTCPServer(unsigned short port, bool nonblock);
int OpenTCPStream(const std::string& server, unsigned short port);

int CreateUDPStream(unsigned short port);

void SendUDPMSG(const int socket, const char* data, int dataSize, uint32_t targetIP, unsigned short targetPort);
void SendUDPMSG(const int socket, const char* data, int dataSize, const sockaddr_in& target);

int getHostIP(const std::string& hostname);

#endif
