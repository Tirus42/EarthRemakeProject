#ifndef NETWORK_H_INCLUDED
#define NETWORK_H_INCLUDED

#include <string>
#include <stdint.h>

class sockaddr_in;

#ifdef WIN32
	#include "windows.h"
#else
	#include <netinet/in.h>

    #define NO_ERROR 0
	#define SOCKET_ERROR -1
	#define INVALID_SOCKET 0xFFFFFFFF

	#define SOCKET int
#endif

/// Initialisiert die Netzwerkumgebung (bei Anwendungsstart 1x Aufrufen)
bool InitNetwork();

/// Erstellt einen TCP-Socket und stellt ihn in den Listen mode
SOCKET CreateTCPServer(uint16_t port, bool nonblock);

/// Öffnet eine TCP Verbindung zum angegebenen Ziel
SOCKET OpenTCPStream(const std::string& server, uint16_t port);

/// Erstellt einen UDP-Socket (Todo: an Port binden) (...)
SOCKET CreateUDPStream(uint16_t port = 0);

/// Setzt einen Socket in den NonBlock mode (recv return damit sofort und wartet nicht bis etwas eingeht)
bool setSocketBlockmode(SOCKET socket, bool nonblock);

/// Gibt die Anzahl an Bytes an, die zum Lesen bereit stehen
unsigned int socketReadAvail(SOCKET socket);

/**
* Sendet Daten an einen (verbundenen) Socket
* @return Die Anzahl an Bytes die tatsächlich gesendet wurden; -1 Falls ein Fehler auftrat
*/
int socketSend(SOCKET socket, const char* buffer, int size);

/**
* Liest Daten aus dem Socket
* @return Größe der gelesenen Daten; -1 Wenn nichts zu lesen war (nonblock); 0 wenn die Verbindung getrennt wurde (TCP)
*/
int socketRecv(SOCKET socket, char* buffer, int size, bool peekOnly = false);

/// Gibt einen neuen SOCKET zurück, wenn ein Server-Socket eine Eingehende Verbindung hat
SOCKET socketAccept(SOCKET socket);

/// Schließt den Socket (und trennt damit ggf. die Verbindung)
void closeSocket(SOCKET socket);

/// Sendet ein UDP-Paket an die Zieladresse
void SendUDPMSG(SOCKET socket, const char* data, int dataSize, uint32_t targetIP, unsigned short targetPort);
void SendUDPMSG(SOCKET socket, const char* data, int dataSize, const sockaddr_in& target);

/// Löst einen Hostnamen in eine IPv4-Adresse auf
int getHostIP(const std::string& hostname);

#endif
