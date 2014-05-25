#include "Map/MapRectArea.h"

#include <cassert>

MapRectArea::MapRectArea() :
	minEdge(MapPosition::InvalidPosition()),
	maxEdge(MapPosition::InvalidPosition()) {
}

MapRectArea::MapRectArea(const MapPosition& pos1, const MapPosition& pos2) :
	minEdge(pos1),
	maxEdge(pos2) {

	// Sichergehen das diese Positionen gültig sind
	assert(pos1.isValid());
	assert(pos2.isValid());

	// minEdge und maxEdge richtig setzen, falls nicht schon richtig
	repair();
}

MapRectArea::MapRectArea(const MapPosition& pos, uint16_t width, uint16_t height) :
	minEdge(pos),
	maxEdge(MapPosition(pos.getX() + width, pos.getY() + height)) {

	// Sichergehen das die Position gültig ist
	assert(pos.isValid());

	// Sichergehen das die Fläche nicht über den Wertebereich hinaus ging
	assert((uint32_t)pos.getX() + (uint32_t)width < 0xFFFF);
	assert((uint32_t)pos.getY() + (uint32_t)height < 0xFFFF);
}

MapRectArea::MapRectArea(const MapPosition& pos, uint16_t radius) :
	minEdge(pos.getX() - radius, pos.getY() - radius),
	maxEdge(pos.getX() + radius, pos.getY() + radius) {

	assert(pos.isValid());

	// Auf Unterlauf prüfen
	if (minEdge.getX() > pos.getX())
		minEdge.setX(0);

	if (minEdge.getY() > pos.getY())
		minEdge.setY(0);

	// Auf Überlauf prüfen

	if (maxEdge.getX() < pos.getX())
		maxEdge.setX(0);

	if (maxEdge.getY() < pos.getY())
		maxEdge.setY(0);
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

bool MapRectArea::isValidOnMap(const Map& map) const {
	return minEdge.isValidOnMap(map) && maxEdge.isValidOnMap(map);
}

bool MapRectArea::isValidOnUsableMapArea(const Map& map) const {
	return minEdge.isValidOnUsableMapArea(map) && maxEdge.isValidOnUsableMapArea(map);
}

void MapRectArea::makeValidOnMap(const Map& map) {
	minEdge.makeValidOnMap(map);
	maxEdge.makeValidOnMap(map);

	assert(isValidOnMap(map));
}
