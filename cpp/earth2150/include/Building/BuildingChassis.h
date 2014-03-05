#ifndef BUILDINGCHASSIS_H_INCLUDED
#define BUILDINGCHASSIS_H_INCLUDED

#include "Building/BuildingGround.h"

#include <string>

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

		/// Name des Chassis
		std::string name;

	public:
		BuildingChassis(uint8_t width, uint8_t height, const std::string& name);
		~BuildingChassis();

		/// Gibt den BuildingGround zurück, die Flächennutzung auf der Karte
		BuildingGround& getBuildingGround() {
			return ground;
		}

		/// Gibt den BuildingGround zurück, die Flächennutzung auf der Karte
		const BuildingGround& getBuildingGround() const {
			return ground;
		}

		/// Gibt den Namen des Gebäudes zurück
		const std::string& getName() const {
			return name;
		}

};

#endif // BUILDINGCHASSIS_H_INCLUDED
