#include "Unit/UnitChassis.h"

UnitChassis::UnitChassis(uint32_t id, const std::string& model, const std::string& name, uint32_t turnRate, uint32_t moveRate, uint32_t buildTime, uint32_t hitPoints) :
		id(id),
		model(model),
		name(name),
		turnRate(turnRate),
		moveRate(moveRate),
		buildTime(buildTime),
		hitPoints(hitPoints) {
}
