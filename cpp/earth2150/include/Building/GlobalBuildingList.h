#ifndef GLOBALBUILDINGLIST_H_INCLUDED
#define GLOBALBUILDINGLIST_H_INCLUDED

#include <vector>
#include <stdint.h>

class BuildingChassis;

/**
* Bisher nur Test Implementierung!
*/
class GlobalBuildingList {
	private:
		GlobalBuildingList(const GlobalBuildingList&);
		GlobalBuildingList operator=(const GlobalBuildingList&);

		// Todo: dies nicht als Pointer sondern direkt in diesem Vektor speichern
		std::vector<BuildingChassis*> buildings;

	public:
		GlobalBuildingList();
		~GlobalBuildingList();

		/// Fügt einige Test Gebäude ein.
		void addTestBuildings();

		/// Gibt die Anzahl der Gebäude zurück
		uint32_t getBuildingCount() const {
			return buildings.size();
		}

		/// Gibt die Referenz auf die BuildingChassis zurück
		const BuildingChassis& getBuilding(uint32_t index) const;

};

#endif // GLOBALBUILDINGLIST_H_INCLUDED
