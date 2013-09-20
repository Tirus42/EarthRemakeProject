#include "Map/MapRectArea.h"

#include <cassert>

MapRectArea::MapRectArea(const MapPosition& pos1, const MapPosition& pos2) :
	minEdge(pos1),
	maxEdge(pos2) {

	// Sichergehen das diese Positionen gültig sind
	assert(pos1.isValid());
	assert(pos2.isValid());

	// minEdge und maxEdge richtig setzen, falls nicht schon richtig
	repair();
}

MapRectArea::~MapRectArea() {

}

void MapRectArea::repair() {
	if (maxEdge.getX() < minEdge.getX()) {
		uint16_t minX = maxEdge.getX();

		maxEdge.setX(minEdge.getX());
		minEdge.setX(minX);
	}

	if (maxEdge.getY() < minEdge.getY()) {
		uint16_t minY = maxEdge.getY();

		maxEdge.setY(minEdge.getY());
		minEdge.setY(minY);
	}
}

bool MapRectArea::isInside(const MapPosition& position) const {
	if (minEdge.getX() > position.getX())
		return false;

	if (maxEdge.getX() < position.getX())
		return false;

	if (minEdge.getY() > position.getY())
		return false;

	if (maxEdge.getY() < position.getY())
		return false;

	return true;
}

bool MapRectArea::isInside(const MapRectArea& area) const {
	if (minEdge.getX() > area.minEdge.getX())
		return false;

	if (maxEdge.getX() < area.maxEdge.getX())
		return false;

	if (minEdge.getY() > area.minEdge.getY())
		return false;

	if (maxEdge.getY() < area.maxEdge.getY())
		return false;

	return true;
}
