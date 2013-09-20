#ifndef IMAPBUILDINGSOBSERVER_H_INCLUDED
#define IMAPBUILDINGSOBSERVER_H_INCLUDED

/**
* Interface Klasse für einen Beobachter von Gebäuden.
* Soll dazu dienen, über hinzufügen und entfernen von Gebäuden Informiert zu werden.
*/
class IMapBuildingsObserver {
	private:
		IMapBuildingsObserver(const IMapBuildingsObserver&);
		IMapBuildingsObserver operator=(const IMapBuildingsObserver&);

	public:
		IMapBuildingsObserver();
		virtual ~IMapBuildingsObserver();

		/// Wird aufgerufen, wenn ein Gebäude erstellt wird
		virtual void OnEvent_NewBuilding(const BuildingReference& buildingRef) = 0;

		/// Wird aufgerufen, wenn ein Gebäude entfernt wird
		virtual void OnEvent_RemoveBuilding(const BuildingReference& buildingRef) = 0;

};


#endif // IMAPBUILDINGSOBSERVER_H_INCLUDED
