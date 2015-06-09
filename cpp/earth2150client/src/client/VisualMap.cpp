#include "client/VisualMap.h"

#include "client/VisualMapPart.h"

#include "Map/MapRectArea.h"
#include "renderer/ScreenRendererHelper.h"

#include <cassert>

using namespace irr;

VisualMap::VisualMap(irr::video::IVideoDriver* driver, scene::ISceneManager* smgr, uint16_t width, uint16_t height) :
	Map(width, height),
	driver(driver),
	smgr(smgr),
	mapParts(),
	materials(),
	boundingBox(),
	mapLight(),
	MarkerManager(*this, smgr) {

	driver->grab();
	smgr->grab();
}

VisualMap::~VisualMap() {
	// Alle Map Parts entfernen
	for (std::vector<VisualMapPart*>::const_iterator i = mapParts.begin(); i != mapParts.end(); ++i) {
		(*i)->removeHardwareBuffer(driver);
		delete *i;
	}

	smgr->drop();
	driver->drop();
}

VisualMapPart* VisualMap::getMapPart(uint16_t partX, uint16_t partY) {
	uint32_t index = partY * (getWidth() / VISUAL_PART_SIZE) + partX;

	assert(index < mapParts.size());

	return mapParts[index];
}

void VisualMap::updateMaterial() {
	for (size_t i = 0; i < mapParts.size(); ++i) {
		mapParts[i]->updateMaterial(*this);
	}
}

void VisualMap::updateTerrainHeight(const MapRectArea& area) {
	const MapPosition& minEdge = area.getMinEdge();
	const MapPosition& maxEdge = area.getMaxEdge();

	uint16_t chunkMinX = minEdge.getX() / VISUAL_PART_SIZE;
	uint16_t chunkMinY = minEdge.getY() / VISUAL_PART_SIZE;
	uint16_t chunkMaxX = maxEdge.getX() / VISUAL_PART_SIZE;
	uint16_t chunkMaxY = maxEdge.getY() / VISUAL_PART_SIZE;

	for (uint16_t y = chunkMinY; y <= chunkMaxY; ++y) {
		for (uint16_t x = chunkMinX; x <= chunkMaxX; ++x) {
			// Todo: Bereich auf den wirklich nötigen Bereich einschränken
			getMapPart(x, y)->updateTerrainHeight(*this, 0, 0, VISUAL_PART_SIZE, VISUAL_PART_SIZE);
		}
	}
}

void VisualMap::updateTerrainTexture(const MapRectArea& area) {
	// Todo
	assert(false);
}

void VisualMap::build() {
	// Bestimme anzahl an benötigter Kartenstücken
	uint32_t countX = getWidth() / VISUAL_PART_SIZE;
	uint32_t countY = getHeight() / VISUAL_PART_SIZE;

	if (getWidth() % VISUAL_PART_SIZE > 0)
		countX++;

	if (getHeight() % VISUAL_PART_SIZE > 0)
		countY++;

	scene::IMeshManipulator* manipulator = smgr->getMeshManipulator();

	boundingBox.reset(core::vector3df(0));

	for (uint32_t y = 0; y < countY; ++y) {
		for (uint32_t x = 0; x < countX; ++x) {
			VisualMapPart* part = new VisualMapPart(*this, x, y);

			irr::scene::SMeshBuffer* mPart = part->getMeshBuffer();

			boundingBox.addInternalBox(mPart->getBoundingBox());

			mapParts.push_back(part);
		}
	}

}

MapPosition VisualMap::pickMapPosition(const core::vector3df& source, const core::vector3df& direction) const {
	core::vector3df myDirection(0);

	// Prüfen ob Richtungsvektor gleich dem Nullvektor ist
	if (direction == myDirection)
		return MapPosition::InvalidPosition();

	// Vektor Normalisieren
	myDirection = direction;
	myDirection.normalize();

	core::vector3df position = source;

	// Zielposition, falls die Oberfläche geschnitten wird
	core::vector3df target;

	// Vektor ablaufen und prüfen, ob wir auf ein Feld der Map treffen
	while (true) {
		// Prüfen ob wir irgendwo aus der Karte raus sind
		if (myDirection.X > 0 && position.X > boundingBox.MaxEdge.X)
			break;
		if (myDirection.X < 0 && position.X < boundingBox.MinEdge.X)
			break;
		if (myDirection.Y > 0 && position.Y > boundingBox.MaxEdge.Y)
			break;
		if (myDirection.Y < 0 && position.Y < boundingBox.MinEdge.Y)
			break;
		if (myDirection.Z > 0 && position.Z > boundingBox.MaxEdge.Z)
			break;
		if (myDirection.Z < 0 && position.Z < boundingBox.MinEdge.Z)
			break;

		// Prüfen ob wir auf der Karte sind
		if (boundingBox.isPointInside(position)) {

			// Höhe des Feldes an dieser Position holen
			double f0 = getField3DHeight(this->position((int)position.X, getHeight() - (int)position.Z));
			double f1 = getField3DHeight(this->position((int)position.X + 1, getHeight() - (int)position.Z));
			double f2 = getField3DHeight(this->position((int)position.X, getHeight() - ((int)position.Z + 1)));
			double f3 = getField3DHeight(this->position((int)position.X + 1, getHeight() - ((int)position.Z + 1)));

			// Eckpunkte als Vektoren anlegen
			core::vector3df v0((int)position.X,		f0, (int)position.Z);
			core::vector3df v1((int)position.X + 1, f1, (int)position.Z);
			core::vector3df v2((int)position.X,		f2, (int)position.Z + 1);
			core::vector3df v3((int)position.X + 1, f3, (int)position.Z + 1);

			// Zu Dreieck verbinden
			core::triangle3df t1(v0, v1, v2);

			// Gegen erstes Dreieck prüfen
			if (t1.getIntersectionWithLine(position, myDirection, target))
				return MapPosition(target.X, getHeight() - target.Z);

			core::triangle3df t2(v1, v3, v2);

			// Gegen zweites Dreieck prüfen
			if (t2.getIntersectionWithLine(position, myDirection, target))
				return MapPosition(target.X, getHeight() - target.Z);
		}

		// Richtungsvektor weiter ablaufen
		position += myDirection;
	}

	// Nichts gefunden
	return MapPosition::InvalidPosition();
}

void VisualMap::drawTerrain(video::IVideoDriver* driver) const {
	driver->setTransform(video::ETS_WORLD, core::IdentityMatrix);

	driver->setMaterial(getMaterial(0));

	const scene::SViewFrustum* frust = smgr->getActiveCamera()->getViewFrustum();

	const core::vector3df camPos = smgr->getActiveCamera()->getAbsolutePosition();

	for (size_t i = 0; i < mapParts.size(); ++i) {
		const VisualMapPart* part = mapParts[i];

		const core::aabbox3df& aabb = part->getMeshBuffer()->getBoundingBox();

		// Zeichne nur Parts in welchem die Kamera ist, oder welche im ViewFrustum liegen
		if (aabb.isPointInside(camPos) || ScreenRendererHelper::isInViewFrustum(*frust, aabb)) {
			driver->drawMeshBuffer(part->getMeshBuffer());
		}
	}

}
