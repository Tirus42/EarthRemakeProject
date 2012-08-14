#ifndef TESTSERVER_H
#define TESTSERVER_H

#include <stdint.h>
#include <string>
#include <list>

class HumanPlayer;
class Map;

class TestServer {
	private:
		static const uint32_t BUFFERSIZE = 8192;

		TestServer(const TestServer&);
		TestServer operator=(const TestServer&);

		int32_t socket;	 	//TCP-Server socket, hört auf Eingehende Verbindungen
		char* netbuffer;	//Bufferspeicher für das lesen und schreiben von Netzwerknachrichten

		Map* map;		//Jaja unschön, nur zum Test...

		void acceptNewConnections();
		void handleNewConnections();

		void checkIncommingData();
		void handleIncommingData(HumanPlayer* player, int32_t size);

		void createHumanPlayer();

		void sendMapDataRaw(Map* map, HumanPlayer* player);
		void sendMapWaymapRaw(Map* map, HumanPlayer* player);

		std::list<int32_t> waitingConnections;  //Speichert alle eingehende Verbindungen, bis eine Anfrage kommt
		std::list<HumanPlayer*> players;

		std::string peekString(uint32_t offset); //Liest aus dem netbuffer an angegebenen Offset einen String aus

	public:
		TestServer(int32_t socket);
		virtual ~TestServer();

		char* getNetbufferPtr() const {return netbuffer;}

		void run(Map* m);	//Nur eine Test Impelemtierung!!!!! (nicht verändern / schöner machen!)

};

#endif // TESTSERVER_H
