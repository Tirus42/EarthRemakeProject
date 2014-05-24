#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include <cassert>
#include <list>
#include <stdlib.h>
#include <stdint.h>
#include <vector>

#include "Map/Map.h"
#include "Map/MapPosition.h"

class Utils {
	public:
		template<typename T>
		static std::list<T> vectorToList(const std::vector<T>& v) {
			std::list<T> list(v.begin(), v.end());

			return list;
		}

		static std::list<uint32_t> mapPositionToPosition(const Map& map, std::list<MapPosition>& list) {
			std::list<uint32_t> outputList;

			for (std::list<MapPosition>::iterator i = list.begin(); i != list.end(); ++i) {
				 outputList.push_back(map.position(i->getX(), i->getY()));
			}
			return outputList;
		}

		/// Gibt den kleineren Winkel zwischen 2 Ausrichtungen zurück (Wertebereich 0 - 7)
		static uint8_t getAngleDifference(uint8_t a, uint8_t b) {
			uint8_t var = a < b ? b - a : a - b;
			return ((var & 4) ? -var : var) & 7;
		}

		/// Gibt den Winkel zurück, in dem das 2te Feld in relation zum ersten steht
		/// (Felder müssen benachbart sein!)
		static Map::ObjectDirection getAngle(const Map& map, uint32_t position1, uint32_t position2) {
			if (position2 == map.addNorth(position1))
				return Map::DIRECTION_NORTH;
			if (position2 == map.addNorthEast(position1))
				return Map::DIRECTION_NORTH_EAST;
			if (position2 == map.addEast(position1))
				return Map::DIRECTION_EAST;
			if (position2 == map.addSouthEast(position1))
				return Map::DIRECTION_SOUTH_EAST;
			if (position2 == map.addSouth(position1))
				return Map::DIRECTION_SOUTH;
			if (position2 == map.addSouthWest(position1))
				return Map::DIRECTION_SOUTH_WEST;
			if (position2 == map.addWest(position1))
				return Map::DIRECTION_WEST;
			if (position2 == map.addNorthWest(position1))
				return Map::DIRECTION_NORTH_WEST;
			assert(false);
			return Map::DIRECTION_INVALID;	// Dieser Fall kann/darf nie eintreten!
		}

};


#endif // UTILS_H_INCLUDED
