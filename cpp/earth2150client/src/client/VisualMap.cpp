#include "client/VisualMap.h"

#include "client/VisualMapPart.h"

#include <cassert>
#include <iostream>

using namespace irr;

VisualMap::VisualMap(irr::video::IVideoDriver* driver, scene::ISceneManager* smgr,  uint16_t width, uint16_t height) :
	Map(width, height),
	driver(driver),
	smgr(smgr),
	mapParts(),
	materials(),
	meshID(-1),
	mesh(0),
	node(0),
	MarkerManager(*this, smgr) {

	driver->grab();
	smgr->grab();

	// Testweiße eine Textur laden
	video::IImage* img = driver->createImageFromFile("sand1.jpg");
	video::ITexture* tex = driver->addTexture("test", img);

	if (tex == 0) {
		std::cout << "Konnte Textur nicht laden!\n";
	}

	video::SMaterial m;

	//m.BackfaceCulling = false;
	//m.Lighting = false;

	m.setTexture(0, tex);
	//m.Wireframe = true;

	m.AmbientColor.set(255,255,255,255);
	m.DiffuseColor.set(255,255,255,255);
	m.EmissiveColor.set(127,127,127,127);

	materials.push_back(m);
}

VisualMap::~VisualMap() {
	// Alle Map Parts entfernen
	for (std::vector<VisualMapPart*>::const_iterator i = mapParts.begin(); i != mapParts.end(); ++i) {
		delete *i;
	}

	if (node)
		node->remove();

	if (mesh)
		mesh->drop();

	smgr->drop();
	driver->drop();
}

void VisualMap::build() {
	int32_t width = getWidth() / VISUAL_PART_SIZE;
	int32_t height = getHeight() / VISUAL_PART_SIZE;

	//width = 16;
	//height = 16;
	if (mesh) {
		mesh->clear();
	} else {
		mesh = new scene::SMesh();
	}

	for (int32_t y = 0; y < height; ++y) {
		for (int32_t x = 0; x < width; ++x) {
			VisualMapPart* part = new VisualMapPart(*this, x, y);

			irr::scene::CMeshBuffer<irr::video::S3DVertex>* mPart = part->getMeshBuffer();

			mesh->addMeshBuffer(mPart);

			mapParts.push_back(part);
		}
	}

	scene::IMeshManipulator* manipulator = smgr->getMeshManipulator();

	// Vorerst Mesh-Normale automatisch berechnen lassen (Gibt unschöne Kanten)
	manipulator->recalculateNormals(mesh);

	// Flag setzen, damit das Mesh im VRAM gehalten wird (deutlich schneller)
	mesh->setHardwareMappingHint(scene::EHM_STATIC, scene::EBT_VERTEX_AND_INDEX);

	mesh->recalculateBoundingBox();

	this->node = smgr->addMeshSceneNode(mesh, 0, meshID);

	// Triangle Selector für das Mesh Setzen
	scene::ITriangleSelector* selector = smgr->createTriangleSelector(mesh, node);
	node->setTriangleSelector(selector);
	selector->drop();
}


MapPosition VisualMap::pickMapPosition(const core::vector3df& source, const core::vector3df& direction) {
	// Hole Bounding Box der Map
	const core::aabbox3df boundingBox = mesh->BoundingBox;

	core::vector3df myDirection(0);

	// Prüfen ob Richtungsvektor gleich dem Nullvektor ist
	if (direction == myDirection)
		return MapPosition::InvalidPosition();

	// Vektor Normalisieren
	myDirection = direction;
	myDirection.normalize();

	core::vector3df position = source + this->node->getPosition();

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
			core::vector3df v0((int)position.X, 	f0, (int)position.Z);
			core::vector3df v1((int)position.X + 1, f1, (int)position.Z);
			core::vector3df v2((int)position.X, 	f2, (int)position.Z + 1);
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
