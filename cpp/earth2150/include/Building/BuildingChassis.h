#ifndef BUILDINGCHASSIS_H_INCLUDED
#define BUILDINGCHASSIS_H_INCLUDED

#include "Building/BuildingGround.h"

/**
* Stellt die Gebäude Grundinformationen dar.
* Speichert Modell, Größe, Typ usw. des Gebäudes.
* Jedes Gebäude das erstellt wird, bezieht sich auf ein
* Objekt dieser Klasse.
*/
class BuildingChassis {
	private:
		/// Objekt das die Bodenfläche des Gebäudes bestimmt
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
