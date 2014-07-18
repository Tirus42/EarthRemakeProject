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

uint16_t MapManipulator::getMinHeight(const MapRectArea& area) const {
	if (!area.isValidOnMap(map))
		return 0;	// Todo: bessser behandeln? oder ausschließen?

	uint16_t minHeight = 0xFFFF;

	for (uint16_t y = area.getMinEdge().getY(); y <= area.getMaxEdge().getY(); ++y) {
		for (uint16_t x = area.getMinEdge().getX(); x <= area.getMaxEdge().getX(); ++x) {
			uint16_t height = map.heightMap[map.position(x, y)];

			if (height < minHeight)
				minHeight = height;
		}
	}

	return minHeight;
}

uint16_t MapManipulator::getMaxHeight(const MapRectArea& area) const {
	if (!area.isValidOnMap(map))
		return 0;	// Todo: bessser behandeln? oder ausschließen?

	uint16_t maxHeight = 0;

	for (uint16_t y = area.getMinEdge().getY(); y <= area.getMaxEdge().getY(); ++y) {
		for (uint16_t x = area.getMinEdge().getX(); x <= area.getMaxEdge().getX(); ++x) {
			uint16_t height = map.heightMap[map.position(x, y)];

			if (height > maxHeight)
				maxHeight = height;
		}
	}

	return maxHeight;
}
