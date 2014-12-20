#ifndef VISUALMAP_H_INCLUDED
#define VISUALMAP_H_INCLUDED

#include "Map/Map.h"
#include "client/MapMarkerManager.h"

#include <irrlicht.h>

#include <vector>

class VisualMapPart;
class MapPosition;
class MapRectArea;

class VisualMap : public Map {
	private:
		irr::video::IVideoDriver* driver;
		irr::scene::ISceneManager* smgr;

		std::vector<VisualMapPart*> mapParts;
		std::vector<irr::video::SMaterial> materials;

		irr::s32 meshID;

		/// Das Mesh welchem die Map besteht.
		irr::scene::SMesh* mesh;

		/// Manager für Markierungen auf der Map
		MapMarkerManager MarkerManager;

		VisualMapPart* getMapPart(uint16_t partX, uint16_t partY) const;

		VisualMap(const VisualMap&);
		VisualMap operator=(const VisualMap&);
	public:
		/// Definiert in was für Größen die Map eingeteilt wird (Grafisch)
		static const uint8_t VISUAL_PART_SIZE = 128;



		VisualMap(irr::video::IVideoDriver* driver, irr::scene::ISceneManager* smgr, uint16_t width, uint16_t height);
		~VisualMap();

		/// Gibt die 3D Höhe des Feldes auf der Karte zurück (Formel wird ggf. noch angepasst!)
		double getField3DHeight(uint32_t position) const {return getRawHeight(position) / 2560.0;}

		/// Gibt die Referenz auf das Material zurück
		const irr::video::SMaterial& getMaterial(int32_t index) const {
			return materials[index];
		}

		/// Gibt die Referenz auf das Material zurück
		irr::video::SMaterial& getMaterial(int32_t index) {
			return materials[index];
		}

		/// Aktualisiert die Interne Struktur, muss aufgerufen werden wenn das Material verändert wurde!
		void updateMaterial();

		/// Aktualisiert die Höhe des Terrains im angegebenen Bereich
		void updateTerrainHeight(const MapRectArea& area);

		/// Aktualisiert die Texturen des Terrains im angegebenene Bereich
		void updateTerrainTexture(const MapRectArea& area);

		/// Test Methode zum erstellen aller Map Parts
		void build();

		/// Setzt die Mesh ID, welche bei allen MapParts gesetzt wird
		void setMeshID(irr::s32 id) {meshID = id;}

		/// Gibt die Referenz auf den MapMarkerManager zurück
		MapMarkerManager& getMapMarkerManager() {return MarkerManager;}

		/// Prüft auf welches Feld auf der Map der Richtungsvektor trifft
		/// \return Die MapPosition mit dem Feld, invalides Feld, falls kein Feld getroffen wird
		MapPosition pickMapPosition(const irr::core::vector3df& source, const irr::core::vector3df& direction) const;

		/// Rendert das Sichtbare Terrain
		void drawTerrain(irr::video::IVideoDriver* driver) const;

};

#endif // VISUALMAP_H_INCLUDED
