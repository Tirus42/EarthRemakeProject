#include "Player/PlayerBuildingList.h"

#include "Building/GlobalBuildingList.h"

#include <cassert>

PlayerBuildingList::PlayerBuildingList(const GlobalBuildingList& globalBuildingList) :
	globalBuildingList(globalBuildingList),
	buildingIDs() {
}

PlayerBuildingList::~PlayerBuildingList() {
}

void PlayerBuildingList::addID(uint32_t id) {
	assert(id < globalBuildingList.getBuildingCount());

	buildingIDs.push_back(id);
}

bool PlayerBuildingList::removeID(uint32_t id) {
	for (uint32_t i = 0; i < buildingIDs.size(); ++i) {
		if (buildingIDs[i] == id) {
			buildingIDs.erase(buildingIDs.begin() + i);
			return true;
		}
	}

	return false;
}

void PlayerBuildingList::removeAll() {
	buildingIDs.clear();
}

void PlayerBuildingList::addAll() {
	removeAll();

	const uint32_t count = globalBuildingList.getBuildingCount();

	for (uint32_t i = 0; i < count; ++i) {
		addID(i);
	}
}

const BuildingChassis& PlayerBuildingList::getBuilding(uint32_t index) const {
	assert(index < buildingIDs.size());

	return globalBuildingList.getBuilding(buildingIDs.at(index));
}
