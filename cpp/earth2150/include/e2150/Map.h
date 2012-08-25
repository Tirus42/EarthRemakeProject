#ifndef MAPINTERFACE_H_INCLUDED
#define MAPINTERFACE_H_INCLUDED

#include <stdint.h>
#include <vector>

class MapPosition;
class Unit;
class Faction;

/**
* (Abstrakte) Map-Oberklasse
* Stellt alle (abstrakte-) Methoden bereit, welche eine Map implementieren muss
*/
class Map {
	protected:
		uint16_t width, height;	 //Breite und Höhe der Karte

	public:
		static const uint8_t NORTH		= (1 << 0);	 //0b00000001;
		static const uint8_t NORTH_EAST	= (1 << 1);	 //0b00000010;
		static const uint8_t EAST		= (1 << 2);	 //0b00000100;
		static const uint8_t SOUTH_EAST	= (1 << 3);	 //0b00001000;
		static const uint8_t SOUTH		= (1 << 4);	 //0b00010000;
		static const uint8_t SOUTH_WEST	= (1 << 5);	 //0b00100000;
		static const uint8_t WEST		= (1 << 6);	 //0b01000000;
		static const uint8_t NORTH_WEST	= (1 << 7);	 //0b10000000;

		static const uint8_t STATUS_UNIT = (1 << 0);	//Auf diesem Feld steht eine Einheit
		static const uint8_t STATUS_MINE = (1 << 1);	//Auf diesem Feld liegt eine Mine


		Map(uint16_t width, uint16_t height) : width(width), height(height) {};
		virtual ~Map() {};

		/// Gibt die Breite der Karte zurück
		uint16_t getWidth() const {return width;}

		/// Gibt die Höhe der Karte zurück
		uint16_t getHeight() const {return height;}

		inline uint32_t position(uint16_t x, uint16_t y) const {return y * width + x;}
		inline uint16_t positionX(uint32_t position) const {return position % width;}
		inline uint16_t positionY(uint32_t position) const {return position / width;}

		/// Gibt die umliegenden Felder um das angegebene Feld zurück
		virtual std::vector<MapPosition> getNeighbourPositions(uint16_t x, uint16_t y) const = 0;

		/// Berechnet den Weg zwischen der Einheit und der Angegebenen Position
		virtual std::vector<MapPosition> getWay(const Unit& unit, uint32_t destination) const = 0;

    /// Gibt die möglichen Bewegungsrichtungen der angegebenen Position aus an
		virtual uint8_t getDirections(uint16_t x, uint16_t y) const = 0;

		/// Prüft ob dieses Feld frei ist (Weder ein Gebäude noch eine Einheit darauf)
		virtual bool isFieldFree(uint32_t position) const = 0;
		
		/// Prüft, ob mal von der angegebenen Position in die gegebene Richtung gehen kann.
		/// Macht keine Pruefung, ob die Quellkoordinate gültig ist
		inline bool isFieldWalkable(uint16_t from_x, uint16_t from_y, uint8_t map_direction) const {
      return (getDirections(from_x, from_y) & map_direction);
		}

		/// Setzt ein Status-Bit für das angegebene Feld
		virtual void setFieldStatusFlag(uint32_t position, uint8_t statusFlag, bool value) = 0;

		/// Gibt das Status-Bit des angegebenen Feldes zurück
		virtual bool getFieldStatusFlag(uint32_t position, uint8_t statusFlag) const = 0;

		/// Fügt eine Einheit in das Spielfeld ein
		virtual bool addUnit(Unit& unit, uint16_t x, uint16_t y) = 0;

		/// Entfernt eine Einheit von dem Spielfeld
		virtual void removeUnit(Unit& unit) = 0;

		/// Gibt an, wie viele Einheiten z.Z. auf dem Spielfeld sind
		virtual uint32_t getUnitCount() const = 0;

		/// Gibt die Anzahl der Player-Spawn-Punkte zurück
		virtual uint8_t countSpawnPoints() const = 0;

		/// Fügt einen weiteren Player-Spawn-Punkt hinzu
		virtual void addSpawnPoint(const MapPosition& position, const Faction* faction = 0) = 0;
};

#endif // MAPINTERFACE_H_INCLUDED
