#ifndef VISUALMAP_H_INCLUDED
#define VISUALMAP_H_INCLUDED

#include "Map/Map.h"
#include "client/MapMarkerManager.h"

#include <irrlicht.h>

#include <vector>

class VisualMapPart;

class VisualMap : public Map {
	private:
		irr::video::IVideoDriver* driver;
		irr::scene::ISceneManager* smgr;

		std::vector<VisualMapPart*> mapParts;
		std::vector<irr::video::SMaterial> materials;

		irr::s32 meshID;

		/// Das Mesh welchem die Map besteht.
		irr::scene::SMesh* mesh;

		/// Irrlicht Scene Node der Map
		irr::scene::IMeshSceneNode* node;

		/// Manager für Markierungen auf der Map
		MapMarkerManager MarkerManager;

		VisualMap(const VisualMap&);
		VisualMap operator=(const VisualMap&);
	public:
		/// Definiert in was für Größen die Map eingeteilt wird (Grafisch)
		static const uint8_t VISUAL_PART_SIZE = 128;



		VisualMap(irr::video::IVideoDriver* driver, irr::scene::ISceneManager* smgr, uint16_t width, uint16_t height);
		~VisualMap();

		/// Gibt die 3D Höhe des Feldes auf der Karte zurück (Formel wird ggf. noch angepasst!)
		double getField3DHeight(uint32_t position) const {return getRawHeight(position) / 2560.0;}

		const irr::video::SMaterial& getMaterial(int32_t index) const {return materials[index];}

		/// Test Methode zum erstellen aller Map Parts
		void build();

		/// Setzt die Mesh ID, welche bei allen MapParts gesetzt wird
		void setMeshID(irr::s32 id) {meshID = id;}

		/// Gibt die Referenz auf den MapMarkerManager zurück
		MapMarkerManager& getMapMarkerManager() {return MarkerManager;}

		/// Prüft auf welches Feld auf der Map der Richtungsvektor trifft
		/// \return Die MapPosition mit dem Feld, invalides Feld, falls kein Feld getroffen wird
		MapPosition pickMapPosition(const irr::core::vector3df& source, const irr::core::vector3df& direction);
};

#endif // VISUALMAP_H_INCLUDED
