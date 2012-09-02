#include "e2150/MapViewerManager.h"

#include "e2150/PlayerViewArea.h"
#include "e2150/HumanPlayer.h"

MapViewerManager::MapViewerManager(uint16_t rasterSize) :
	viewers(),
	rasterSize(rasterSize) {

}

void MapViewerManager::createEntity(const Entity& entity) {
	//Todo: prüfen ob senden wirklich erforderlich

	//Todo: Paket an alle Spieler senden
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
	for (std::list<PlayerViewArea*>::iterator i = viewers.begin(); i != viewers.end(); ++i) {
		(*i)->getPlayer().debugPaintFields(fields, color);
	}
}
