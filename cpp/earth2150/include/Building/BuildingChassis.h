#ifndef BUILDINGCHASSIS_H_INCLUDED
#define BUILDINGCHASSIS_H_INCLUDED

#include "Building/BuildingGround.h"

/**
* Stellt die Geb�ude Grundinformationen dar.
* Speichert Modell, Gr��e, Typ usw. des Geb�udes.
* Jedes Geb�ude das erstellt wird, bezieht sich auf ein
* Objekt dieser Klasse.
*/
class BuildingChassis {
	private:
		/// Objekt das die Bodenfl�che des Geb�udes bestimmt
		BuildingGround ground;

	public:
		BuildingChassis(uint8_t width, uint8_t height);
		~BuildingChassis();

		BuildingGround& getBuildingGround() {
			return ground;
		}

		const BuildingGround& getBuildingGround() const {
			return ground;
		}


};

#endif // BUILDINGCHASSIS_H_INCLUDED
