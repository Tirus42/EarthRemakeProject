#ifndef MAPINTERFACE_H_INCLUDED
#define MAPINTERFACE_H_INCLUDED

#include "Map/MapPosition.h"
#include "Map/MapViewerManager.h"
#include "Unit/MovingUnit.h"

#include <map>
#include <queue>
#include <stdint.h>
#include <string>
#include <vector>

class Player;
class Unit;
class MovingUnit;
class Faction;
class INavigator;

/**
* Beinhaltet die HeightMap sowie die Wegemap
* und verwaltet alle Spielobjekte auf der Karte
*/
class Map {
	friend class MapManipulator;
	private:
		/// Breite der Karte
		uint16_t width;

		/// H�he der Karte
		uint16_t height;

		/// Die Anzahl an Feldern am Rand, die nicht nutzbar f�r die Spieler sind
		uint16_t borderWidth;

		/// Array mit H�heninformationen, wird zeilenweise gespeichert.
		uint16_t* heightMap;

		/// Karte der Bewegungsm�glichkeiten.
		uint8_t* movementMap;

		/// Beschreibt den Status eines Feldes (eine Einheit steht drauf z.B.).
		uint8_t* statusMap;

		///z.B. die A* oder JPS Implementierung
		INavigator* navigator;

		std::map<uint32_t, Unit*> units;		//Bedarf ggf. �berarbeitung

		/// Speichert alle sich in Bewegung befindenden Einheiten in der Reihenfolge
		/// ihrer Ankunft
		std::priority_queue<MovingUnit*, std::vector<MovingUnit*>, MovingUnitComparator> movingUnits;

		std::vector<MapPosition> spawnPositions;

		MapViewerManager viewerManager;

		/// Anzahl der Felder, auf denen sich bewegt werden kann (Weggitter hat Verbindung)
		uint32_t getNumberOfMoveableFields() const;

		/// Gibt den H�henunterschied auf einem Feld zur�ck (Unterschied zwischen h�chster und tiefster Ecke)
		uint16_t getHeightDiffOnField(uint32_t position) const;

		Map(const Map& cc);
		Map& operator=(const Map& cc);

	public:
		/// Maximale H�hendifferenz, bei dem ein Weg noch als begehbar gilt.
		const static uint16_t MAX_HEIGHTDIFF = 2000;

		/// Richtungskonstanten um Bewegungsm�glichkeiten auf einem Feld zu speichern.
		enum WaymapDirection {
			NORTH      = (1 << 0), //0b00000001;
			NORTH_EAST = (1 << 1), //0b00000010;
			EAST       = (1 << 2), //0b00000100;
			SOUTH_EAST = (1 << 3), //0b00001000;
			SOUTH      = (1 << 4), //0b00010000;
			SOUTH_WEST = (1 << 5), //0b00100000;
			WEST       = (1 << 6), //0b01000000;
			NORTH_WEST = (1 << 7), //0b10000000;
		};

		/// Richtungskonstanten um die Ausrichtung eines Objektes anzugeben (nur 3-Bit n�tig)
		enum ObjectDirection {
			DIRECTION_NORTH      = 0,
			DIRECTION_NORTH_EAST = 1,
			DIRECTION_EAST       = 2,
			DIRECTION_SOUTH_EAST = 3,
			DIRECTION_SOUTH      = 4,
			DIRECTION_SOUTH_WEST = 5,
			DIRECTION_WEST       = 6,
			DIRECTION_NORTH_WEST = 7,
			DIRECTION_INVALID
		};

		uint32_t addNorth(uint32_t position) const{return position-width;}
		uint32_t addNorthEast(uint32_t position) const{return position-width+1;}
		static uint32_t addEast(uint32_t position){return position+1;}
		uint32_t addSouthEast(uint32_t position) const{return position+width+1;}
		uint32_t addSouth(uint32_t position) const{return position+width;}
		uint32_t addSouthWest(uint32_t position) const{return position+width-1;}
		static uint32_t addWest(uint32_t position){return position-1;}
		uint32_t addNorthWest(uint32_t position) const{return position-width-1;}

		/// Verschiebt die Position in die angegebene Richtung (keine Randbereichspr�fung!)
		uint32_t addDirection(uint32_t position, const WaymapDirection& direction) const;
		uint32_t addDirection(uint32_t position, const ObjectDirection& direction) const;

		// Auf diesem Feld steht eine Einheit
		static const uint8_t STATUS_UNIT = (1 << 0);
		// Auf diesem Feld liegt eine Mine
		static const uint8_t STATUS_MINE = (1 << 1);

		/// Erstellt eine neue Map und allokiert den f�r die
		/// angegebene Gr��e notwendigen Speicher.
		Map(uint16_t width, uint16_t height);
		virtual ~Map();

		/// Gibt die Breite der Karte zur�ck
		uint16_t getWidth() const {return width;}

		/// Gibt die H�he der Karte zur�ck
		uint16_t getHeight() const {return height;}

		/// Gibt die Breite des Spielfeldrandes zur�ck
		uint16_t getBorderWidth() const {return borderWidth;}

		/// Gibt die kleinste X Position zur�ck, worauf Spielobjekte platziert sein k�nnen
		uint16_t getMinX() const {return borderWidth;}

		/// Gibt die kleinste Y Position zur�ck, worauf Spielobjekte platziert sein k�nnen
		uint16_t getMinY() const {return borderWidth;}

		/// Gibt die gr��te X Position zur�ck, worauf Spielobjekte platziert sein k�nnen
		uint16_t getMaxX() const {return width-borderWidth;}

		/// Gibt die gr��te Y Position zur�ck, worauf Spielobjekte platziert sein k�nnen
		uint16_t getMaxY() const {return height-borderWidth;}

