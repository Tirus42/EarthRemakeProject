#include "e2150/HumanPlayer.h"

#include "e2150/Utils.h"
#include <iostream>

HumanPlayer::HumanPlayer(TestServer* server, int32_t socket, const std::string& name, const sockaddr_in& networkAddress) :
		Player(name),
		server(server),
		connection(socket, networkAddress) {
}

HumanPlayer::~HumanPlayer() {
	//Todo: Alle Kameras entfernen
	std::cout << "Human Player Destruktor aufgerufen\n";

	if (countVisibleMaps() > 0) {
		std::cout << "WARNUNG: Player ist noch in " << countVisibleMaps() << " Map(s) eingetragen!\n";
	}
}

void HumanPlayer::debugPaintFields(const std::list<uint32_t>& fields, uint32_t color) {
	char* buffer = new char[9 + fields.size()*4];

	buffer[0] = 255;

	*(uint32_t*)&buffer[1] = color;
	*(uint32_t*)&buffer[5] = fields.size();

	uint32_t offset = 9;

	for (std::list<uint32_t>::const_iterator i=fields.begin(); i != fields.end(); ++i) {
		*(uint32_t*)&buffer[offset] = *i;
		offset += 4;
	}

	connection.sendPacket(buffer, offset);
}
