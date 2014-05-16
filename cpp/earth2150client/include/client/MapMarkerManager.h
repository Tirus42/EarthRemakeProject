#ifndef MAPMARKERMANAGER_H_INCLUDED
#define MAPMARKERMANAGER_H_INCLUDED

namespace irr {
	namespace video {
		class SMaterial;
	}
	namespace scene {
		class SMesh;
		class IMeshSceneNode;
		class ISceneManager;
	}
}

class VisualMap;
class MapMarker;

/**
* Diese Klasse erstellt und Verwaltet alle Markierungen auf der Karte (Visuelle).
* Es können weitere Markierungen hinzugefügt, bestehende verschoben oder entfernt werden.
*/
class MapMarkerManager {
	private:
		MapMarkerManager(const MapMarkerManager&);
		MapMarkerManager operator=(const MapMarkerManager&);

		const VisualMap& map;

		irr::scene::ISceneManager* smgr;

		irr::scene::SMesh* mesh;
		irr::scene::IMeshSceneNode* node;

	public:
		MapMarkerManager(const VisualMap& map, irr::scene::ISceneManager* smgr);
		~MapMarkerManager();

		/// Gibt einen Marker für ein Bestimmtes Material
		MapMarker* getMarkerForMaterial(const irr::video::SMaterial& mat);

		const VisualMap& getMap() const {return map;}

		void setDirty() const;
};

#endif // MAPMARKERMANAGER_H_INCLUDED
