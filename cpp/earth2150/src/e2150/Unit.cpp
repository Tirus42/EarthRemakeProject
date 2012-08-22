#include "e2150/Unit.h"
#include "e2150/UnitChassis.h"

Unit::Unit(uint32_t id, const UnitChassis& chassis) :
	Entity(id, chassis.getHitPoints()),
	controlableFlags(0),
	chassis(chassis) {

}

Unit::~Unit() {
}


int32_t Unit::dumpData(char* buffer) const {
	int32_t offset = Entity::dumpData(buffer);

	buffer[offset]						= controlableFlags;
	*(uint32_t*)(&buffer[offset + 1])	= chassis.getID();

	return offset + 5;
}
