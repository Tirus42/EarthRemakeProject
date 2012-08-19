#ifndef PLAYERVIEWAREA_H_INCLUDED
#define PLAYERVIEWAREA_H_INCLUDED

#include <stdint.h>

#include "e2150/MapPosition.h"

class Map;
class HumanPlayer;

/**
* Diese Klasse beschreibt die Kamera Position des Spielers.
* Jede "Kamera" des Spielers wird von einem Objekt dieser Klasse zugewiesen.
* Mit Hilfe dieser Daten ist die Map-Klasse in der Lage zu bestimmen,
* welche Daten für den Spieler relevant sind
*/
class PlayerViewArea {
	private:
		MapPosition position;
		uint32_t radius;

	public:
		///Erstellt eine neue "Kamera" des Spielers, und weißt der Map diese neue Kamera zu
		PlayerViewArea(Map* map, MapPosition& position, uint32_t radius);

		void setRadius(uint32_t radius) {this->radius = radius;}
		void setPosition(int16_t x, int16_t y) {position.setPosition(x, y);}

		uint32_t getRadius() const {return radius;}
		uint16_t getX() const {return position.getX();}
		uint16_t getY() const {return position.getY();}
};

#endif // PLAYERVIEWAREA_H_INCLUDED
