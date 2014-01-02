#include "Map/MapPositionFloat.h"

#include <math.h>

MapPositionFloat::MapPositionFloat(uint16_t x, uint16_t y) :
	MapPosition(x, y),
	dx(0.5f),
	dy(0.5f) {
}

MapPositionFloat::MapPositionFloat(uint16_t x, uint16_t y, float dx, float dy) :
	MapPosition(x, y),
	dx(dx),
	dy(dy) {
}

MapPositionFloat::MapPositionFloat(float globalX, float globalY) :
	MapPosition(),
	dx(),
	dy() {

	const float fieldX = floorf(globalX);
	const float fieldY = floorf(globalY);

	setXY(fieldX, fieldY);

	dx = globalX - fieldX;
	dy = globalY - fieldY;
}
