#include "Map/MapBuildings.h"

#include <cassert>

MapBuildings::MapBuildings() :
	buildings(1024),
	observers() {

}

MapBuildings::~MapBuildings() {
	assert(observers.size() == 0);
}

void MapBuildings::attachObserver(IMapBuildingsObserver* observer) {
	observers.push_back(observer);
}

void MapBuildings::detachObserver(IMapBuildingsObserver* observer) {
	for (std::vector<IMapBuildingsObserver*>::iterator i = observers.begin(); i != observers.end(); ++i) {
		if ((*i) == observer) {
			observers.erase(i);
			return;
		}
	}

	assert(false);
}
