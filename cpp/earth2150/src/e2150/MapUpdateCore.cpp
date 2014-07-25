#include "e2150/MapUpdateCore.h"

#include "Map/MapManipulator.h"
#include "Map/MapRectArea.h"

MapUpdateCore::MapUpdateCore(Map& map) :
	map(map),
	buildingUpdater(map),
	tickCount(0) {

}

MapUpdateCore::~MapUpdateCore() {

}


void MapUpdateCore::doTick() {

	buildingUpdater.doTick(map, *this);

	tickCount++;
}
