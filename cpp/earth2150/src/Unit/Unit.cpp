#include "Unit/Unit.h"
#include "Unit/UnitChassis.h"
#include "Map/Map.h"

Unit::Unit(uint32_t id, const UnitChassis& chassis) :
	Entity(id, chassis.getHitPoints()),
	statusFlags(0),
	chassis(chassis),
	wayPoints() {

}

Unit::~Unit() {
}

bool Unit::isControlable() const {
	//Wird noch erweitert
	return !(statusFlags & FLAG_INBUILDING);
}

void Unit::setStatusFlag(uint8_t statusFlag, bool value) {
	if (value) {
		statusFlags |= statusFlag;
	} else {
		statusFlags &= ~statusFlag;
	}
}

bool Unit::getStatusFlag(uint8_t statusFlag) const {
	return statusFlags & statusFlag;
}

void Unit::move(uint8_t direction) {
	uint16_t x = getPosition().getX();
	uint16_t y = getPosition().getY();

	switch(direction) {
		case Map::DIRECTION_NORTH:
			setPosition(x, y - 1);
			break;
		case Map::DIRECTION_NORTH_EAST:
			setPosition(x + 1, y - 1);
			break;
		case Map::DIRECTION_EAST:
			setPosition(x + 1, y);
			break;
		case Map::DIRECTION_SOUTH_EAST:
			setPosition(x + 1, y + 1);
			break;
		case Map::DIRECTION_SOUTH:
			setPosition(x, y + 1);
			break;
		case Map::DIRECTION_SOUTH_WEST:
			setPosition(x - 1, y + 1);
			break;
		case Map::DIRECTION_WEST:
			setPosition(x - 1, y);
			break;
		case Map::DIRECTION_NORTH_WEST:
			setPosition(x - 1, y - 1);
			break;
		default:
			return;	// Dieser Fall kann/darf nie eintreten
	}
}

int32_t Unit::dumpData(char* buffer) const {
	int32_t offset = Entity::dumpData(buffer);

	buffer[offset]						= statusFlags;
	*(uint32_t*)(&buffer[offset + 1])	= chassis.getID();

	return offset + 5;
}

uint32_t Unit::dumpDataSize() const {
	return Entity::dumpDataSize() + 5;
}
