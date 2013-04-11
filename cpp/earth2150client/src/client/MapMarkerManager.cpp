#include "client/MapMarkerManager.h"

#include "client/MapMarker.h"

#include <irrlicht.h>

using namespace irr;

MapMarkerManager::MapMarkerManager(const VisualMap& map, scene::ISceneManager* smgr) :
	map(map),
	smgr(smgr),
	mesh(0) {

		smgr->grab();

		mesh = new scene::SMesh();

		node = smgr->addMeshSceneNode(mesh);
}

MapMarkerManager::~MapMarkerManager() {
	node->remove();
	mesh->drop();
	smgr->drop();
}

MapMarker* MapMarkerManager::getMarkerForMaterial(const video::SMaterial& mat) {
	MapMarker* marker = new MapMarker(*this, mat);

	mesh->addMeshBuffer(marker->getMeshBuffer());

	return marker;
}


void MapMarkerManager::setDirty() const {
	mesh->setDirty();
	mesh->recalculateBoundingBox();

	node->setMesh(mesh);
}
