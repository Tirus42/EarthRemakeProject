#ifndef MAPINTERFACE_H_INCLUDED
#define MAPINTERFACE_H_INCLUDED

#include "Map/MapPosition.h"
#include "Map/MapViewerManager.h"
#include "Unit/MovingUnit.h"
#include <stdint.h>
#include <map>
#include <queue>
#include <memory>
#include <string>
#include <vector>

class Player;
class Unit;
class MovingUnit;
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

		///z.B. die A* oder JPS Implementierung
		Navigator* navigator;

		std::map<uint32_t, Unit*> units;		//Bedarf ggf. überarbeitung

		/// Speichert alle sich in Bewegung befindenden Einheiten in der Reihenfolge
		/// ihrer Ankunft
		std::priority_queue<MovingUnit*, std::vector<MovingUnit*>, MovingUnitComparator> movingUnits;

		std::vector<MapPosition> spawnPositions;

		MapViewerManager viewerManager;

		/// Anzahl der Felder, auf denen sich Bewegt werden kann (Weggitter hat verbindung)
		uint32_t getNumberOfMoveableFields() const;

		/// Gibt den Höhenunterschied auf einem Feld zurück (Unterschied zwischen höchster und tiefster Ecke)
		uint16_t getHeightDiffOnField(uint32_t position) const;

		Map(const Map& cc);
		Map& operator=(const Map& cc);

	public:
		/// Maximale Höhendifferenz, bei dem ein Weg noch als begehbar gilt.
		const static uint16_t MAX_HEIGHTDIFF = 2000;

		/// Richtungskonstanten um Bewegungsmöglichkeiten auf einem Feld zu speichern.
		static const uint8_t NORTH		= (1 << 0);	 //0b00000001;
		static const uint8_t NORTH_EAST	= (1 << 1);	 //0b00000010;
		static const uint8_t EAST		= (1 << 2);	 //0b00000100;
		static const uint8_t SOUTH_EAST	= (1 << 3);	 //0b00001000;
		static const uint8_t SOUTH		= (1 << 4);	 //0b00010000;
		static const uint8_t SOUTH_WEST	= (1 << 5);	 //0b00100000;
		static const uint8_t WEST		= (1 << 6);	 //0b01000000;
		static const uint8_t NORTH_WEST	= (1 << 7);	 //0b10000000;

		/// Richtungskonstanten um die Ausrichtung eines Objektes anzugeben (nur 3-Bit nötig)
		static const uint8_t DIRECTION_NORTH		= 0;
		static const uint8_t DIRECTION_NORTH_EAST 	= 1;
		static const uint8_t DIRECTION_EAST			= 2;
		static const uint8_t DIRECTION_SOUTH_EAST	= 3;
		static const uint8_t DIRECTION_SOUTH		= 4;
		static const uint8_t DIRECTION_SOUTH_WEST	= 5;
		static const uint8_t DIRECTION_WEST			= 6;
		static const uint8_t DIRECTION_NORTH_WEST	= 7;

		uint32_t addNorth(uint32_t position) const{return position-width;}
		uint32_t addNorthEast(uint32_t position) const{return position-width+1;}
		static uint32_t addEast(uint32_t position){return position+1;}
		uint32_t addSouthEast(uint32_t position) const{return position+width+1;}
		uint32_t addSouth(uint32_t position) const{return position+width;}
		uint32_t addSouthWest(uint32_t position) const{return position+width-1;}
		static uint32_t addWest(uint32_t position){return position-1;}
		uint32_t addNorthWest(uint32_t position) const{return position-width-1;}

		// Auf diesem Feld steht eine Einheit
		static const uint8_t STATUS_UNIT = (1 << 0);
		// Auf diesem Feld liegt eine Mine
		static const uint8_t STATUS_MINE = (1 << 1);

		/// Erstellt eine neue map und allokiert den für die
		/// angegebene Größe notwendigen Speicher.
		Map(uint16_t width, uint16_t height);
		virtual ~Map();

		/// Gibt die Breite der Karte zurück
		uint16_t getWidth() const {return width;}

		/// Gibt die Höhe der Karte zurück
		uint16_t getHeight() const {return height;}

		/// Gibt die breite des Spielfeldrandes zurück
		uint16_t getBorderWidth() const {return borderWidth;}

		/// Gibt die kleinste X Position zurück, worauf Spielobjekte plaziert sein können
		uint16_t getMinX() const {return borderWidth;}

		/// Gibt die kleinste Y Position zurück, worauf Spielibjekte plaziert sein können
		uint16_t getMinY() const {return borderWidth;}

		/// Gibt die größte X Position zurück, worauf Spielobjekte plaziert sein können
		uint16_t getMaxX() const {return width-borderWidth;}

		/// Gibt die größe Y Position zurück, worauf Spielobjekte plaziert sein können
		uint16_t getMaxY() const {return height-borderWidth;}

		/// Gibt die nutzbare Breite der Spielfläche zurück
		uint16_t getInnerWidth() const {return width-2*borderWidth;}

		/// Gibt die nutzbare Höhe der Spielfläche zurück
		uint16_t getInnerHeight() const {return height-2*borderWidth;}

		uint32_t position(uint16_t x, uint16_t y) const {return y * width + x;}
		uint16_t positionX(uint32_t position) const {return position % width;}
		uint16_t positionY(uint32_t position) const {return position / width;}

		/// Gibt die anzahl der begehbaren Nachbarn zurück und speichert diese in neighbours
		size_t getNeighbours(uint32_t position, uint32_t *neighbours) const;

		/// Richtung zum Ziel, dabei wird N/E/S/W eher genommen als NE/SE/SW/NW, falls es nicht eindeutig ist (NNE -> N)
		uint8_t getDirection(uint32_t start_index, uint32_t goal_index) const;

		/// Berechnet den Weg zwischen zwei Punkten und speichert den Weg
		/// in die Liste. Gibt true zurück, wenn der Weg gefunden wurde.
		bool getWay(uint32_t start_index, uint32_t goal_index,
					std::list<uint32_t>& path_list) const;

		/// Gibt die möglichen Bewegungsrichtungen
		/// des angegebenen Index aus zurück.
		uint8_t getDirections(uint32_t index) const{return movementMap[index];}

		/// Gibt die Höhe an einer Position auf dem Feld zurück (x, y in [0, 1])
		uint16_t getFieldHeight(uint32_t position, float x, float y) const;

		/// Gibt true zurück, falls dieses Feld frei ist.
		/// (Weder ein Gebäude noch eine Einheit darauf)
		bool isFieldFree(uint32_t position) const;

		/// Prüft, ob man von der angegebenen Position in die gegebene Richtung gehen kann.
		/// Macht keine Prüfung, ob die Quellkoordinate gültig ist
		bool isFieldWalkable(uint32_t from_index, uint8_t map_direction) const {
			return getDirections(from_index) & map_direction;
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

		Unit* getUnit(uint32_t unitID) {return units[unitID];}

		/// Lässt eine Einheit zur angegebenen Position fahren (falls ein Weg gefunden wird)
		/// (Diese Einheit muss in der Liste der Einheiten stehen!)
		void UnitDriveTo(Unit& unit, uint32_t target);

		/// Gibt die Anzahl der Player-Spawn-Punkte zurück
		uint8_t countSpawnPoints() const { return spawnPositions.size(); }

		/// Fügt einen weiteren Player-Spawn-Punkt hinzu
		void addSpawnPoint(const MapPosition& position, const Faction* faction = 0);

		uint16_t getRawHeight(uint32_t offset) const {return heightMap[offset];}
		uint8_t getRawWay(uint32_t offset) const {return movementMap[offset];}

		/// Gibt die Liste der Einheiten auf dem Spielfeld zurück
		const std::map<uint32_t, Unit*> getUnits() const {return units;}

		/// Berechnet die Bewegungsmöglichkeiten von jedem Feld auf der Map neu
		void updateMovementMap();

		/// Berechnet die Bewegungsmöglichkeiten innerhalb der angegebenen Felder (x, y) neu
		/// (x1 <= x < x2, y1 <= y < y2)
		void updateMovementMap(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);

		///Berechnet die Bewegungsmöglichkeiten innerhalb der angegebenen Felder auf der Map neu
		void updateMovementMap(uint32_t position1, uint32_t position2);

		/// Läd eine Heightmap 1:1 aus einer Datei
		bool loadHeightMapRAW(const std::string& filename);

		/// Fügt einen weiteren Spieler in einen bestimmten Slot (Spawnpoint) hinzu
		bool addPlayer(Player& player, uint32_t slot);

		/// Entfernt einen Spieler und entfernt ggf. alle ihm gehörende Spielobjekte
		void removePlayer(Player& player, bool removeEntitys = true);

		const Navigator* getNavigator() { return navigator; }

		/// Berechnet alle Veränderungen die in der zwischenzeit geschehen sind
		void updateGameField(uint32_t currentTime);

		/// Prüft ob ein Feld wirklich auf der Karte ist (und nicht außerhalb)
		/// (Für Asserts gedacht)
		bool fieldOnMap(const MapPosition& position) const {return fieldOnMap(position.getX(), position.getY());}
		bool fieldOnMap(uint16_t x, uint16_t y) const {return (x < width) && (y < height);}
		bool fieldOnMap(uint32_t position) const {return (position < uint32_t(width * height));}
};

#endif // MAPINTERFACE_H_INCLUDED
