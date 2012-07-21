#ifndef TESTSERVER_H
#define TESTSERVER_H

#include <stdint.h>
#include <list>

#include "e2150/Map.h"
#include "e2150/Player.h"
#include "e2150/HumanPlayer.h"

class TestServer {
    private:
        static const uint32_t BUFFERSIZE = 8192;

        //TestServer(const TestServer&);
        TestServer operator=(const TestServer&);

        int32_t socket;     //TCP-Server socket, hört auf Eingehende Verbindungen
        char* netbuffer;    //Bufferspeicher für das lesen und schreiben von Netzwerknachrichten

        Map* map;

        void acceptNewConnections();
        void handleNewConnections();

        void checkIncommingData();
        void handleIncommingData(HumanPlayer* player, int32_t size);

        void createHumanPlayer();

        void sendMapDataRaw(Map* map, HumanPlayer* player);

        std::list<int32_t> waitingConnections;  //Speichert alle eingehende Verbindungen, bis eine Anfrage kommt
        std::list<HumanPlayer*> players;

        std::string peekString(uint32_t offset); //Liest aus dem netbuffer an angegebenen Offset einen String aus

    public:
        TestServer(int32_t socket);
        virtual ~TestServer();

        void run(Map* m);    //Nur eine Test Impelemtierung!!!!! (nicht verändern / schöner machen!)

};

#endif // TESTSERVER_H
