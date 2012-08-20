#ifndef MAPVIEWERMANAGER_H_INCLUDED
#define MAPVIEWERMANAGER_H_INCLUDED

#include <list>
#include <stdint.h>

class PlayerViewArea;
class Entity;

/**
* Diese Klasse Verwaltet die Spieler-Kamera Positionen und bestimmt daraus,
* welche update-Informationen an den Spieler gesendet werden m�ssen.
* Dies wird mit Hilfe von "Rastern" bestimmt.
* Der Viewer bekommt eine Rastergr��e, in welche die Gesammte Map aufgeteilt wird,
* damit erh�lt der Spieler entweder den Inhalt des gesammten Rasters, oder nicht.
* Anhand der Kamera Position kann bestimmt werden, in welche Raster der Spieler
* z.Z. einsicht hat.
*/
class MapViewerManager {
	private:
		std::list<PlayerViewArea*> viewers;		//Spieler-Kameras (in diesem Bereich m�ssen die Spieler �ber Updates informiert werden)

		uint16_t rasterSize;

	public:
		MapViewerManager(uint16_t rasterSize = 32);

		/// Sendet (falls erforderlich) ein spawn-Paket an den/die Spieler
		void createEntity(const Entity& entity);

		/// Sendet (falls erforderlich) ein remove-Paket am den/die Spieler
		void removeEntity(const Entity& entity);

		/// Sendet (falls erforderlich) ein positions-Paket an den/die Spieler
		void updateEntityPosition(const Entity& entity);

};

#endif // MAPVIEWERMANAGER_H_INCLUDED
