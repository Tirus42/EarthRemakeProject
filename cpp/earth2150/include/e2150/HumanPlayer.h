#ifndef HUMANPLAYER_H
#define HUMANPLAYER_H

#include "e2150/Player.h"
#include "Network/TestServer.h"
#include "Network/PlayerNetworkConnection.h"
#include <vector>


class TestServer;
class Map;
class PlayerViewArea;

/**
* Jeder über das Netzwerk verbundene Spieler wird in dieser Klasse gespeichert
* Die Klasse sendet alle (nötigen) Events die an den Player gehen, über das Netzwerk an den Spieler weiter
*/
class HumanPlayer : public Player {
	private:
		TestServer* server;
		PlayerNetworkConnection connection;		//Netzwerkverbindung des Spielers

		HumanPlayer(const HumanPlayer&);
		HumanPlayer operator=(const HumanPlayer&);

	public:
		HumanPlayer(TestServer* server, SOCKET socket, const std::string& name, const sockaddr_in& networkAdress);
		virtual ~HumanPlayer();

		/// Setzt die Spieler-Netzwerkverbindung (kopiert das Objekt)
		void setConnection(const PlayerNetworkConnection& connection) {this->connection = connection;}

		/// Gibt die Spieler-Netzwerkverbindung zurück
		PlayerNetworkConnection& getConnection() {return connection;}

		/// Sendet eine Liste an Feldern an den Client, welche "angemalt" werden sollen
		void debugPaintFields(const std::list<uint32_t>& fields, uint32_t color);	//Zeichnet beim Client die angegebenen Felder farbig
};

#endif
