#ifndef MAP_H
#define MAP_H

#include "e2150/Map.h"
#include "e2150/MapPosition.h"
#include "e2150/MapViewerManager.h"
#include <stdint.h>
#include <vector>
#include <string>
#include <map>

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
		std::vector<uint8_t> movementMap;	//Karte der Bewegungsmöglichkeiten
		std::vector<uint8_t> statusMap;		//Beschreibt den Status eines Feldes (eine Einheit steht drauf z.B.)
		uint32_t borderWidth; 				//Die Anzahl an Felder an Rand, die nicht nutzbar für die Spieler ist
		Navigator* navigator;

		std::map<uint32_t, Unit*> units;		//Bedarf ggf. überarbeitung

		std::vector<MapPosition> spawnPositions;

		MapViewerManager viewerManager;

		uint32_t getNumberOfMoveableFields() const; //Anzahl der Felder, auf denen sich Bewegt werden kann (Weggitter hat verbindung)
		uint16_t getHeightDiffOnField(uint32_t position) const;

	public:
		MapImpl(uint16_t width, uint16_t height);
		virtual ~MapImpl();

		/// Berechnet die Bewegungsmöglichkeiten von jedem Feld auf der Map neu
		void updateMovementMap();

		///Berechnet die Bewegungsmöglichkeiten innerhalb der angegebenen Felder auf der Map neu
		void updateMovementMap(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);

		///Berechnet die Bewegungsmöglichkeiten innerhalb der angegebenen Felder auf der Map neu
		void updateMovementMap(uint32_t position1, uint32_t position2);

		void updateMovementMapWithBorder();

		virtual std::vector<MapPosition> getNeighbourPositions(uint16_t x, uint16_t y) const;
		virtual std::vector<MapPosition> getWay(const Unit& unit, uint32_t destination) const;

		virtual bool isFieldFree(uint32_t position) const;

		virtual void setFieldStatusFlag(uint32_t position, uint8_t statusFlag, bool value);
		virtual bool getFieldStatusFlag(uint32_t position, uint8_t statusFlag) const;

		virtual bool addUnit(Unit& unit, uint16_t x, uint16_t y);
		virtual void removeUnit(Unit& unit);

		virtual uint8_t countSpawnPoints() const {return spawnPositions.size();}
		virtual void addSpawnPoint(const MapPosition& position, const Faction* faction = NULL);

		uint8_t getDirections(uint32_t x, uint32_t y) const {return movementMap[position(x, y)];}

		inline uint16_t getRawHeight(uint32_t offset) const {return heightMap[offset];}
		inline uint8_t getRawWay(uint32_t offset) const {return movementMap[offset];}

		const Navigator* getNavigator() const {return navigator;}

		bool loadHeightMapRAW(const std::string& filename);  //Läd eine Heightmap 1:1 aus einer Datei
};

#endif
