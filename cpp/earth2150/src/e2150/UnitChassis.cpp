#include "e2150/UnitChassis.h"

UnitChassis::UnitChassis(uint32_t uID, std::string name, uint32_t turnRate, uint32_t moveRate) :
	uID(uID),
	name(name),
	turnRate(0),
	moveRate(0) {

	setTurnRate(turnRate);
	setMoveRate(moveRate);


}

