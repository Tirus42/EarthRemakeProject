#ifndef NAVIGATOR_H
#define NAVIGATOR_H

#include <vector>
#include <stdint.h>

class Map;
class MapPosition;
class Unit;

class Navigator{
	public:
		virtual std::vector<MapPosition> getPath(const Map& map, const Unit& unit, uint32_t x, uint32_t y) const=0;
};

#endif // NAVIGATOR_H
