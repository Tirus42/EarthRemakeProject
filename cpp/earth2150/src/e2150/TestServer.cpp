#include "e2150/TestServer.h"

#include "e2150/Utils.h"

#include "e2150/HumanPlayer.h"
#include "e2150/UnitChassis.h"
#include "e2150/Map.h"
#include "e2150/AStar.h"
#include "e2150/Unit.h"
#include "e2150/Navigator.h"
#include "tf/time.h"

#include <iostream>
#include <cstring>

TestServer::TestServer(int32_t socket, Map& map):
		socket(socket),
		netbuffer(new char[BUFFERSIZE]),
		map(map),
		waitingConnections(),
		players(),
		unitChassis(),
		lastGivenEntityID(0) {
	std::cout << "Server erstellt!\n";
}

TestServer::~TestServer() {
	delete netbuffer;
}

void TestServer::addUnitChassis(const UnitChassis& chassis) {
	unitChassis.insert(std::pair<uint32_t, const UnitChassis*>(chassis.getID(), &chassis));
}

bool TestServer::createUnit(const UnitChassis& chassis, uint16_t x, uint16_t y) {
	Unit* unit = new Unit(getFreeEntityID(), chassis);

	if (!map.addUnit(*unit, x, y)) {
		delete unit;
		return false;
	}

	sendUnitSpawn(*unit);

	return true;
}

void TestServer::sendUnitToPosition(uint32_t unitID, uint32_t position) {
	Unit* unit = map.getUnit(unitID);

	std::cout << "Unit: " << unit << "\n";

	if (unit == 0)
		return;

	map.UnitDriveTo(*unit, position);

}

void TestServer::sendUnitSpawn(const Unit& unit) {
	netbuffer[0] = 200;
	int32_t size = unit.dumpData(&netbuffer[1]);

	for (std::list<HumanPlayer*>::iterator i = players.begin(); i != players.end(); ++i) {
		(*i)->getConnection().sendPacket(netbuffer, size + 1);
	}
}

void TestServer::run() {

	const uint32_t frameTime = 50;	//20FPS == 50ms pro Frame

	// Tirus: Da ich leider noch keine "Timer" Funktion unter Linux finden konnte
	// hier diese Fallunterscheidung, unter Linux ist die FrameZeit daher z.Z. nicht garantiert
    #ifdef WIN32
    int32_t timer = CreateTimer(frameTime);
    #else
    timespec time;
    time.tv_sec = 0;
    time.tv_nsec = frameTime * 1000000L;
    #endif

	uint32_t rTime;

	while (true) {
		rTime = MilliSecs();

		acceptNewConnections();
		handleNewConnections();
		checkIncommingData();

		if (MilliSecs() - rTime > frameTime) {
			std::cout << "Warnung: Frame Time dauerte " << (MilliSecs() - rTime) << "ms\n";
		}

        #ifdef WIN32
            WaitTimer(timer);
        #else
            nanosleep(&time, 0);
        #endif
	}

}

void TestServer::checkIncommingData() {
	for (std::list<HumanPlayer*>::iterator i = players.begin(); i != players.end(); ++i) {
		int32_t socket = (*i)->getConnection().getSocket();

		int32_t size = socketRecv(socket, netbuffer, BUFFERSIZE, true);

		// Bei -1 keine neuen Daten, bei 0 Verbindung getrennt
		if (size > 0) {
			handleIncommingData(*(*i), size);
		} else if (size == 0) {
			removeHumanPlayer(*(*i));
			return;
		}

		//Wenn noch Daten zum senden anstehen, dann senden
		(*i)->getConnection().sendBufferContent();
	}
}

