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
		irr::scene::CMeshBuffer<irr::video::S3DVertex>* meshBuffer;

		void buildMesh(const VisualMap& map);

		/// Setzt alle Normalen neu
		void updateNormals();
		void updateNormalsBorderTop();
		void updateNormalsBorderBottom();
		void updateNormalsBorderLeft();
		void updateNormalsBorderRight();

		VisualMapPart(const VisualMapPart&);
		VisualMapPart operator=(const VisualMapPart&);
	public:
		VisualMapPart(const VisualMap& map, uint16_t x, uint16_t y);
		~VisualMapPart();

		irr::scene::CMeshBuffer<irr::video::S3DVertex>* getMeshBuffer() const {return meshBuffer;}


};


#endif // VISUALMAPPART_H_INCLUDED
