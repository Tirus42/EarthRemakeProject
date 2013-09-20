#include "Building/BuildingReference.h"

BuildingReference::BuildingReference() :
	id(),
	refID(),
	buildingArea() {

}

BuildingReference::BuildingReference(uint32_t id, uint32_t refID, const MapRectArea& area) :
	id(id),
	refID(refID),
	buildingArea(area) {

}

BuildingReference::~BuildingReference() {

}
