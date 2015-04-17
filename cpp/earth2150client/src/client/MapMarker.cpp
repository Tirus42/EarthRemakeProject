#include "client/MapMarker.h"

#include "client/MapMarkerManager.h"

#include "client/VisualMap.h"
#include "Map/MapPosition.h"
#include "Map/MapRectArea.h"

#include <irrlicht.h>

using namespace irr;

MapMarker::MapMarker(const MapMarkerManager& manager, const video::SMaterial& mat) :
	fieldCount(0),
	manager(manager),
	meshBuffer(0) {

		meshBuffer = new scene::SMeshBuffer();

		meshBuffer->Material = mat;

}

MapMarker::~MapMarker() {
	meshBuffer->drop();
}

void MapMarker::addFieldToMesh(const VisualMap& map, uint16_t x, uint16_t y) {

	// Todo: Fall behandeln wenn mehr als 16k Vertices gesetzt wurden

	const uint16_t mapHeight = map.getHeight();

	// Hole Höre der Karte für das Feld
	double h1 = map.getField3DHeight(map.position(x, y));
	double h2 = map.getField3DHeight(map.position(x + 1, y));
	double h3 = map.getField3DHeight(map.position(x, y + 1));
	double h4 = map.getField3DHeight(map.position(x + 1, y + 1));

	// Hole Vertex und Index Arrays
	core::array<video::S3DVertex> &vertices = meshBuffer->Vertices;
	core::array<u16> &indices = meshBuffer->Indices;

	u16 index = vertices.size();

	// Erstelle Vertices
	vertices.push_back(video::S3DVertex(x, h1, mapHeight - y, 0,0,0, video::SColor(255,255,255,255), 0.0, 0.0));
	vertices.push_back(video::S3DVertex(x + 1, h2, mapHeight - y, 0,0,0, video::SColor(255,255,255,255), 1.0, 0.0));
	vertices.push_back(video::S3DVertex(x, h3, mapHeight - (y + 1), 0,0,0, video::SColor(255,255,255,255), 0.0, 1.0));
	vertices.push_back(video::S3DVertex(x + 1, h4, mapHeight - (y + 1), 0,0,0, video::SColor(255,255,255,255), 1.0, 1.0));

	// Verbinde Vertices zu Dreiecken
	indices.push_back(index + 0);
	indices.push_back(index + 1);
	indices.push_back(index + 3);

	indices.push_back(index + 0);
	indices.push_back(index + 3);
	indices.push_back(index + 2);
}

void MapMarker::addField(const MapPosition& position) {
	const VisualMap& map = manager.getMap();

	const uint16_t x = position.getX();
	const uint16_t y = position.getY();

	addFieldToMesh(map, x, y);

	meshBuffer->setDirty();
	meshBuffer->recalculateBoundingBox();

	manager.setDirty();
}

void MapMarker::addMapRectArea(const MapRectArea& area) {
	const VisualMap& map = manager.getMap();

	const MapPosition& minEdge = area.getMinEdge();
	const MapPosition& maxEdge = area.getMaxEdge();

	for (uint16_t y = minEdge.getY(); y <= maxEdge.getY(); ++y) {
		for (uint16_t x = minEdge.getX(); x <= maxEdge.getX(); ++x) {
			addFieldToMesh(map, x, y);
		}
	}

	meshBuffer->setDirty();
	meshBuffer->recalculateBoundingBox();

	manager.setDirty();
}

void MapMarker::clear() {
    meshBuffer->Vertices.clear();
    meshBuffer->Indices.clear();

    meshBuffer->setDirty();
    meshBuffer->recalculateBoundingBox();

    manager.setDirty();
}

video::SMaterial& MapMarker::getMaterial() {
	return meshBuffer->getMaterial();
}
