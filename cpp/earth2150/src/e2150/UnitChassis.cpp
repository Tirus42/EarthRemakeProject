#include "e2150/UnitChassis.h"

UnitChassis::UnitChassis(const std::string& id, const std::string& name, uint32_t turnRate, uint32_t moveRate) :
		id(id),
		name(name),
		turnRate(turnRate),
		moveRate(moveRate) {
}
