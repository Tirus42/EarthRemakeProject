#ifndef TSEARCHER_H_INCLUDED
#define TSEARCHER_H_INCLUDED


#include <stdint.h>
#include <memory.h>

#define container std::list

/// Hilfsstruktur für abgesuche Wegpunkte
struct Waypoint {
	Waypoint(Waypoint* previus, uint32_t position) :
		searched(false), previus(previus), position(position) {}

	bool searched;
	Waypoint* previus;
	uint32_t position;
};

/**
* Breitensuchen Suchalgorithmus
* Identische Implementierung wie z.Z. im BlitzBasic Client
*/
class TSearcher {
	private:
		TSearcher(const TSearcher&);
		TSearcher& operator=(const TSearcher&);

		size_t mapSize;	// Rohdatengröße
		uint8_t* searchMap;	// Such-Bitmap

		const Map& map;

		/// Suchkarte mit 0-en überschreiben
		inline void clearSearchMap() {
			memset(searchMap, 0, mapSize);
		}

	public:
		TSearcher(const Map& map) :
			mapSize(map.getWidth() * map.getHeight()),
			searchMap(new uint8_t[mapSize]),
			map(map) {};

		~TSearcher() {
			delete searchMap;
		};

		/// Such-funktion
		bool FindWay(uint32_t position1, uint32_t position2, std::list<uint32_t>& path_list) {
			// Zu Begin die Suchkarte leeren
			clearSearchMap();

			// Punktliste
			container<Waypoint> waypoints;

			// Startpunkt in Suchliste eintragen
			waypoints.push_back(Waypoint(NULL, position1));

			uint32_t position = 0;

			const uint16_t mapWidth = map.getWidth();

			while (true) {
				for (container<Waypoint>::iterator i = waypoints.begin(); i != waypoints.end(); ++i) {
					Waypoint& way = (*i);

					if (i == waypoints.begin() && way.searched == true) {
						// Abbrechen, da es keinen Weg gibt
						return false;
					}

					if (way.searched == true)
						break;

					way.searched = true;
					position1 = way.position;

					uint8_t byte = map.getDirections(position1);

					/*
					Untersuche einzelne Richtungen nach Bewegungsmöglichkeit
					*/
					if (byte & Map::NORTH_EAST) {
						position = position1 - mapWidth + 1;

						if (!searchMap[position]) {
							waypoints.push_front(Waypoint(&way, position));

							searchMap[position] = 1;
						}

						if (position == position2)
							goto way_found;
					}

					if (byte & Map::SOUTH_EAST) {
						position = position1 + mapWidth + 1;

						if (!searchMap[position]) {
							waypoints.push_front(Waypoint(&way, position));

							searchMap[position] = 1;
						}

						if (position == position2)
							goto way_found;
					}

					if (byte & Map::SOUTH_WEST) {
						position = position1 + mapWidth - 1;

						if (!searchMap[position]) {
							waypoints.push_front(Waypoint(&way, position));

							searchMap[position] = 1;
						}

						if (position == position2)
							goto way_found;
					}

					if (byte & Map::NORTH_WEST) {
						position = position1 - mapWidth - 1;

						if (!searchMap[position]) {
							waypoints.push_front(Waypoint(&way, position));

							searchMap[position] = 1;
						}

						if (position == position2)
							goto way_found;
					}

					if (byte & Map::NORTH) {
						position = position1 - mapWidth;

						if (!searchMap[position]) {
							waypoints.push_front(Waypoint(&way, position));

							searchMap[position] = 1;
						}

						if (position == position2)
							goto way_found;
					}

					if (byte & Map::EAST) {
						position = position1 + 1;

						if (!searchMap[position]) {
							waypoints.push_front(Waypoint(&way, position));

							searchMap[position] = 1;
						}

						if (position == position2)
							goto way_found;
					}

					if (byte & Map::SOUTH) {
						position = position1 + mapWidth;

						if (!searchMap[position]) {
							waypoints.push_front(Waypoint(&way, position));

							searchMap[position] = 1;
						}

						if (position == position2)
							goto way_found;
					}

					if (byte & Map::WEST) {
						position = position1 - 1;

						if (!searchMap[position]) {
							waypoints.push_front(Waypoint(&way, position));

							searchMap[position] = 1;
						}

						if (position == position2)
							goto way_found;
					}

				}
			}

			// Weg gefunden
			way_found:;

			// Letze eingetragene Position holen
			Waypoint& w = waypoints.front();
			Waypoint* way = &w;

			while (way->previus != NULL) {
				path_list.push_back(way->position);
				way = way->previus;
			}

			return true;
		};
};

#undef container

#endif // TSEARCHER_H_INCLUDED
