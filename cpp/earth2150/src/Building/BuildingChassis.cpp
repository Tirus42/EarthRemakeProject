#include "Building/BuildingChassis.h"

BuildingChassis::BuildingChassis(uint8_t width, uint8_t height, const std::string& name) :
	ground(width, height),
	name(name) {

}

BuildingChassis::~BuildingChassis() {
}
