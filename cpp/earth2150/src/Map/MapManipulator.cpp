#include "Map/MapManipulator.h"

#include "Map/Map.h"
#include "Map/MapRectArea.h"

#include <cassert>

MapManipulator::MapManipulator(Map& map) :
	map(map) {

}

void MapManipulator::setHeight(const MapRectArea& area, uint16_t height) const {
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

bool MapManipulator::copyAreaHeight(const MapRectArea& from, const MapPosition& to, const Map* sourceMap) const {
	// Todo: Fall beachten wenn Quelle und Ziel sich überlappen
	if (!sourceMap)
		sourceMap = &map;

	if (!from.isValidOnMap(*sourceMap))
		return false;

	MapRectArea targetArea(to, from.getWidth(), from.getHeight());
	targetArea.makeValidOnMap(map);

	const MapPosition& origin = from.getMinEdge();

	for (uint16_t y = 0; y < targetArea.getHeight(); ++y) {
		for (uint16_t x = 0; x < targetArea.getWidth(); ++x) {
			uint16_t height = sourceMap->heightMap[sourceMap->position(origin.getX() + x, origin.getY() + y)];

			map.heightMap[map.position(to.getX() + x, to.getY() + y)] = height;
		}
	}

	return true;
}
