#ifndef NETWORK_H_INCLUDED
#define NETWORK_H_INCLUDED

#include <string>
#include <stdint.h>

class sockaddr_in;

#ifdef WIN32
bool InitNetwork();
#else
bool InitNetwork() {return true;}; //Unix braucht das nicht
#endif

int CreateTCPServer(unsigned short port, bool nonblock);
int OpenTCPStream(const std::string& server, unsigned short port);

int CreateUDPStream(unsigned short port);

void closeSocket(int socket);

void SendUDPMSG(const int socket, const char* data, int dataSize, uint32_t targetIP, unsigned short targetPort);
void SendUDPMSG(const int socket, const char* data, int dataSize, const sockaddr_in& target);

int getHostIP(const std::string& hostname);

#endif
