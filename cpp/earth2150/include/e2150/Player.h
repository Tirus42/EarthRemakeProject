#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include <string>
#include <vector>

#include <stdint.h>

class Map;

/**
* Klasse stellt jede Art von Spieler dar (mit z.T. abstrakten Funktionen)
* Bots und Menschliche Spieler erben von dieser Klasse und implementieren die
* erforderlichen funktionen
*/
class Player {
	private:
		std::string name;

		/// Karten auf denen der Spieler aktiv ist und/oder Einsicht hat
		std::vector<const Map*> maps;

	public:
		Player(const std::string& name) : name(name), maps(0) {};
		virtual ~Player();

		/// Gibt den Namen des Spielers zurück
		const std::string& getName() const {return name;}

		/// Fügt eine weitere Karte in den "Sichtbereich" des Spielers hinzu
		void addVisibleMap(const Map& map) {maps.push_back(&map);}

		/// Entfernt eine Karte aus dem "Sichtbereich" des Spielers
		void removeVisibleMap(const Map& map);

		/// Gibt die Anzahl an Karten zurück, in welcher des Spieler einsicht hat
		uint32_t countVisibleMaps() const {return maps.size();}
};

#endif
