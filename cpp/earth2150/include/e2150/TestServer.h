#ifndef TESTSERVER_H
#define TESTSERVER_H

#include <stdint.h>
#include <string>
#include <map>
#include <list>

class HumanPlayer;
class MapImpl;
class UnitChassis;
class Unit;

class TestServer {
	private:
		static const uint32_t BUFFERSIZE = 8192;

		int32_t socket;	 	//TCP-Server socket, h�rt auf Eingehende Verbindungen
		char* netbuffer;	//Bufferspeicher f�r das lesen und schreiben von Netzwerknachrichten
		MapImpl& map;		//Jaja unsch�n, nur zum Test...
		std::list<int32_t> waitingConnections;  //Speichert alle eingehende Verbindungen, bis eine Anfrage kommt
		std::list<HumanPlayer*> players;

		std::map<uint32_t, const UnitChassis*> unitChassis;
		uint32_t lastGivenEntityID;


		TestServer(const TestServer&);
		TestServer operator=(const TestServer&);

		void acceptNewConnections();
		void handleNewConnections();

		void checkIncommingData();
		void handleIncommingData(HumanPlayer& player, int32_t size);

		void createHumanPlayer();
		void removeHumanPlayer(HumanPlayer& player);

		void sendMapDataRaw(const MapImpl& map, HumanPlayer& player) const;
		void sendMapWaymapRaw(const MapImpl& map, HumanPlayer& player) const;
		void sendChassisList(HumanPlayer& player);

		/// Sendet die Liste aller Einheiten auf der Karte an den Spieler
		void sendUnitList(HumanPlayer& player);

		/// Sendet ein Spawn Paket an alle Spieler
		void sendUnitSpawn(const Unit& unit);

		uint32_t getFreeEntityID();

		std::string peekString(uint32_t offset); //Liest aus dem netbuffer an angegebenen Offset einen String aus
		uint32_t pokeString(const std::string& text, uint32_t offset);
	public:
		TestServer(int32_t socket, MapImpl& map);
		virtual ~TestServer();

		/// F�gt eine weitere UnitChassis zu den Spawnm�glichkeiten hinzu
		void addUnitChassis(const UnitChassis& chassis);

		/// Erstellt (wenn m�glich) eine neue Einheit auf der Karte
		bool createUnit(const UnitChassis& chassis, uint16_t x, uint16_t y);

		char* getNetbufferPtr() const {return netbuffer;}

		void run();	//Nur eine Test Impelemtierung!!!!! (nicht ver�ndern / sch�ner machen!)

};

#endif // TESTSERVER_H
