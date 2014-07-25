#include "e2150/MapUpdateCore.h"

MapUpdateCore::MapUpdateCore(Map& map) :
	map(map),
	buildingUpdater(map),
	tickCount(0),
	updatedTerrainHeight() {

}

MapUpdateCore::~MapUpdateCore() {

}


void MapUpdateCore::doTick() {

	buildingUpdater.doTick(map, *this);


	tickCount++;
}

void MapUpdateCore::extractChangedTerrainHeight(std::vector<MapRectArea>& target) {
	// Todo: Thread safe...
	updatedTerrainHeight.swap(target);
}
