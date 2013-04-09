#ifndef TESTSERVER_H
#define TESTSERVER_H

#include "tf/network.h"

#include <stdint.h>
#include <string>
#include <map>
#include <list>

class HumanPlayer;
class Map;
class UnitChassis;
class Unit;

class TestServer {
	private:
		static const uint32_t BUFFERSIZE = 8192;

		SOCKET socket;	 	//TCP-Server socket, hört auf Eingehende Verbindungen
		char* netbuffer;	//Bufferspeicher für das lesen und schreiben von Netzwerknachrichten
		Map& map;		//Jaja unschön, nur zum Test...
		std::list<SOCKET> waitingConnections;  //Speichert alle eingehende Verbindungen, bis eine Anfrage kommt
		std::list<HumanPlayer*> players;

		std::map<uint32_t, const UnitChassis*> unitChassis;
		uint32_t lastGivenEntityID;


		TestServer(const TestServer&);
		TestServer operator=(const TestServer&);

		void acceptNewConnections();
		void handleNewConnections();

		void checkIncommingData();
		void handleIncommingData(HumanPlayer& player, int32_t size);

		/// Entfernt einen Spieler aus dem Spiel
		void removeHumanPlayer(HumanPlayer& player);

		void sendMapDataRaw(const Map& map, HumanPlayer& player) const;
		void sendMapWaymapRaw(const Map& map, HumanPlayer& player) const;
		void sendChassisList(HumanPlayer& player);

		/// Sendet die Liste aller Einheiten auf der Karte an den Spieler
		void sendUnitList(HumanPlayer& player);

		uint32_t getFreeEntityID();

		std::string peekString(uint32_t offset); //Liest aus dem netbuffer an angegebenen Offset einen String aus
		uint32_t pokeString(const std::string& text, uint32_t offset);
	public:
		TestServer(SOCKET socket, Map& map);
		virtual ~TestServer();

		/// Fügt eine weitere UnitChassis zu den Spawnmöglichkeiten hinzu
		void addUnitChassis(const UnitChassis& chassis);

		/// Erstellt (wenn möglich) eine neue Einheit auf der Karte
		bool createUnit(const UnitChassis& chassis, uint16_t x, uint16_t y);

		/// Gibt einer Einheit den Befehl zu einem Ziel zu fahren
		void sendUnitToPosition(uint32_t unitID, uint32_t position);

		char* getNetbufferPtr() const {return netbuffer;}

		void run();	//Nur eine Test Impelemtierung!!!!! (nicht verändern / schöner machen!)

};

#endif // TESTSERVER_H
