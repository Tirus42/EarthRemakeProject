#ifndef MAPINTERFACE_H_INCLUDED
#define MAPINTERFACE_H_INCLUDED

#include "e2150/MapPosition.h"
#include "e2150/MapViewerManager.h"
#include <stdint.h>
#include <map>
#include <memory>
#include <string>
#include <vector>

class Unit;
class Faction;
class Navigator;

/**
* Beinhaltet die HeightMap sowie die Wegemap
* und Verwaltet alle Spielobjekte auf der Karte
*/
class Map {
	private:
		/// Breite der Karte
		uint16_t width;

		/// Höhe der Karte
		uint16_t height;

		/// Die Anzahl an Felder am Rand, die nicht nutzbar für die Spieler ist
		uint16_t borderWidth;

		/// Array mit Höheninformationen, wird Zeilenweise gespeichert.
		uint16_t* heightMap;

		/// Karte der Bewegungsmöglichkeiten.
		uint8_t* movementMap;

		/// Beschreibt den Status eines Feldes (eine Einheit steht drauf z.B.).
		uint8_t* statusMap;

		Navigator* navigator;

		std::map<uint32_t, Unit*> units;		//Bedarf ggf. überarbeitung

		std::vector<MapPosition> spawnPositions;

		MapViewerManager viewerManager;

		/// Anzahl der Felder, auf denen sich Bewegt werden kann (Weggitter hat verbindung)
		uint32_t getNumberOfMoveableFields() const;
		uint16_t getHeightDiffOnField(uint32_t position) const;

		Map(const Map& cc);
		Map& operator=(const Map& cc);
	public:
		// Maximale höhendifferenz bei der ein Weg noch als begehbar gilt.
		const static uint16_t MAX_HEIGHTDIFF = 2000;

		static const uint8_t NORTH		= (1 << 0);	 //0b00000001;
		static const uint8_t NORTH_EAST	= (1 << 1);	 //0b00000010;
		static const uint8_t EAST		= (1 << 2);	 //0b00000100;
		static const uint8_t SOUTH_EAST	= (1 << 3);	 //0b00001000;
		static const uint8_t SOUTH		= (1 << 4);	 //0b00010000;
		static const uint8_t SOUTH_WEST	= (1 << 5);	 //0b00100000;
		static const uint8_t WEST		= (1 << 6);	 //0b01000000;
		static const uint8_t NORTH_WEST	= (1 << 7);	 //0b10000000;

		// Auf diesem Feld steht eine Einheit
		static const uint8_t STATUS_UNIT = (1 << 0);
		// Auf diesem Feld liegt eine Mine
		static const uint8_t STATUS_MINE = (1 << 1);

		/// Erstellt eine neue map und allokiert den für die
		/// angegebene Größe notwendigen Speicher.
		Map(uint16_t width, uint16_t height);
		~Map();

		/// Gibt die Breite der Karte zurück
		uint16_t getWidth() const{return width;}

		/// Gibt die Höhe der Karte zurück
		uint16_t getHeight() const{return height;}

		uint16_t getBorderWidth() const{return borderWidth;}

		uint16_t getMinX() const{return borderWidth;}
		uint16_t getMinY() const{return borderWidth;}
		uint16_t getMaxX() const{return width-borderWidth;}
		uint16_t getMaxY() const{return height-borderWidth;}

		inline uint32_t position(uint16_t x, uint16_t y) const {return y * width + x;}
		inline uint16_t positionX(uint32_t position) const {return position % width;}
		inline uint16_t positionY(uint32_t position) const {return position / width;}

		/// Gibt die umliegenden Felder um das angegebene Feld zurück
		std::vector<MapPosition> getNeighbourPositions(uint16_t x, uint16_t y) const;

		/// Berechnet den Weg zwischen zwei Punkten und speichert den Weg
		/// in die Liste. Gibt true zurück, wenn der Weg gefunden wurde.
		bool getWay(uint32_t start_index, uint32_t goal_index,
					std::list<uint32_t>& path_list) const;

		/// Gibt die möglichen Bewegungsrichtungen
		/// des angegebenen Index aus zurück.
		uint8_t getDirections(uint32_t index) const{return movementMap[index];}

		/// Gibt true zurück, falls dieses Feld frei ist.
		/// (Weder ein Gebäude noch eine Einheit darauf)
		bool isFieldFree(uint32_t position) const;

		/// Prüft, ob man von der angegebenen Position in die gegebene Richtung gehen kann.
		/// Macht keine Prüfung, ob die Quellkoordinate gültig ist
		inline bool isFieldWalkable(uint32_t from_index, uint8_t map_direction) const {
			return (getDirections(from_index) & map_direction);
		}

		/// Setzt ein Status-Bit für das angegebene Feld
		void setFieldStatusFlag(uint32_t position, uint8_t statusFlag, bool value);

		/// Gibt das Status-Bit des angegebenen Feldes zurück
		bool getFieldStatusFlag(uint32_t position, uint8_t statusFlag) const;

		/// Fügt eine Einheit in das Spielfeld ein
		bool addUnit(Unit& unit, uint16_t x, uint16_t y);

		/// Entfernt eine Einheit von dem Spielfeld
		void removeUnit(Unit& unit);

		/// Gibt an, wie viele Einheiten z.Z. auf dem Spielfeld sind
		uint32_t getUnitCount() const { return units.size(); }

		/// Gibt die Anzahl der Player-Spawn-Punkte zurück
		uint8_t countSpawnPoints() const { return spawnPositions.size(); }

		/// Fügt einen weiteren Player-Spawn-Punkt hinzu
		void addSpawnPoint(const MapPosition& position, const Faction* faction = 0);

		inline uint16_t getRawHeight(uint32_t offset) const {return heightMap[offset];}
		inline uint8_t getRawWay(uint32_t offset) const {return movementMap[offset];}

		/// Gibt die Liste der Einheiten auf dem Spielfeld zurück
		const std::map<uint32_t, Unit*> getUnits() const {return units;}

		/// Berechnet die Bewegungsmöglichkeiten von jedem Feld auf der Map neu
		void updateMovementMap();

		///Berechnet die Bewegungsmöglichkeiten innerhalb der angegebenen Felder auf der Map neu
		void updateMovementMap(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);

		///Berechnet die Bewegungsmöglichkeiten innerhalb der angegebenen Felder auf der Map neu
		void updateMovementMap(uint32_t position1, uint32_t position2);

		void updateMovementMapWithBorder();

		/// Läd eine Heightmap 1:1 aus einer Datei
		bool loadHeightMapRAW(const std::string& filename);

		const Navigator* getNavigator() { return navigator; }
};

#endif // MAPINTERFACE_H_INCLUDED
