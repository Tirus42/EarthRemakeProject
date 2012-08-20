#include "e2150/MapViewerManager.h"

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
