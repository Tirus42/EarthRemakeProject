#include "e2150/HumanPlayer.h"

#include "e2150/SendBuffer.h"
#include <iostream>

HumanPlayer::HumanPlayer(TestServer* server, int32_t socket, const std::string& name, sockaddr_in networkAddress) :
		Player(name),
		server(server),
		socket(socket),
		networkAdress(networkAdress),
		currentMap(0) {
}

HumanPlayer::~HumanPlayer() {

}

void HumanPlayer::sendPacket(char* pointer, uint32_t length) {
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

void HumanPlayer::sendBufferContent() {
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

void HumanPlayer::debugPaintFields(std::list<uint32_t>& fields, uint32_t color) {
	char* netbuffer = server->getNetbufferPtr();

	netbuffer[0] = 255;

	*(uint32_t*)&netbuffer[1] = color;
	*(uint32_t*)&netbuffer[5] = fields.size();

	uint32_t offset = 9;

	for (std::list<uint32_t>::iterator i=fields.begin(); i != fields.end(); ++i) {
		*(uint32_t*)&netbuffer[offset] = *i;
		offset += 4;
	}

	sendPacket(netbuffer, 4 + offset);
}
