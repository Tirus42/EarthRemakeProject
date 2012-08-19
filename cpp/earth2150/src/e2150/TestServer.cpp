#include "e2150/TestServer.h"

#include "e2150/Utils.h"

#include "e2150/HumanPlayer.h"
#include "e2150/MapImpl.h"
#include "e2150/Astar.h"
#include <iostream>

TestServer::TestServer(int32_t socket, MapImpl& map):
		socket(socket),
		netbuffer(new char[BUFFERSIZE]),
		map(map),
		waitingConnections(),
		players() {
	std::cout << "Server erstellt!\n";
}

TestServer::~TestServer() {
	delete netbuffer;
}

void TestServer::run() {

	while (true) {
		acceptNewConnections();
		handleNewConnections();
		checkIncommingData();

		Sleep(50);
	}

}

void TestServer::checkIncommingData() {
	for (std::list<HumanPlayer*>::iterator i = players.begin(); i != players.end(); ++i) {
		int32_t socket = (*i)->getSocket();

		int32_t size = recv(socket, netbuffer, BUFFERSIZE, MSG_PEEK);

		if (size > 0) {
			handleIncommingData(*i, size);
		}
	}
}

void TestServer::handleIncommingData(HumanPlayer* player, int32_t size) {
	int32_t socket = player->getSocket();

	switch (netbuffer[0]) {
		case 0x01:	///Anfrage des Clients der ganzen Map
			recv(socket, netbuffer, 1, 0);
			sendMapDataRaw(map, player);
			break;

		case 0x02:	///Anfrage des Clients nach der Wegekarte
			recv(socket, netbuffer, 1, 0);
			sendMapWaymapRaw(map, player);
			break;

		case 0x03: {	///Anfrage des Clients nach einer Wegberechnung
			recv(socket, netbuffer, 9, 0);

			uint32_t start = *((uint32_t*)&netbuffer[1]);
			uint32_t target = *((uint32_t*)&netbuffer[5]);

			std::cout << "Suche Weg von " << map.positionX(start) << " " << map.positionY(start);
			std::cout << "zu " << map.positionX(target) << " " << map.positionY(target) << "\n";

			std::list<MapPosition> liste = Utils::vectorToList(
						map.getNavigator()->getPath(map, start, target));

			player->debugPaintFields(
				Utils::mapPositionToPosition(
					map, liste), 0x11FFU);
		}
		break;

		default:
			recv(socket, netbuffer, 1, 0);
			std::cout << "Unbekanntes Paket eingegangen! (" << (int)netbuffer[0] << ")\n";
	}
}

void TestServer::acceptNewConnections() {
	uint32_t accepted;
	do {
		accepted = accept(socket, NULL, NULL);

		if (accepted != INVALID_SOCKET){
			std::cout << "Neue Verbindung!\n";

			//Füge neue Verbindung in Warteliste ein
			waitingConnections.push_back(accepted);
		}
	} while (accepted != INVALID_SOCKET);
}

void TestServer::handleNewConnections() {
	for (std::list<int32_t>::iterator i = waitingConnections.begin(); i != waitingConnections.end(); ++i) {
		int32_t socket = (*i);

		int32_t size = recv(socket, netbuffer, BUFFERSIZE, MSG_PEEK);

		if (size == SOCKET_ERROR) {
			if (WSAGetLastError() == WSAECONNRESET) {
				closesocket(socket);
				waitingConnections.erase(i);
				std::cout << "Ungueltige eingehende Verbindung geschlossen\n";
				return;
			}
		} else if (size >= 4) {
			//Kleine Test-Header erkennung
			if (*(unsigned int*)netbuffer == 0xABCDEF01) {
				//Nachricht komplett lesen
				recv(socket, netbuffer, 4, 0);

				HumanPlayer* player = new HumanPlayer(this, socket, std::string("Player"), sockaddr_in());

				players.push_back(player);

				//Sende ein Connection-ACK
				netbuffer[0] = 1;
				player->sendPacket(netbuffer, 1);

				std::cout << "Neuer Spieler hinzugefuegt!\n";

				waitingConnections.erase(i);
				return;
			}
		}
	}
}

std::string TestServer::peekString (uint32_t offset) {
	uint16_t length = *(uint16_t*)&netbuffer[offset];

	std::string text;

	for (int32_t i = 2; i < length + 2; ++i){
		text += netbuffer[offset + i];
	}

	return text;
}

void TestServer::sendMapDataRaw(const MapImpl& map, HumanPlayer* player) {
	std::cout << "Sende Karte\n";

	uint32_t dataSize = map.getWidth() * map.getHeight();
	uint16_t* buffer = new uint16_t[dataSize + 4];

	buffer[0] = map.getWidth();
	buffer[1] = map.getHeight();

	for (uint32_t i = 0; i < dataSize; ++i) {
		buffer[2 + i] = map.getRawHeight(i);
	}

	player->sendPacket((char*)buffer, 4 + dataSize * 2);

	delete[] buffer;
}

void TestServer::sendMapWaymapRaw(const MapImpl& map, HumanPlayer* player) {
	std::cout << "Sende Wegekarte\n";

	uint32_t dataSize = map.getWidth() * map.getHeight();
	uint8_t* buffer = new uint8_t[dataSize];

	for (uint32_t i = 0; i < dataSize; ++i) {
		buffer[i] = map.getRawWay(i);
	}

	player->sendPacket((char*)buffer, dataSize);

	delete[] buffer;
}
