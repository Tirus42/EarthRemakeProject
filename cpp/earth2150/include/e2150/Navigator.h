#ifndef NAVIGATOR_H
#define NAVIGATOR_H

#include "e2150/Map.h"
#include "e2150/Unit.h"

class Map;
class MapPosition;

class Navigator {
	public:
		virtual ~Navigator(){}

		std::vector<MapPosition> getPath(const Map& map, const Unit& unit, uint16_t x, uint16_t y) const{
			return getPath(map, unit.getX(), unit.getY(), x, y);
		}

		std::vector<MapPosition> getPath(const Map& map, uint32_t start, uint32_t target) const {
			return getPath(map, map.positionX(start), map.positionY(start), map.positionX(target), map.positionY(target));
		}

		virtual std::vector<MapPosition> getPath(const Map& map, uint16_t startX, uint16_t startY, uint16_t x, uint16_t y) const = 0;
};

#endif // NAVIGATOR_H
