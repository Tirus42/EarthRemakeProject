#ifndef GAMECONTENT_H_INCLUDED
#define GAMECONTENT_H_INCLUDED

#include "e2150/FactionManager.h"
#include "Unit/UnitChassisList.h"
#include "Building/GlobalBuildingList.h"

class GameContent {
	private:
		GameContent(const GameContent&);
		GameContent operator=(const GameContent&);

		FactionManager factionManager;

		UnitChassisList unitChassisList;

		GlobalBuildingList buildingList;

	public:
		GameContent();
		~GameContent();

		/// Gibt den FactionManager zurück
		const FactionManager& getFactionManager() const {
			return factionManager;
		}

		/// Gibt den FactionManager zurück
		FactionManager& getFactionManager() {
			return factionManager;
		}

		const UnitChassisList& getUnitChassisList() const {
			return unitChassisList;
		}

		UnitChassisList& getUnitChassisList() {
			return unitChassisList;
		}

		const GlobalBuildingList& getBuildingList() const {
			return buildingList;
		}

		GlobalBuildingList& getBuildingList() {
			return buildingList;
		}


};


#endif // GAMECONTENT_H_INCLUDED
