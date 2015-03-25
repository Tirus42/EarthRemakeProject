#include "Map/MapManipulator.h"

#include "Map/Map.h"
#include "Map/MapRectArea.h"

#include <cassert>

MapManipulator::MapManipulator(Map& map) :
	map(map) {

}

void MapManipulator::setHeight(const MapRectArea& area, uint16_t height) {
	if (!area.isValidOnMap(map))
		return;

	for (uint16_t y = area.getMinEdge().getY(); y <= area.getMaxEdge().getY(); ++y) {
		for (uint16_t x = area.getMinEdge().getX(); x <= area.getMaxEdge().getX(); ++x) {
			map.heightMap[map.position(x, y)] = height;
		}
	}
}

bool MapManipulator::getMinMaxHeight(const MapRectArea& area, uint16_t& min, uint16_t& max) const {
	if (!area.isValidOnMap(map))
		return false;	// Todo: bessser behandeln? oder ausschließen?

	uint16_t minHeight = 0xFFFF;
	uint16_t maxHeight = 0;

	for (uint16_t y = area.getMinEdge().getY(); y <= area.getMaxEdge().getY(); ++y) {
		for (uint16_t x = area.getMinEdge().getX(); x <= area.getMaxEdge().getX(); ++x) {
			uint16_t height = map.heightMap[map.position(x, y)];

			if (height < minHeight)
				minHeight = height;

			if (height > maxHeight)
				maxHeight = height;
		}
	}

	min = minHeight;
	max = maxHeight;

	return true;
}
