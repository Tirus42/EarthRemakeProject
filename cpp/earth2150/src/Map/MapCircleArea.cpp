#include "Map/MapCircleArea.h"

MapCircleArea::MapCircleArea() :
	center(),
	radius(0) {
}

MapCircleArea::MapCircleArea(const MapPosition& center, uint32_t radius) :
	center(center),
	radius(radius) {
}

bool MapCircleArea::isCenterOnMap(const Map& map) const {
	return center.isValidOnMap(map);
}
