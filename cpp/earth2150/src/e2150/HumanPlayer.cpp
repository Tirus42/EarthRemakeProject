#include "e2150/HumanPlayer.h"

#include "e2150/Utils.h"
#include <iostream>

HumanPlayer::HumanPlayer(TestServer* server, int32_t socket, const std::string& name, const sockaddr_in& networkAddress) :
		Player(name),
		server(server),
		connection(socket, networkAddress),
		cameras(0) {
}

HumanPlayer::~HumanPlayer() {
	//Todo: Alle Kameras entfernen
	std::cout << "Human Player Destruktor aufgerufen\n";
}



void HumanPlayer::debugPaintFields(const std::list<uint32_t>& fields, uint32_t color) {
	char* netbuffer = server->getNetbufferPtr();

	netbuffer[0] = 255;

	*(uint32_t*)&netbuffer[1] = color;
	*(uint32_t*)&netbuffer[5] = fields.size();

	uint32_t offset = 9;

	for (std::list<uint32_t>::const_iterator i=fields.begin(); i != fields.end(); ++i) {
		*(uint32_t*)&netbuffer[offset] = *i;
		offset += 4;
	}

	connection.sendPacket(netbuffer, 4 + offset);
}
