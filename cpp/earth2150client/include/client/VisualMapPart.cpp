#include "client/VisualMapPart.h"

#include <iostream>

using namespace irr;

VisualMapPart::VisualMapPart(const VisualMap& map, uint16_t x, uint16_t y) :
	x(x),
	y(y) {

	buildMesh(map);
}

void VisualMapPart::buildMesh(const VisualMap& map) {
	const uint8_t size = VisualMap::VISUAL_PART_SIZE;

	const uint16_t partX = this->x * map.VISUAL_PART_SIZE;
	const uint16_t partY = this->y * map.VISUAL_PART_SIZE;

	/// Mesh-Buffer erstellen

	scene::CMeshBuffer<video::S3DVertex>* buffer = new scene::CMeshBuffer<video::S3DVertex>();

	/// Vertex und Zuweisungsarray holen und vorskalieren
	core::array<video::S3DVertex> &vertices = buffer->Vertices;
	core::array<u16> &indices = buffer->Indices;

	//vertices.reallocate((size + 1) * (size + 1));
	//Todo indeces auch skalieren

	/// Setze alle Vertices
	for (uint16_t y = 0; y <= size; ++y) {
		for (uint16_t x = 0; x <= size; ++x) {
			/// Hole Höhe der Map
			uint16_t fHeight = map.getRawHeight(map.position(partX + x, partY + y));

			double dHeight = fHeight / 2560.0;

			/// Setze den Vertice an die 3D Position
			vertices.push_back(video::S3DVertex(x, dHeight,	y, 0,0,0, video::SColor(255,255,255,255), x, y));
		}
	}

	/// Verbinde die Vertices zu Dreiecken
	for (uint16_t y = 0; y < size; ++y) {
		for (uint16_t x = 0; x < size; ++x) {
			int offset = y * (size + 1) + x;

			indices.push_back(offset);
			indices.push_back(offset + 1);
			indices.push_back((y + 1) * (size + 1) + x + 1);

			indices.push_back((y + 1) * (size + 1) + x + 1);
			indices.push_back((y + 1) * (size + 1) + x);
			indices.push_back(offset);
		}
	}

	/// Material zuweißen (temp) und Mesh-Buffer in Mesh setzen
	buffer->Material = map.getMaterial(0);

	buffer->setDirty(scene::EBT_VERTEX_AND_INDEX);
	buffer->recalculateBoundingBox();

	/// Eigendliches Mesh erstellen
	mesh = new scene::SMesh();

	mesh->addMeshBuffer(buffer);
	mesh->recalculateBoundingBox();
	mesh->setDirty(scene::EBT_VERTEX_AND_INDEX);

	core::aabbox3d<f32> Box2 = mesh->getBoundingBox();

	std::cout << "Mesh Bouncing Box\n";
	std::cout << Box2.getCenter().X << " " << Box2.getCenter().Y << " " << Box2.getCenter().Z << "\n";

}
