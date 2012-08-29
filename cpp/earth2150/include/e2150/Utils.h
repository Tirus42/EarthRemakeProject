#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include <vector>
#include <list>
#include <stdlib.h>
#include <stdint.h>

#include "e2150/Map.h"
#include "e2150/MapPosition.h"

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

		/// Gibt den kleineren Winkel zwischen 2 Ausrichtungen zur�ck (Wertebereich 0 - 7)
		static uint8_t getAngleDifference(int8_t a, int8_t b) {
			int8_t var = abs(a-b);
			return (var > 4) ? (8 - var) : var;
		}

		/// Gibt den Winkel zur�ck, in dem das 2te Feld in relation zum ersten steht
		/// (Felder m�ssen benachbart sein!)
		static uint8_t getAngle(const Map& map, uint32_t position1, uint32_t position2) {
			if (position2 == position1 - map.getWidth())
				return Map::DIRECTION_NORTH;
			if (position2 == position1 - map.getWidth() + 1)
				return Map::DIRECTION_NORTH_EAST;
			if (position2 == position1 + 1)
				return Map::DIRECTION_EAST;
			if (position2 == position1 + map.getWidth() + 1)
				return Map::DIRECTION_SOUTH_EAST;
			if (position2 == position1 + map.getWidth())
				return Map::DIRECTION_SOUTH;
			if (position2 == position1 + map.getWidth() - 1)
				return Map::DIRECTION_SOUTH_WEST;
			if (position2 == position1 - 1)
				return Map::DIRECTION_WEST;
			if (position2 == position1 - map.getWidth() -1)
				return Map::DIRECTION_NORTH_WEST;

			return 0xFF;	// Dieser Fall kann/darf nie eintreten!
		}

};


#endif // UTILS_H_INCLUDED
