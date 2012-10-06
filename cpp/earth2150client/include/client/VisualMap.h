#ifndef VISUALMAP_H_INCLUDED
#define VISUALMAP_H_INCLUDED

#include "e2150/Map.h"

#include "Irrlicht.h"

#include <vector>

class VisualMapPart;

class VisualMap : public Map {
	private:
		irr::video::IVideoDriver* driver;

		std::vector<VisualMapPart*> mapParts;
		std::vector<irr::video::SMaterial> materials;

	public:
		/// Definiert in was für Größen die Map eingeteilt wird (Grafisch)
		static const uint8_t VISUAL_PART_SIZE = 16;



		VisualMap(irr::video::IVideoDriver* driver, uint16_t width, uint16_t height);


		const irr::video::SMaterial& getMaterial(int32_t index) const {return materials[index];}

		/// Test Methode zum erstellen des ersten Parts
		void build(irr::scene::ISceneManager* smgr);

};

#endif // VISUALMAP_H_INCLUDED
