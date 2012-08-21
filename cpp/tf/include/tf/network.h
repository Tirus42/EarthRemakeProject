#ifndef NETWORK_H_INCLUDED
#define NETWORK_H_INCLUDED

#include <string>
#include <stdint.h>

class sockaddr_in;

#ifndef WIN32
    #define NO_ERROR 0
	#define SOCKET_ERROR -1
	#define INVALID_SOCKET 0xFFFFFFFF
#endif

bool InitNetwork();

int CreateTCPServer(unsigned short port, bool nonblock);
int OpenTCPStream(const std::string& server, unsigned short port);

int CreateUDPStream(unsigned short port);

bool setSocketNonblock(int socket);

/**
* Sendet Daten an einen (verbundenen) Socket
* @return Die Anzahl an Bytes die tatsächlich gesendet wurden; -1 Falls ein Fehler auftrat
*/
int socketSend(int socket, char* buffer, int size);

/**
* Liest Daten aus dem Socket
* @return Größe der gelesenen Daten; -1 Wenn nichts zu lesen war (nonblock); 0 wenn die Verbindung getrennt wurde (TCP)
*/
int socketRecv(int socket, char* buffer, int size, bool peekOnly);


void closeSocket(int socket);

void SendUDPMSG(const int socket, const char* data, int dataSize, uint32_t targetIP, unsigned short targetPort);
void SendUDPMSG(const int socket, const char* data, int dataSize, const sockaddr_in& target);

int getHostIP(const std::string& hostname);

#endif