void TestServer::handleIncommingData(HumanPlayer& player, int32_t size) {
	int32_t socket = player.getConnection().getSocket();

	switch (netbuffer[0]) {
		case 0x01:	/// Anfrage des Clients der ganzen Map
			socketRecv(socket, netbuffer, 1, false);
			sendMapDataRaw(map, player);
			break;

		case 0x02:	/// Anfrage des Clients nach der Wegekarte
			socketRecv(socket, netbuffer, 1, false);
			sendMapWaymapRaw(map, player);
			break;

		case 0x03: {	/// Anfrage des Clients nach einer Wegberechnung
			socketRecv(socket, netbuffer, 9, 0);

			uint32_t start = *((uint32_t*)&netbuffer[1]);
			uint32_t target = *((uint32_t*)&netbuffer[5]);

			std::cout << "Suche Weg von (" << map.positionX(start) << ":" << map.positionY(start);
			std::cout << ") zu (" << map.positionX(target) << ":" << map.positionY(target) << ")\n";


			int32_t time = MilliSecs();
			std::list<uint32_t> liste;
			map.getWay(start, target, liste);
			time = MilliSecs() - time;

			std::cout << "Die Wegsuche dauerte " << time << "ms"
				<< " und hat einen Weg ueber " << liste.size() << " Felder gefunden!\n";

			player.debugPaintFields(liste, 0xFFAA00U);
		}
		break;

		case 0x04:		/// Anfrage des Clients nach der Chassis Liste
			socketRecv(socket, netbuffer, 1, false);
			sendChassisList(player);
			break;

		case 0x05: {		/// Client möchte eine Unit spawnen
			socketRecv(socket, netbuffer, 9, false);

			uint32_t chassisID = *((uint32_t*)&netbuffer[1]);
			uint32_t position = *((uint32_t*)&netbuffer[5]);

			int16_t x = map.positionX(position);
			int16_t y = map.positionY(position);

			std::cout << "Erhalte spawn Befehl!\n";

			const UnitChassis* chassis = unitChassis[chassisID];

			if (chassis != 0) {
				createUnit(*chassis, x, y);
			}
		}
		break;

		case 0x06: {	/// Anfrage der Einheiten auf der ganzen Karte
			socketRecv(socket, netbuffer, 1, false);
			sendUnitList(player);
			break;
		}

		case 0x07: {	/// Eine Einheit soll zu einer Position fahren
			socketRecv(socket, netbuffer, 9, false);
			uint32_t unitID = *((uint32_t*)&netbuffer[1]);
			uint32_t position = *((uint32_t*)&netbuffer[5]);
			sendUnitToPosition(unitID, position);
			break;
		}

		default:
			socketRecv(socket, netbuffer, 1, false);
			std::cout << "Unbekanntes Paket eingegangen! (" << (int)netbuffer[0] << ")\n";
	}
}

void TestServer::removeHumanPlayer(HumanPlayer& player) {
	players.remove(&player);
	delete &player;
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
				socketRecv(socket, netbuffer, 4, false);

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

void TestServer::sendMapDataRaw(const Map& map, HumanPlayer& player) const {
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

void TestServer::sendMapWaymapRaw(const Map& map, HumanPlayer& player) const {
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

	int32_t offset = 4;
	for (std::map<uint32_t, const UnitChassis*>::const_iterator i = unitChassis.begin(); i != unitChassis.end(); ++i) {
		const UnitChassis* c = (*i).second;

		*(uint32_t*)(&netbuffer[offset]) = c->getID();
		offset = pokeString(c->getModel(), offset+4);
		offset = pokeString(c->getName(), offset);

		*(uint32_t*)(&netbuffer[offset])	= c->getTurnRate();
		*(uint32_t*)(&netbuffer[offset+4])	= c->getMoveRate();
		*(uint32_t*)(&netbuffer[offset+8])	= c->getBuildTime();
		*(uint32_t*)(&netbuffer[offset+12])	= c->getHitPoints();

		offset += 16;
	}

	player.getConnection().sendPacket(netbuffer, offset);
}

void TestServer::sendUnitList(HumanPlayer& player) {
	std::cout << "Sende Einheiten Liste\n";

	const std::map<uint32_t, Unit*> units = map.getUnits();

	*(uint32_t*)(&netbuffer[0]) = units.size();

	// Todo: Prüfen ob Speicher wirklich groß genug für alle und ggf. neuen anlegen

	int32_t offset = 4;
	for (std::map<uint32_t, Unit*>::const_iterator i = units.begin(); i != units.end(); ++i) {
		const Unit* u = (*i).second;

		offset += u->dumpData(&netbuffer[offset]);
	}

	player.getConnection().sendPacket(netbuffer, offset);
}

uint32_t TestServer::getFreeEntityID() {
	return ++lastGivenEntityID;
}
