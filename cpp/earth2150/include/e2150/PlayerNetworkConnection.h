#ifndef PLAYERNETWORKCONNECTION_H_INCLUDED
#define PLAYERNETWORKCONNECTION_H_INCLUDED

#include "tf/network.h"
#include <stdint.h>
#include <list>

class SendBuffer;

class PlayerNetworkConnection {
	friend class HumanPlayer;

	private:
        PlayerNetworkConnection(const PlayerNetworkConnection &pnc);
		PlayerNetworkConnection& operator=(const PlayerNetworkConnection &pnc);

		SOCKET socket;
		sockaddr_in networkAdress;

		std::list<SendBuffer*> sendBuffers;   //Netzwerkbuffer, welche noch an den Spieler gesendet werden müssen

	public:
		PlayerNetworkConnection(SOCKET socket, const sockaddr_in& networkAdress);
		virtual ~PlayerNetworkConnection();

		SOCKET getSocket() const {return socket;}

		void sendPacket(char* pointer, int32_t length);	//Sendet angegebene Daten an den Client (behält Reihenfolge!)
		void sendBufferContent();   //Sendet ggf. ausstehende Daten an den Client

};

#endif // PLAYERNETWORKCONNECTION_H_INCLUDED
