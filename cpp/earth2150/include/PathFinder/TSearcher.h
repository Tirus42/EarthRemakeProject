#ifndef TSEARCHER_H_INCLUDED
#define TSEARCHER_H_INCLUDED

#include "Map/Map.h"

#include <stdint.h>
#include <memory.h>

#define container std::vector

/// Hilfsstruktur für abgesuche Wegpunkte
struct Waypoint {
	Waypoint(uint32_t previus, uint32_t position) :
		searched(false), previus(previus), position(position) {}

	bool searched;
	uint32_t previus;
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

		static const uint32_t START_WAYPOINT = 0xFFFFFFFF;

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

			// Genügend Speicher Reservieren, damit es nicht zu unnötigen reallocs kommt
			waypoints.reserve(map.getWidth() * map.getHeight());

			// Startpunkt in Suchliste eintragen
			waypoints.push_back(Waypoint(START_WAYPOINT, position1));

			uint32_t position = 0;

			const uint16_t mapWidth = map.getWidth();

			while (true) {
				for (int32_t i = waypoints.size() - 1; i >= 0; --i) {
					int32_t currentEnd = waypoints.size() - 1;

					Waypoint& way = waypoints[i];

					if (i == currentEnd && way.searched == true) {
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
							waypoints.push_back(Waypoint(i, position));

							searchMap[position] = 1;
						}

						if (position == position2)
							goto way_found;
					}

					if (byte & Map::SOUTH_EAST) {
						position = position1 + mapWidth + 1;

						if (!searchMap[position]) {
							waypoints.push_back(Waypoint(i, position));

							searchMap[position] = 1;
						}

						if (position == position2)
							goto way_found;
					}

					if (byte & Map::SOUTH_WEST) {
						position = position1 + mapWidth - 1;

						if (!searchMap[position]) {
							waypoints.push_back(Waypoint(i, position));

							searchMap[position] = 1;
						}

						if (position == position2)
							goto way_found;
					}

					if (byte & Map::NORTH_WEST) {
						position = position1 - mapWidth - 1;

						if (!searchMap[position]) {
							waypoints.push_back(Waypoint(i, position));

							searchMap[position] = 1;
						}

						if (position == position2)
							goto way_found;
					}

					if (byte & Map::NORTH) {
						position = position1 - mapWidth;

						if (!searchMap[position]) {
							waypoints.push_back(Waypoint(i, position));

							searchMap[position] = 1;
						}

						if (position == position2)
							goto way_found;
					}

					if (byte & Map::EAST) {
						position = position1 + 1;

						if (!searchMap[position]) {
							waypoints.push_back(Waypoint(i, position));

							searchMap[position] = 1;
						}

						if (position == position2)
							goto way_found;
					}

					if (byte & Map::SOUTH) {
						position = position1 + mapWidth;

						if (!searchMap[position]) {
							waypoints.push_back(Waypoint(i, position));

							searchMap[position] = 1;
						}

						if (position == position2)
							goto way_found;
					}

					if (byte & Map::WEST) {
						position = position1 - 1;

						if (!searchMap[position]) {
							waypoints.push_back(Waypoint(i, position));

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
			Waypoint& w = waypoints.back();
			Waypoint* way = &w;

			while (way->previus != START_WAYPOINT) {
				path_list.push_back(way->position);
				way = &waypoints[way->previus];
			}

			return true;
		};
};

#undef container

#endif // TSEARCHER_H_INCLUDED
