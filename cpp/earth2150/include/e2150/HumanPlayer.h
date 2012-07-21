#ifndef HUMANPLAYER_H
#define HUMANPLAYER_H

#include <list>

#include "e2150/Player.h"
#include "e2150/SendBuffer.h"
#include "e2150/Map.h"

#include "tf/network.h"

/**
* Jeder über das Netzwerk verbundene Spieler wird in dieser Klasse gespeichert
* Die Klasse sendet alle (nötigen) Events die an den Player gehen, über das Netzwerk an den Spieler weiter
*/
class HumanPlayer: public Player {
    private:
        HumanPlayer(const HumanPlayer&);
        HumanPlayer operator=(const HumanPlayer&);

        int32_t socket;
        sockaddr_in networkAdress;

        Map* currentMap;    //Zeigen auf die Spielkarte, worauf der Spieler gerade seine Kamera hat

        std::list<SendBuffer*> sendBuffers;   //Netzwerkbuffer, welche noch an den Spieler gesendet werden müssen

    public:
        HumanPlayer(int32_t socket, const std::string& name, sockaddr_in networkAdress);
        ~HumanPlayer();

        int32_t getSocket() const{return socket;}

        void sendPacket(char* pointer, uint32_t length);    //Sendet angegebene Daten an den Client (behält Reihenfolge!)
        void sendBufferContent();   //Sendet ggf. ausstehende Daten an den Client
};

#endif
