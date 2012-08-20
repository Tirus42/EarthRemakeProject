#ifndef PLAYERNETWORKCONNECTION_H_INCLUDED
#define PLAYERNETWORKCONNECTION_H_INCLUDED

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

#include "tf/network.h"
#include <stdint.h>
#include <list>

class SendBuffer;

class PlayerNetworkConnection {
	friend class HumanPlayer;

	private:
		int32_t socket;
		sockaddr_in networkAdress;

		std::list<SendBuffer*> sendBuffers;   //Netzwerkbuffer, welche noch an den Spieler gesendet werden müssen

	public:
		PlayerNetworkConnection(int32_t socket, const sockaddr_in& networkAdress);
		virtual ~PlayerNetworkConnection();

		int32_t getSocket() const {return socket;}

		void sendPacket(char* pointer, uint32_t length);	//Sendet angegebene Daten an den Client (behält Reihenfolge!)
		void sendBufferContent();   //Sendet ggf. ausstehende Daten an den Client

};

#endif // PLAYERNETWORKCONNECTION_H_INCLUDED
