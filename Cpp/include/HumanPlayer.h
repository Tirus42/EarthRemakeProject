#ifndef HUMANPLAYER_H_INCLUDED
#define HUMANPLAYER_H_INCLUDED

#include "Player.h"
#include "funktionen/network.h"

/**
* Jeder über das Netzwerk verbundene Spieler wird in dieser Klasse gespeichert
* Die Klasse sendet alle (nötigen) Events die an den Player gehen, über das Netzwerk an den Spieler weiter
*/
class HumanPlayer : public Player {
    private:
        sockaddr_in networkAdress;
    public:
        HumanPlayer(const std::string& name, const sockaddr_in& networkAdress);
};

#endif
