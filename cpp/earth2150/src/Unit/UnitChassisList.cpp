#include "Unit/UnitChassisList.h"

UnitChassisList::UnitChassisList() :
	chassisList() {}

uint32_t UnitChassisList::addChassis(const UnitChassis& chassis) {
	uint32_t index = chassisList.size();

	chassisList.push_back(chassis);
	return index;
}

const UnitChassis& UnitChassisList::getChassis(uint32_t index) const {
	return chassisList.at(index);
}

UnitChassis& UnitChassisList::getChassis(uint32_t index) {
	return chassisList.at(index);
}
