#include "e2150/Entity.h"

Entity::Entity() :
		id(),
		mapPosition(),
		direction(0),
		hitPoints(),
		hitPointsMax() {
}

Entity::Entity(uint32_t id, int32_t hitPointsMax) :
		id(id),
		mapPosition(),
		direction(0),
		hitPoints(hitPointsMax),
		hitPointsMax(hitPointsMax) {

}

Entity::~Entity() {
}

int32_t Entity::dumpData(char* buffer) const {
	*(uint32_t*)(&buffer[0])	= getID();
	*(uint16_t*)(&buffer[4])	= getX();
	*(uint16_t*)(&buffer[6])	= getY();
	*(int32_t*)(&buffer[8])		= getHitPoints();
	*(int32_t*)(&buffer[12])	= getHitPointsMax();

	return 16;
}
