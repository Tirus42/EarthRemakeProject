#include "client/VisualMap.h"

#include "client/VisualMapPart.h"

#include <cassert>
#include <iostream>

using namespace irr;

VisualMap::VisualMap(irr::video::IVideoDriver* driver, scene::ISceneManager* smgr,  uint16_t width, uint16_t height) :
	driver(driver),
	smgr(smgr),
	meshID(-1),
	Map(width, height),
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

	smgr->drop();
	driver->drop();
}

void VisualMap::build() {
	int32_t width = getWidth() / VISUAL_PART_SIZE;
	int32_t height = getHeight() / VISUAL_PART_SIZE;

	//width = 16;
	//height = 16;

	// Erstelle aus dem Mesh ein "AnimatedMesh" und füge es der Scene hinzu
	scene::IMeshManipulator* manipulator = smgr->getMeshManipulator();

	for (int32_t y = 0; y < height; ++y) {
		for (int32_t x = 0; x < width; ++x) {
			VisualMapPart* part = new VisualMapPart(*this, x, y);

			scene::IMesh* mesh = part->getMesh();

			// Vorerst Mesh-Normale automatisch berechnen lassen (Gibt unschöne Kanten)
			manipulator->recalculateNormals(mesh);

			scene::IMeshSceneNode* node = smgr->addMeshSceneNode(mesh, 0, meshID,
							core::vector3df(x * VISUAL_PART_SIZE, 0, -y * VISUAL_PART_SIZE));

			// Flag setzen das das Mesh im VRAM gehalten wird
			mesh->setHardwareMappingHint(scene::EHM_STATIC, scene::EBT_VERTEX_AND_INDEX);

			// Triangle Selector für jeden Part setzen
			scene::ITriangleSelector* selector = smgr->createTriangleSelector(mesh, node);
			node->setTriangleSelector(selector);
			selector->drop();

			mapParts.push_back(part);
		}
	}
}
