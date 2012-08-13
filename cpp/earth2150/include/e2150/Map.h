#ifndef MAP_H
#define MAP_H

#include <list>
#include <stdint.h>
#include <vector>
#include <string>

#include "tf/file.h"

#define MAX_HEIGHTDIFF 2000

/**
* Map Klasse
* Beinhaltet die HeightMap sowie die Wegemap
* und Verwaltet alle Spielobjekte auf der Karte
*/
class Map {
	private:
		static const uint8_t NORTH		= (1 << 0);	 //0b00000001;
		static const uint8_t NORTH_EAST	= (1 << 1);	 //0b00000010;
		static const uint8_t EAST		= (1 << 2);	 //0b00000100;
		static const uint8_t SOUTH_EAST	= (1 << 3);	 //0b00001000;
		static const uint8_t SOUTH		= (1 << 4);	 //0b00010000;
		static const uint8_t SOUTH_WEST	= (1 << 5);	 //0b00100000;
		static const uint8_t WEST		= (1 << 6);	 //0b01000000;
		static const uint8_t NORTH_WEST	= (1 << 7);	 //0b10000000;


		Map(const Map&);
		void operator=(const Map&);

		uint16_t width, height;	 //Breite und Höhe der Karte

		std::vector<uint16_t> heightMap;
		std::vector<uint8_t> movementMap;  //Karte der Bewegungsmöglichkeiten

		int32_t borderWidth; //Die Anzahl an Felder an Rand, die nicht nutzbar für die Spieler ist

		void updateMovementMap();   //Berechnet die gesammte Karte neu
		void updateMovementMap(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
		void updateMovementMap(uint32_t position1, uint32_t position2);

		uint32_t getNumberOfMoveableFields() const; //Anzahl der Felder, auf denen sich Bewegt werden kann (Weggitter hat verbindung)

		uint16_t getHeightDiffOnField(uint32_t position) const;

		inline uint32_t position(uint16_t x, uint16_t y) const {return y * width + x;}
		inline uint16_t positionX(uint32_t position) const {return position % width;}
		inline uint16_t positionY(uint32_t position) const {return position / width;}

	public:
		Map(uint16_t width, uint16_t height);
		virtual ~Map();

		uint16_t getWidth() const {return width;}
		uint16_t getHeight() const {return height;}

		inline uint16_t getRawHeight(uint32_t offset) const {return heightMap[offset];}

		std::list<uint32_t> getWay(uint32_t source, uint32_t destination);

		bool loadHeightMapRAW(const std::string& filename);  //Läd eine Heightmap 1:1 aus einer Datei
};

#endif
