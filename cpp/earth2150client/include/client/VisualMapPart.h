#ifndef VISUALMAPPART_H_INCLUDED
#define VISUALMAPPART_H_INCLUDED

#include "client/VisualMap.h"

#include "Irrlicht.h"

/**
* Stellt einen Teil der Karte dar
* Der Konstruktor erstellt ein (3D) Mesh von der angegebenen Map stelle
*/
class VisualMapPart {
	private:
		uint16_t x;
		uint16_t y;

		/// Zeiger auf das Irrlicht-Mesh Objekt
		irr::scene::SMesh* mesh;
		/// Referenz auf das Vertex-Array
		//irr::core::array<irr::video::S3DVertex> &vertices;

		void buildMesh(const VisualMap& map);

		/// Setzt alle Normalen neu
		void updateNormals();
		void updateNormalsBorderTop();
		void updateNormalsBorderBottom();
		void updateNormalsBorderLeft();
		void updateNormalsBorderRight();

	public:
		VisualMapPart(const VisualMap& map, uint16_t x, uint16_t y);

		irr::scene::SMesh* getMesh() const {return mesh;}


};


#endif // VISUALMAPPART_H_INCLUDED
