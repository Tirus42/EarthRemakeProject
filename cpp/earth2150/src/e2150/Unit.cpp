#include "e2150/Unit.h"
#include "e2150/UnitChassis.h"

Unit::Unit():
		controlableFlags(0) {
}

Unit::Unit(uint32_t id, const UnitChassis& chassis) :
	Entity(id, chassis.getHitPoints()),
	controlableFlags(0) {

}

Unit::~Unit() {
}
