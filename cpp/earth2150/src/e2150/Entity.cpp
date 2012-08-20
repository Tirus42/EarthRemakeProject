#include "e2150/Entity.h"

Entity::Entity() :
		id(),
		mapPosition(),
		hitPoints(),
		hitPointsMax() {
}

Entity::Entity(uint32_t id, int32_t hitPointsMax) :
		id(id),
		mapPosition(),
		hitPoints(hitPointsMax),
		hitPointsMax(hitPointsMax) {

}

Entity::~Entity() {
}
