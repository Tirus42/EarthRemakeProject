#ifndef MAP_H
#define MAP_H

#include "e2150/Map.h"
#include "e2150/MapPosition.h"
#include <stdint.h>
#include <vector>
#include <string>

class Navigator;
class Unit;

/**
* Map Klasse
* Beinhaltet die HeightMap sowie die Wegemap
* und Verwaltet alle Spielobjekte auf der Karte
*/
class MapImpl : public Map {
	private:
		const static uint16_t MAX_HEIGHTDIFF = 2000;

		MapImpl(const MapImpl&);
		void operator=(const MapImpl&);

		std::vector<uint16_t> heightMap;
		std::vector<uint8_t> movementMap;  //Karte der Bewegungsm�glichkeiten
		uint32_t borderWidth; //Die Anzahl an Felder an Rand, die nicht nutzbar f�r die Spieler ist
		Navigator* navigator;

		uint32_t getNumberOfMoveableFields() const; //Anzahl der Felder, auf denen sich Bewegt werden kann (Weggitter hat verbindung)
		uint16_t getHeightDiffOnField(uint32_t position) const;

	public:
		MapImpl(uint16_t width, uint16_t height);
		virtual ~MapImpl();

		/// Berechnet die Bewegungsm�glichkeiten von jedem Feld auf der Map neu
		void updateMovementMap();

		///Berechnet die Bewegungsm�glichkeiten innerhalb der angegebenen Felder auf der Map neu
		void updateMovementMap(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);

		///Berechnet die Bewegungsm�glichkeiten innerhalb der angegebenen Felder auf der Map neu
		void updateMovementMap(uint32_t position1, uint32_t position2);

		void updateMovementMapWithBorder();

		virtual std::vector<MapPosition> getNeighbourPositions(uint32_t x, uint32_t y) const;
		virtual std::vector<MapPosition> getWay(const Unit& unit, uint32_t destination) const;

		uint8_t getDirections(uint32_t x, uint32_t y) const {return movementMap[position(x, y)];}

		inline uint16_t getRawHeight(uint32_t offset) const {return heightMap[offset];}
		inline uint8_t getRawWay(uint32_t offset) const {return movementMap[offset];}

		const Navigator* getNavigator() const {return navigator;}

		bool loadHeightMapRAW(const std::string& filename);  //L�d eine Heightmap 1:1 aus einer Datei
};

#endif