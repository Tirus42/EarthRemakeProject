#include "Building/BuildingGround.h"

#include "Building/BuildingDirection.h"

#include <cassert>

BuildingGround::BuildingGround(uint8_t width, uint8_t height, GroundType startType) :
	width(width),
	height(height),
	field(new uint8_t[width * height]) {

		memset(field, startType, width * height);
}

BuildingGround::~BuildingGround() {
	delete[] field;
}

size_t BuildingGround::getIndex(uint8_t x, uint8_t y) const {
	assert(x < width);
	assert(y < height);

	return (size_t)y * (size_t)width + x;
}

BuildingGround::GroundType BuildingGround::getField(uint8_t x, uint8_t y) const {
	return (GroundType)field[getIndex(x, y)];
}

BuildingGround::GroundType BuildingGround::getField(uint8_t x, uint8_t y, const BuildingDirection& buildingDirection) const {
	buildingDirection.convertPosition(x, y, width, height);

	return getField(x, y);
}

void BuildingGround::setField(uint8_t x, uint8_t y, GroundType type) {
	field[getIndex(x, y)] = type;
}
