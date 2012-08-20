#include "e2150/PlayerNetworkConnection.h"
#include "e2150/SendBuffer.h"

#include <iostream>

PlayerNetworkConnection::PlayerNetworkConnection(int32_t socket, const sockaddr_in& networkAdress) :
	socket(socket),
	networkAdress(networkAdress),
	sendBuffers() {

}

PlayerNetworkConnection::~PlayerNetworkConnection() {
	closesocket(socket);
	std::cout << "Netzwerkverbindung zu Spieler getrennt!\n";
}

void PlayerNetworkConnection::sendPacket(char* pointer, uint32_t length) {
	std::cout << "Sende " << length << " bytes\n";

	if (sendBuffers.size() != 0) { //Wenn bereits etwas im Buffer liegt, dann hinten dran einfügen
		SendBuffer* b = new SendBuffer(pointer, length);
		sendBuffers.push_back(b);
		return;
	}

	//Wenn Buffer leer, dann versuche es direkt zu senden, wenn nicht erfolgreich, schreibe es in den Buffer
	if (send(socket, pointer, length, 0) == SOCKET_ERROR) {
		std::cout << "Lagere aus\n";
		SendBuffer* b = new SendBuffer(pointer, length);
		sendBuffers.push_back(b);
	}
};

void PlayerNetworkConnection::sendBufferContent() {
	for (std::list<SendBuffer*>::iterator i=sendBuffers.begin();i!=sendBuffers.end();/*kein zähler*/) {
		if((*i)->sendPacket(socket) == true){
			delete (*i);	//Buffer wurde gesendet, also kann er wieder freigegeben werden
			i = sendBuffers.erase(i);
		}
		else{   //Offenbar Limit wieder erreicht, also hier abbrechen
			return;
		}
	}
}
