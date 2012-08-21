#ifndef TESTSERVER_H
#define TESTSERVER_H

#include <stdint.h>
#include <string>
#include <vector>
#include <list>

class HumanPlayer;
class MapImpl;
class UnitChassis;

class TestServer {
	private:
		static const uint32_t BUFFERSIZE = 8192;

		int32_t socket;	 	//TCP-Server socket, hört auf Eingehende Verbindungen
		char* netbuffer;	//Bufferspeicher für das lesen und schreiben von Netzwerknachrichten
		MapImpl& map;		//Jaja unschön, nur zum Test...
		std::list<int32_t> waitingConnections;  //Speichert alle eingehende Verbindungen, bis eine Anfrage kommt
		std::list<HumanPlayer*> players;

		std::vector<const UnitChassis*> unitChassis;

		TestServer(const TestServer&);
		TestServer operator=(const TestServer&);

		void acceptNewConnections();
		void handleNewConnections();

		void checkIncommingData();
		void handleIncommingData(HumanPlayer& player, int32_t size);

		void createHumanPlayer();
		void removeHumanPlayer(HumanPlayer& player);

		void sendMapDataRaw(const MapImpl& map, HumanPlayer& player);
		void sendMapWaymapRaw(const MapImpl& map, HumanPlayer& player);

		void sendChassisList(HumanPlayer& player);

		std::string peekString(uint32_t offset); //Liest aus dem netbuffer an angegebenen Offset einen String aus
		uint32_t pokeString(const std::string& text, uint32_t offset);
	public:
		TestServer(int32_t socket, MapImpl& map);
		virtual ~TestServer();

		void addUnitChassis(const UnitChassis& chassis);

		char* getNetbufferPtr() const {return netbuffer;}

		void run();	//Nur eine Test Impelemtierung!!!!! (nicht verändern / schöner machen!)

};

#endif // TESTSERVER_H
