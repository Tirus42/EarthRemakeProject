#ifndef MAPUPDATECORE_H_INCLUDED
#define MAPUPDATECORE_H_INCLUDED

#include <stdint.h>

#include "BuildingUpdater.h"

class Map;

/**
* Stellt die Kern Klasse des Spiel Updates für eine Karte da.
* Verwaltet eine Karte und ruft entsprechende
* Update-Methoden auf, um den Spielverlauf zu berechnen.
* Ist darauf ausgelegt in einem seperaten Thread zu der
* Visualisierung des Spiels zu laufen.
* Dafür werden Listen an Änderungen erstellt, welcher der
* Visualisierungsthread abrufen kann, um nur die Änderungen
* zu übernehmen und nicht alles abfragen zu müssen.
*/
class MapUpdateCore {
	private:
		Map& map;

		BuildingUpdater buildingUpdater;

		uint32_t tickCount;

		MapUpdateCore(const MapUpdateCore&);
		MapUpdateCore operator=(const MapUpdateCore&);

	public:
		MapUpdateCore(Map& map);
		~MapUpdateCore();

		/// Berechnet einen Updates Zyklus
		void doTick();

		uint32_t getTickCount() const {
			return tickCount;
		}

};


#endif // MAPUPDATECORE_H_INCLUDED
