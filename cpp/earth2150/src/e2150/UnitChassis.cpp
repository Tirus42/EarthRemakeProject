#include "e2150/UnitChassis.h"

UnitChassis::UnitChassis(uint32_t id, const std::string& model, const std::string& name, uint32_t turnRate, uint32_t moveRate) :
		id(id),
		model(model),
		name(name),
		turnRate(turnRate),
		moveRate(moveRate),
		buildTime(0) {
}
