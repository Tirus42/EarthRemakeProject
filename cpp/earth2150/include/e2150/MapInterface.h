#ifndef MAPINTERFACE_H_INCLUDED
#define MAPINTERFACE_H_INCLUDED

#include <stdint.h>
#include <vector>

class MapPosition;
class Unit;

class MapInterface {
	protected:
		static const uint8_t NORTH		= (1 << 0);	 //0b00000001;
		static const uint8_t NORTH_EAST	= (1 << 1);	 //0b00000010;
		static const uint8_t EAST		= (1 << 2);	 //0b00000100;
		static const uint8_t SOUTH_EAST	= (1 << 3);	 //0b00001000;
		static const uint8_t SOUTH		= (1 << 4);	 //0b00010000;
		static const uint8_t SOUTH_WEST	= (1 << 5);	 //0b00100000;
		static const uint8_t WEST		= (1 << 6);	 //0b01000000;
		static const uint8_t NORTH_WEST	= (1 << 7);	 //0b10000000;

		uint16_t width, height;	 //Breite und Höhe der Karte

	public:
		uint16_t getWidth() const {return width;}
		uint16_t getHeight() const {return height;}

		MapInterface(uint16_t width, uint16_t height) : width(width), height(height) {};
		virtual ~MapInterface() {};

		/// Gibt die umliegenden Felder um das angegebene Feld zurück
		virtual std::vector<MapPosition> getNeighbourPositions(uint32_t x, uint32_t y) const = 0;

		/// Berechnet den Weg zwischen der Einheit und der Angegebenen Position
		virtual std::vector<MapPosition> getWay(const Unit& unit, uint32_t destination) const = 0;
};

#endif // MAPINTERFACE_H_INCLUDED
