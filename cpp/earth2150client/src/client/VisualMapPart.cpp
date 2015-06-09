#include "client/VisualMapPart.h"

#include <cassert>

using namespace irr;

VisualMapPart::VisualMapPart(const VisualMap& map, uint16_t x, uint16_t y) :
	x(x),
	y(y),
	meshBuffer() {

	buildMesh(map);
}

VisualMapPart::~VisualMapPart() {
	assert(meshBuffer.getReferenceCount() == 1 && "MeshBuffer Reference Count > 1");
}

void VisualMapPart::updateNormals(const VisualMap& map) {
	const uint16_t size = VisualMap::VISUAL_PART_SIZE;

	const uint16_t mapX = this->x * size;
	const uint16_t mapY = this->y * size;

	// Ignoriere Kartenrand
	uint16_t minX = this->x == 0 ? 1 : 0;
	uint16_t minY = this->y == 0 ? 1 : 0;
	uint16_t maxX = this->x == 7 ? size : size + 1;
	uint16_t maxY = this->y == 7 ? size : size + 1;

	// Iteriere über inneres Feld und setze Normalen
	for (uint16_t y = minY; y < maxY; ++y) {
		for (uint16_t x = minX; x < maxX; ++x) {
			const float left_height = map.getField3DHeight(map.position(mapX + x - 1, mapY + y));
			const float right_height = map.getField3DHeight(map.position(mapX + x + 1, mapY + y));
			const float top_height = map.getField3DHeight(map.position(mapX + x, mapY + y - 1));
			const float bottom_height = map.getField3DHeight(map.position(mapX + x, mapY + y + 1));

			meshBuffer.Vertices[y * (size + 1) + x].Normal =
			core::vector3df(0.0, top_height - bottom_height, 2.0).crossProduct(core::vector3df(2.0, right_height - left_height, 0.0)).normalize();
		}
	}
}

void VisualMapPart::buildMesh(const VisualMap& map) {
	const uint16_t partX = this->x * map.VISUAL_PART_SIZE;
	const uint16_t partY = this->y * map.VISUAL_PART_SIZE;

	const uint16_t mapWidth = map.getWidth();
	const uint16_t mapHeight = map.getHeight();

	const uint8_t sizeX = partX + VisualMap::VISUAL_PART_SIZE < mapWidth ? VisualMap::VISUAL_PART_SIZE : mapWidth - partX;
	const uint8_t sizeY = partY + VisualMap::VISUAL_PART_SIZE < mapHeight ? VisualMap::VISUAL_PART_SIZE : mapHeight - partY;

	// Vertex und Zuweisungsarray holen und vorskalieren
	core::array<video::S3DVertex>& vertices = meshBuffer.Vertices;
	core::array<u16>& indices = meshBuffer.Indices;

	vertices.clear();
	indices.clear();

	// Bestimmen wieviel Einträge gebraucht werden
	const uint32_t countVertices = (sizeX + 1) * (sizeY + 1);
	const uint32_t countIndices = sizeX * sizeY * 6;

	// Speicher direkt allokieren um unnötiges realloc zu sparen
	vertices.reallocate(countVertices);
	indices.reallocate(countIndices);

	// Setze alle Vertices
	for (uint16_t y = 0; y <= sizeY; ++y) {
		for (uint16_t x = 0; x <= sizeX; ++x) {
			// Hole 3D-Höhe der Map
			double dHeight = map.getField3DHeight(map.position(partX + x, partY + y));

			// Setze den Vertice an die 3D Position
			vertices.push_back(video::S3DVertex(partX + x, dHeight, mapHeight - (partY + y), 0,0,0, video::SColor(255,255,255,255), x, y));
		}
	}

	// Verbinde die Vertices zu Dreiecken (d.h. Baue eine Feld-Fläche auf)
	for (uint16_t y = 0; y < sizeY; ++y) {
		for (uint16_t x = 0; x < sizeX; ++x) {
			int offset = y * (sizeX + 1)  + x;

			indices.push_back(offset);
			indices.push_back(offset + 1);
			indices.push_back(offset + 1 + (sizeX + 1));

			indices.push_back(offset);
			indices.push_back(offset + 1 + (sizeX + 1));
			indices.push_back(offset + (sizeX + 1));
		}
	}

	// Prüfen ob vorberechnte Anzahl stimmt
	assert(vertices.size() == countVertices);
	assert(indices.size() == countIndices);

	// Material zuweißen (temp) und Mesh-Buffer in Mesh setzen
	updateMaterial(map);
	updateNormals(map);

	// Flag setzen, damit das Mesh im VRAM gespeichert wird
	meshBuffer.setHardwareMappingHint(scene::EHM_STATIC, scene::EBT_VERTEX_AND_INDEX);

	meshBuffer.recalculateBoundingBox();
}

void VisualMapPart::updateMaterial(const VisualMap& map) {
	// Todo: richtiges Material verwenden
	meshBuffer.Material = map.getMaterial(0);
}

void VisualMapPart::updateTerrainHeight(const VisualMap& map, uint16_t startX, uint16_t startY, uint16_t endX, uint16_t endY) {
	assert(startX <= VisualMap::VISUAL_PART_SIZE);
	assert(startY <= VisualMap::VISUAL_PART_SIZE);

	assert(endX <= VisualMap::VISUAL_PART_SIZE);
	assert(endY <= VisualMap::VISUAL_PART_SIZE);

	uint16_t partX = this->x * VisualMap::VISUAL_PART_SIZE;
	uint16_t partY = this->y * VisualMap::VISUAL_PART_SIZE;

	core::array<video::S3DVertex>& vertices = meshBuffer.Vertices;

	for (uint16_t y = startY; y <= endY; ++y) {
		for (uint16_t x = startX; x <= endX; ++x) {
			double dHeight = map.getField3DHeight(map.position(partX + x, partY + y));

			vertices[y * (VisualMap::VISUAL_PART_SIZE + 1) + x].Pos.Y = dHeight;
		}
	}

	// Setzte MeshBuffer als Verändert, damit es neu zur Grafikkarte gesendet wird
	meshBuffer.setDirty();
}

void VisualMapPart::removeHardwareBuffer(irr::video::IVideoDriver* driver) {
	driver->removeHardwareBuffer(&meshBuffer);
}
