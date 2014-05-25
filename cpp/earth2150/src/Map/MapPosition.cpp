#include "Map/MapPosition.h"

#include "Map/Map.h"

#include <algorithm>
#include <cassert>

MapPosition::MapPosition() :
	 x(~0),
	 y(~0) {
}

MapPosition::MapPosition(uint16_t x, uint16_t y) :
	x(x),
	y(y) {
}

bool MapPosition::isValid() const {
	// Gibt False zurück, wenn X oder Y == 0xFFFF sind
	return (x != (uint16_t)(~0) && y != (uint16_t)(~0));
}

bool MapPosition::isValidOnMap(const Map& map) const {
	return isValid() && (x < map.getWidth()) && (y < map.getHeight());
}

bool MapPosition::isValidOnUsableMapArea(const Map& map) const {
	if (!isValidOnMap(map))
		return false;

	if (x <= map.getBorderWidth() || x > map.getWidth() - map.getBorderWidth())
		return false;

	if (y <= map.getBorderWidth() || y > map.getHeight() - map.getBorderWidth())
		return false;

	return true;
}

void MapPosition::makeValidOnMap(const Map& map) {
	x = std::min(x, uint16_t(map.getWidth() - 1));
	y = std::min(y, uint16_t(map.getHeight() - 1));

	assert(isValid());
}

uint16_t MapPosition::getDistToMapBorderX(const Map& map) const {
	assert(isValidOnMap(map));

	return (x < map.getWidth() / 2 ? x : map.getWidth() - x);
}

uint16_t MapPosition::getDistToMapBorderY(const Map& map) const {
	assert(isValidOnMap(map));

	return (y < map.getHeight() / 2 ? y : map.getHeight() - y);
}

uint16_t MapPosition::getDistToUseableMapBorderX(const Map& map) const {
	assert(isValidOnUsableMapArea(map));

	return getDistToMapBorderX(map) - map.getBorderWidth();
}

uint16_t MapPosition::getDistToUseableMapBorderY(const Map& map) const {
	assert(isValidOnUsableMapArea(map));

	return getDistToMapBorderY(map) - map.getBorderWidth();
}
