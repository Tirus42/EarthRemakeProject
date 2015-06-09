#ifndef VISUALMAPPART_H_INCLUDED
#define VISUALMAPPART_H_INCLUDED

#include "client/VisualMap.h"

#include <irrlicht.h>

/**
* Stellt einen Teil der Karte dar
* Der Konstruktor erstellt ein (3D) Mesh von der angegebenen Map stelle
*/
class VisualMapPart {
	private:
		uint16_t x;
		uint16_t y;

		/// Zeiger auf den Meshbuffer
		irr::scene::SMeshBuffer meshBuffer;

		void buildMesh(const VisualMap& map);

		/// Setzt alle Normalen neu
		void updateNormals(const VisualMap& map);

		VisualMapPart(const VisualMapPart&);
		VisualMapPart operator=(const VisualMapPart&);
	public:
		VisualMapPart(const VisualMap& map, uint16_t x, uint16_t y);
		~VisualMapPart();

		const irr::scene::SMeshBuffer* getMeshBuffer() const {return &meshBuffer;}
		irr::scene::SMeshBuffer* getMeshBuffer() {return &meshBuffer;}

		void updateMaterial(const VisualMap& map);

		/// Setzt die Vertex Höhen in dem Bereich neu aus der Map
		void updateTerrainHeight(const VisualMap& map, uint16_t x, uint16_t y, uint16_t width, uint16_t height);

		/// Gibt dem IVideoTreiber die anweisung alle Speicher auf der GPU freizugeben
		/// Muss vor dem Destruktur aufgerufen werden
		void removeHardwareBuffer(irr::video::IVideoDriver* driver);
};


#endif // VISUALMAPPART_H_INCLUDED
