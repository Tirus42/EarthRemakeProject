#include "client/VisualMapPart.h"

#include <cassert>
#include <iostream>

using namespace irr;

VisualMapPart::VisualMapPart(const VisualMap& map, uint16_t x, uint16_t y) :
	x(x),
	y(y),
	meshBuffer(0) {

	buildMesh(map);
}

VisualMapPart::~VisualMapPart() {
	// Wir geben unsere Referenz auf das Mesh ab
	meshBuffer->drop();
}

void VisualMapPart::updateNormals() {
	/// Lagere "teure" Randfallprüfung in Extra Methoden aus (und nicht in Schleife)
	updateNormalsBorderTop();

	const uint16_t size = VisualMap::VISUAL_PART_SIZE;

	/// Iteriere über inneres Feld und setze Normalen
	for (uint16_t y = 1; y < size; ++y) {
		for (uint16_t x = 1; x < size; ++x) {

		}
	}
}

void VisualMapPart::updateNormalsBorderTop() {

	core::vector3df left(-1, 0, 0);
}

void VisualMapPart::buildMesh(const VisualMap& map) {
	const uint8_t size = VisualMap::VISUAL_PART_SIZE;

	const uint16_t partX = this->x * map.VISUAL_PART_SIZE;
	const uint16_t partY = this->y * map.VISUAL_PART_SIZE;

	const uint16_t mapHeight = map.getHeight();

	// Mesh-Buffer erstellen
	if (meshBuffer)
		meshBuffer->drop();

	meshBuffer = new scene::CMeshBuffer<video::S3DVertex>();

	// Vertex und Zuweisungsarray holen und vorskalieren
	core::array<video::S3DVertex> &vertices = meshBuffer->Vertices;
	core::array<u16> &indices = meshBuffer->Indices;

	// Setze alle Vertices
	for (uint16_t y = 0; y <= size; ++y) {
		for (uint16_t x = 0; x <= size; ++x) {
			// Hole 3D-Höhe der Map
			double dHeight = map.getField3DHeight(map.position(partX + x, partY + y));

			// Setze den Vertice an die 3D Position
			vertices.push_back(video::S3DVertex(partX + x, dHeight, mapHeight - (partY + y), 0,0,0, video::SColor(255,255,255,255), x, y));
		}
	}

	// Verbinde die Vertices zu Dreiecken (d.h. Baue eine Feld-Fläche auf)
	for (uint16_t y = 0; y < size; ++y) {
		for (uint16_t x = 0; x < size; ++x) {
			int offset = y * (size + 1)  + x;

			indices.push_back(offset);
			indices.push_back(offset + 1);
			indices.push_back(offset + 1 + (size + 1));

			indices.push_back(offset);
			indices.push_back(offset + 1 + (size + 1));
			indices.push_back(offset + (size + 1));
		}
	}

	// Material zuweißen (temp) und Mesh-Buffer in Mesh setzen
	updateMaterial(map);

	meshBuffer->recalculateBoundingBox();
}

void VisualMapPart::updateMaterial(const VisualMap& map) {
	// Todo: richtiges Material verwenden
	meshBuffer->Material = map.getMaterial(0);
}
