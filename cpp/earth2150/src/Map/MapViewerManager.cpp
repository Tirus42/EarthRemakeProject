#include "Map/MapViewerManager.h"

#include "e2150/HumanPlayer.h"
#include "e2150/Entity.h"
#include <iostream>
#include <cassert>

MapViewerManager::MapViewerManager(Map& map) :
	map(map),
	viewers() {

}

MapViewerManager::~MapViewerManager() {
	if (viewers.size() != 0) {
		std::cout << "WARNUNG! MapVierManager Destruktor aufgerufen, obwohl noch Spieler eingetragen!\n";
	}
}

void MapViewerManager::addHumanPlayer(HumanPlayer* player) {
	viewers.push_back(player);

	player->addVisibleMap(map);
}

void MapViewerManager::removeHumanPlayer(HumanPlayer* player) {
	for (std::vector<HumanPlayer*>::iterator i = viewers.begin(); i != viewers.end(); ++i) {
		if ((*i) == player) {
			viewers.erase(i);
			player->removeVisibleMap(map);
			return;
		}
	}

	assert(false);
}

void MapViewerManager::createEntity(const Entity& entity) {
	char* buffer = new char[entity.dumpDataSize() + 1];

	buffer[0] = '\xC8'; //0xC8 = 200
	int32_t size = entity.dumpData(&buffer[1]);

	for (std::vector<HumanPlayer*>::iterator i = viewers.begin(); i != viewers.end(); ++i) {
		(*i)->getConnection().sendPacket(buffer, size + 1);
	}
}

void MapViewerManager::removeEntity(const Entity& entity) {
	//Todo: siehe createEntity
}


void MapViewerManager::debugPaintField(uint32_t position, uint32_t color) {
	std::list<uint32_t> list;
	list.push_back(position);
	debugPaintFields(list, color);
}

void MapViewerManager::debugPaintFields(const std::list<uint32_t>& fields, uint32_t color) {
	for (std::vector<HumanPlayer*>::iterator i = viewers.begin(); i != viewers.end(); ++i) {
		(*i)->debugPaintFields(fields, color);
	}
}
