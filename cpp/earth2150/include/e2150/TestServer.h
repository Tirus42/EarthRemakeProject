#ifndef TESTSERVER_H
#define TESTSERVER_H

#include <stdint.h>
#include <string>
#include <list>

class HumanPlayer;
class MapImpl;

class TestServer {
	private:
		static const uint32_t BUFFERSIZE = 8192;

		int32_t socket;	 	//TCP-Server socket, hört auf Eingehende Verbindungen
		char* netbuffer;	//Bufferspeicher für das lesen und schreiben von Netzwerknachrichten
		MapImpl& map;		//Jaja unschön, nur zum Test...
		std::list<int32_t> waitingConnections;  //Speichert alle eingehende Verbindungen, bis eine Anfrage kommt
		std::list<HumanPlayer*> players;

		TestServer(const TestServer&);
		TestServer operator=(const TestServer&);

		void acceptNewConnections();
		void handleNewConnections();

		void checkIncommingData();
		void handleIncommingData(HumanPlayer* player, int32_t size);

		void createHumanPlayer();

		void sendMapDataRaw(const MapImpl& map, HumanPlayer* player);
		void sendMapWaymapRaw(const MapImpl& map, HumanPlayer* player);

		std::string peekString(uint32_t offset); //Liest aus dem netbuffer an angegebenen Offset einen String aus


	public:
		TestServer(int32_t socket, MapImpl& map);
		virtual ~TestServer();

		char* getNetbufferPtr() const {return netbuffer;}

		void run();	//Nur eine Test Impelemtierung!!!!! (nicht verändern / schöner machen!)

};

#endif // TESTSERVER_H
