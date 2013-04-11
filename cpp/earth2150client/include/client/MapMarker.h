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

/**
* Bietet die Schnittstelle, um einzelne Felder auf der Karte zu Markieren.
* Wird von dem MapMarkerManager erzeugt.
*/
class MapMarker {
	private:
		MapMarker(const MapMarker&);
		MapMarker operator=(const MapMarker&);

		uint32_t fieldCount;

		const MapMarkerManager& manager;

		irr::scene::CMeshBuffer<irr::video::S3DVertex>* meshBuffer;

	public:
		MapMarker(const MapMarkerManager& manager, const irr::video::SMaterial& mat);
		~MapMarker();

		void addField(const MapPosition& position);

		irr::scene::CMeshBuffer<irr::video::S3DVertex>* getMeshBuffer() const {return meshBuffer;}

		irr::video::SMaterial& getMaterial();

};

#endif // MAPMARKER_H_INCLUDED
