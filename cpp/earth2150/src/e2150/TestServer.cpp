#include "e2150/TestServer.h"

#include "e2150/Utils.h"

#include "e2150/HumanPlayer.h"
#include "e2150/UnitChassis.h"
#include "e2150/MapImpl.h"
#include "e2150/AStar.h"
#include <iostream>
#include <cstring>

TestServer::TestServer(int32_t socket, MapImpl& map):
		socket(socket),
		netbuffer(new char[BUFFERSIZE]),
		map(map),
		waitingConnections(),
		players(),
		unitChassis() {
	std::cout << "Server erstellt!\n";
}

TestServer::~TestServer() {
	delete netbuffer;
}

void TestServer::addUnitChassis(const UnitChassis& chassis) {
	unitChassis.push_back(&chassis);
}

void TestServer::run() {

    #ifndef WIN32
    timespec time;
    time.tv_sec = 0;
    time.tv_nsec = 50000000L;
    #endif

	while (true) {
		acceptNewConnections();
		handleNewConnections();
		checkIncommingData();

        #ifdef WIN32
            Sleep(50);
        #else
            nanosleep(&time, 0);
        #endif

	}

}

void TestServer::checkIncommingData() {
	for (std::list<HumanPlayer*>::iterator i = players.begin(); i != players.end(); ++i) {
		int32_t socket = (*i)->getConnection().getSocket();

		int32_t size = recv(socket, netbuffer, BUFFERSIZE, MSG_PEEK);

		if (size > 0) {
			handleIncommingData(*(*i), size);
		}

		//Wenn noch Daten zum senden anstehen, dann senden
		(*i)->getConnection().sendBufferContent();
	}
}

void TestServer::handleIncommingData(HumanPlayer& player, int32_t size) {
	int32_t socket = player.getConnection().getSocket();

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

			std::cout << "Suche Weg von (" << map.positionX(start) << " " << map.positionY(start);
			std::cout << ") zu (" << map.positionX(target) << " " << map.positionY(target) << ")\n";

			std::list<MapPosition> liste = Utils::vectorToList(
						map.getNavigator()->getPath(map, start, target));

			player.debugPaintFields(
				Utils::mapPositionToPosition(
					map, liste), 0xFFAA00U);
		}
		break;

		case 0x04:
			socketRecv(socket, netbuffer, 1, false);
			sendChassisList(player);
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

            //Setze Verbindung in NonBlock mode (in Windows wird das übernommen, in Linux nicht...)
            setSocketNonblock(accepted);

			//Füge neue Verbindung in Warteliste ein
			waitingConnections.push_back(accepted);
		}
	} while (accepted != INVALID_SOCKET);
}

void TestServer::handleNewConnections() {
	for (std::list<int32_t>::iterator i = waitingConnections.begin(); i != waitingConnections.end(); ++i) {
		int32_t socket = (*i);

		int32_t size = socketRecv(socket, netbuffer, BUFFERSIZE, true);

        std::cout << "Paket: " << size << std::endl;

		if (size == 0) {
            closeSocket(socket);
            waitingConnections.erase(i);
            std::cout << "Ungueltige eingehende Verbindung geschlossen\n";
            return;
		} else if (size >= 4) {
			//Kleine Test-Header erkennung
			if (*(unsigned int*)netbuffer == 0xABCDEF01) {
				//Nachricht komplett lesen
				recv(socket, netbuffer, 4, 0);

				HumanPlayer* player = new HumanPlayer(this, socket, std::string("Player"), sockaddr_in());

				players.push_back(player);

				//Sende ein Connection-ACK
				netbuffer[0] = 1;
				player->getConnection().sendPacket(netbuffer, 1);

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

uint32_t TestServer::pokeString(const std::string& text, uint32_t offset) {
	uint16_t length = text.length();

	*(uint16_t*)(&netbuffer[offset]) = length;
	memcpy((void*)&netbuffer[offset + 2], text.c_str(), length);

	return offset + length + 2;
}

void TestServer::sendMapDataRaw(const MapImpl& map, HumanPlayer& player) {
	std::cout << "Sende Karte\n";

	uint32_t dataSize = map.getWidth() * map.getHeight();
	uint16_t* buffer = new uint16_t[dataSize + 4];

	buffer[0] = map.getWidth();
	buffer[1] = map.getHeight();

	for (uint32_t i = 0; i < dataSize; ++i) {
		buffer[2 + i] = map.getRawHeight(i);
	}

	player.getConnection().sendPacket((char*)buffer, 4 + dataSize * 2);

	delete[] buffer;
}

void TestServer::sendMapWaymapRaw(const MapImpl& map, HumanPlayer& player) {
	std::cout << "Sende Wegekarte\n";

	uint32_t dataSize = map.getWidth() * map.getHeight();
	uint8_t* buffer = new uint8_t[dataSize];

	for (uint32_t i = 0; i < dataSize; ++i) {
		buffer[i] = map.getRawWay(i);
	}

	player.getConnection().sendPacket((char*)buffer, dataSize);

	delete[] buffer;
}

void TestServer::sendChassisList(HumanPlayer& player) {
	std::cout << "Sende Chassis Liste\n";

	*(int32_t*)(&netbuffer[0]) = unitChassis.size();

	int offset = 4;
	for (std::vector<const UnitChassis*>::const_iterator i = unitChassis.begin(); i != unitChassis.end(); ++i) {
		*(uint32_t*)(&netbuffer[offset]) = (*i)->getID();
		offset = pokeString((*i)->getModel(), offset+4);
		offset = pokeString((*i)->getName(), offset);

		*(uint32_t*)(&netbuffer[offset])	= (*i)->getTurnRate();
		*(uint32_t*)(&netbuffer[offset+4])	= (*i)->getMoveRate();
		*(uint32_t*)(&netbuffer[offset+8])	= (*i)->getBuildTime();
		*(uint32_t*)(&netbuffer[offset+12])	= (*i)->getHitPoints();

		offset += 16;
	}

	player.getConnection().sendPacket(netbuffer, offset);
}