		/// Gibt die nutzbare Breite der Spielfl�che zur�ck
		uint16_t getInnerWidth() const {return width-2*borderWidth;}

		/// Gibt die nutzbare H�he der Spielfl�che zur�ck
		uint16_t getInnerHeight() const {return height-2*borderWidth;}

		uint32_t position(uint16_t x, uint16_t y) const {return y * width + x;}
		uint16_t positionX(uint32_t position) const {return position % width;}
		uint16_t positionY(uint32_t position) const {return position / width;}

		/// Gibt die Anzahl der begehbaren Nachbarn zur�ck und speichert diese in neighbours
		size_t getNeighbours(uint32_t position, uint32_t *neighbours) const;

		/// Richtung zum Ziel, dabei wird N/E/S/W eher genommen als NE/SE/SW/NW, falls es nicht eindeutig ist (NNE -> N)
		uint8_t getDirection(uint32_t start_index, uint32_t goal_index) const;

		/// Berechnet den Weg zwischen zwei Punkten und speichert den Weg
		/// in die Liste. Gibt true zur�ck, wenn der Weg gefunden wurde.
		bool getWay(uint32_t start_index, uint32_t goal_index,
					std::list<uint32_t>& path_list) const;

		/// Gibt die m�glichen Bewegungsrichtungen
		/// des angegebenen Index aus zur�ck.
		uint8_t getDirections(uint32_t index) const{return movementMap[index];}

		/// Gibt die H�he an einer Position auf dem Feld zur�ck (x, y in [0, 1])
		uint16_t getFieldHeight(uint32_t position, float x, float y) const;

		/// Gibt true zur�ck, falls dieses Feld frei ist.
		/// (Weder ein Geb�ude noch eine Einheit darauf)
		bool isFieldFree(uint32_t position) const;

		/// Pr�ft, ob man von der angegebenen Position in die gegebene Richtung gehen kann.
		/// Macht keine Pr�fung, ob die Quellkoordinate g�ltig ist
		bool isFieldWalkable(uint32_t from_index, uint8_t map_direction) const {
			return getDirections(from_index) & map_direction;
		}

		/// Setzt ein Status-Bit f�r das angegebene Feld
		void setFieldStatusFlag(uint32_t position, uint8_t statusFlag, bool value);

		/// Gibt das Status-Bit des angegebenen Feldes zur�ck
		bool getFieldStatusFlag(uint32_t position, uint8_t statusFlag) const;

		/// F�gt eine Einheit in das Spielfeld ein
		bool addUnit(Unit& unit, uint16_t x, uint16_t y);

		/// Entfernt eine Einheit von dem Spielfeld
		void removeUnit(Unit& unit);

		/// Gibt an, wie viele Einheiten z.Z. auf dem Spielfeld sind
		uint32_t getUnitCount() const { return units.size(); }

		Unit* getUnit(uint32_t unitID) {return units[unitID];}

		/// L�sst eine Einheit zur angegebenen Position fahren (falls ein Weg gefunden wird)
		/// (Diese Einheit muss in der Liste der Einheiten stehen!)
		void UnitDriveTo(Unit& unit, uint32_t target);

		/// Gibt die Anzahl der Player-Spawn-Punkte zur�ck
		uint8_t countSpawnPoints() const { return spawnPositions.size(); }

		/// F�gt einen weiteren Player-Spawn-Punkt hinzu
		void addSpawnPoint(const MapPosition& position, const Faction* faction = 0);

		uint16_t getRawHeight(uint32_t offset) const {return heightMap[offset];}
		uint8_t getRawWay(uint32_t offset) const {return movementMap[offset];}

		/// Gibt die Liste der Einheiten auf dem Spielfeld zur�ck
		const std::map<uint32_t, Unit*> getUnits() const {return units;}

		/// Berechnet die Bewegungsm�glichkeiten von jedem Feld auf der Map neu
		void updateMovementMap();

		/// Berechnet die Bewegungsm�glichkeiten innerhalb der angegebenen Felder (x, y) neu
		/// (x1 <= x < x2, y1 <= y < y2)
		void updateMovementMap(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);

		///Berechnet die Bewegungsm�glichkeiten innerhalb der angegebenen Felder auf der Map neu
		void updateMovementMap(uint32_t position1, uint32_t position2);

		/// L�dt eine Heightmap 1:1 aus einer Datei
		bool loadHeightMapRAW(const std::string& filename);

		/// Speichert eine komplette Karte der begehbaren Felder als S/W Bitmap
		/// Unpassable Stellen werden schwarz dargestellt
		void exportPassablesToBMP(const std::string& fileName) const;

		/// F�gt einen weiteren Spieler in einen bestimmten Slot (Spawnpoint) hinzu
		bool addPlayer(Player& player, uint32_t slot);

		/// Entfernt einen Spieler und entfernt ggf. alle ihm geh�renden Spielobjekte
		void removePlayer(Player& player, bool removeEntitys = true);

		const INavigator* getNavigator() { return navigator; }

		/// Berechnet alle Ver�nderungen die in der Zwischenzeit geschehen sind
		void updateGameField(uint32_t currentTime);

		/// Pr�ft, ob ein Feld wirklich auf der Karte ist (und nicht au�erhalb)
		/// (F�r Asserts gedacht)
		bool fieldOnMap(const MapPosition& position) const {return fieldOnMap(position.getX(), position.getY());}
		bool fieldOnMap(uint16_t x, uint16_t y) const {return (x < width) && (y < height);}
		bool fieldOnMap(uint32_t position) const {return (position < uint32_t(width * height));}
};

#endif // MAPINTERFACE_H_INCLUDED
