#ifndef MAPVIEWERMANAGER_H_INCLUDED
#define MAPVIEWERMANAGER_H_INCLUDED

#include <list>
#include <vector>
#include <stdint.h>

class Map;
class Entity;
class HumanPlayer;

/**
* Verwaltet die (Menschlichen-) Spieler welche zugriff auf die Karte haben.
* (ggf. auch nur Zuschauer)
*/
class MapViewerManager {
	private:
		Map& map;

		std::vector<HumanPlayer*> viewers;		//Spieler-Kameras (in diesem Bereich müssen die Spieler über Updates informiert werden)

	public:
		MapViewerManager(Map& map);
		~MapViewerManager();

		/// Fügt weiteren Spieler hinzu
		void addHumanPlayer(HumanPlayer* player);

		/// Entfernt einen Spieler
		void removeHumanPlayer(HumanPlayer* player);

		/// Sendet (falls erforderlich) ein spawn-Paket an den/die Spieler
		void createEntity(const Entity& entity);

		/// Sendet (falls erforderlich) ein remove-Paket am den/die Spieler
		void removeEntity(const Entity& entity);

		/// Sendet (falls erforderlich) ein positions-Paket an den/die Spieler
		void updateEntityPosition(const Entity& entity);

		/// "Malt" bei allen verbundenen Spielern ein Feld auf der Karte an
		void debugPaintField(uint32_t position, uint32_t color);
		void debugPaintFields(const std::list<uint32_t>& fields, uint32_t color);

};

#endif // MAPVIEWERMANAGER_H_INCLUDED
