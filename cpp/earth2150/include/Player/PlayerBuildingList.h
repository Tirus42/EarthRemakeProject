#ifndef PLAYERBUILDINGLIST_H_INCLUDED
#define PLAYERBUILDINGLIST_H_INCLUDED

#include <vector>
#include <stdint.h>

class GlobalBuildingList;
class BuildingChassis;

/**
* Liste der Gebäude welche der Spieler bauen kann.
* Die Einträge sind verweiße auf die Einträge der GlobalBuildingList.
*/
class PlayerBuildingList {
	private:
		PlayerBuildingList(const PlayerBuildingList&);
		PlayerBuildingList operator=(const PlayerBuildingList&);

		/// Referenz auf die Globale Gebäudeliste
		const GlobalBuildingList& globalBuildingList;

		std::vector<uint32_t> buildingIDs;

	public:
		PlayerBuildingList(const GlobalBuildingList& globalBuildingList);
		~PlayerBuildingList();

		/// Fügt ein Gebäude über seine ID hinzu
		void addID(uint32_t id);

		/// Entfernt ein bestimmtes Gebäude aus der Liste
		bool removeID(uint32_t id);

		/// Entfernt alle Gebäude aus der Liste
		void removeAll();

		/// Fügt alle Verfügbaren Gebäude in die Liste ein
		void addAll();

		/// Gibt die Anzahl der Gebäude zurück
		uint32_t getCount() const {
			return buildingIDs.size();
		}

		/// Gibt das BuildingChassis zum angegebenen Index zurück
		const BuildingChassis& getBuilding(uint32_t index) const;
};


#endif // PLAYERBUILDINGLIST_H_INCLUDED
