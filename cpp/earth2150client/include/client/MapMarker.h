#ifndef MAPMARKER_H_INCLUDED
#define MAPMARKER_H_INCLUDED

#include <stdint.h>

namespace irr {
	namespace scene {
		template <class T> class CMeshBuffer;
	};
	namespace video {
		class S3DVertex;
		class SMaterial;
	}
};

class MapMarkerManager;
class MapPosition;
class MapRectArea;
class VisualMap;

/**
* Bietet die Schnittstelle, um einzelne Felder auf der Karte zu Markieren.
* Wird von dem MapMarkerManager erzeugt.
*/
class MapMarker {
	private:
		MapMarker(const MapMarker&);
		MapMarker operator=(const MapMarker&);

		/// Anzahl der Markierten Felder
		uint32_t fieldCount;

		/// Referenz auf den Manager
		const MapMarkerManager& manager;

		/// MeshBuffer welcher auf die Map gezeichnet wird
		irr::scene::CMeshBuffer<irr::video::S3DVertex>* meshBuffer;

		void addFieldToMesh(const VisualMap& map, uint16_t x, uint16_t y);

	public:
		MapMarker(const MapMarkerManager& manager, const irr::video::SMaterial& mat);
		~MapMarker();

		/// Markiert ein weiteres Feld auf der Map
		void addField(const MapPosition& position);

		/// Füllt das Gebiet mit der Markierung aus
		void addMapRectArea(const MapRectArea& area);

		/// Entfernt alle Markierungen von diesem Marker
		void clear();

		irr::scene::CMeshBuffer<irr::video::S3DVertex>* getMeshBuffer() const {return meshBuffer;}

		irr::video::SMaterial& getMaterial();

};

#endif // MAPMARKER_H_INCLUDED
