#ifndef MAPBUILDINGS_H_INCLUDED
#define MAPBUILDINGS_H_INCLUDED

#include "Util/MemoryBlock.h"
#include "Building/BuildingReference.h"

#include <stdint.h>
#include <vector>

class IMapBuildingsObserver;

/**
* Verwaltet alle Gebäude auf einer Map.
* Bietet eine Schnittstelle für Observer, welche über neue und entfernte Gebäude
* informiert werden.
*/
class MapBuildings {
	private:
		/// Referenzliste der Gebäude
		MemoryBlock<BuildingReference> buildings;

		/// Liste der Observer, welche über Änderungen Benachrichtigt werden sollen
		std::vector<IMapBuildingsObserver*> observers;

		MapBuildings(const MapBuildings&);
		MapBuildings operator=(const MapBuildings&);

	public:
		MapBuildings();
		~MapBuildings();

		/// Gibt die Anzahl an Gebäuden zurück
		uint32_t getCount() const;

		/// Fügt einen Observer in die Benachrichtigungsliste ein
		void attachObserver(IMapBuildingsObserver* observer);

		/// Entfernt einen Observer aus der Benachrichtigungsliste
		void detachObserver(IMapBuildingsObserver* observer);

};


#endif // MAPBUILDINGS_H_INCLUDED
