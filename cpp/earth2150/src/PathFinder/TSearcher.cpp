#include "PathFinder/TSearcher.h"

#include <memory.h>

#define container std::vector


TSearcher::TSearcher(const Map& map) :
	mapSize(map.getWidth() * map.getHeight()),
	searchMap(new uint8_t[mapSize]),
	map(map) {};

TSearcher::~TSearcher() {
	delete searchMap;
}

/// Hilfsstruktur für abgesuche Wegpunkte
struct Waypoint {
	Waypoint(uint32_t previus, uint32_t position) :
		previus(previus), position(position) {}

	/// Index Eintrag des Vorgängers
	uint32_t previus;

	/// Position des betrachteten Feldes
	uint32_t position;
};

bool TSearcher::FindWay(uint32_t position1, uint32_t position2, std::list<uint32_t>& path_list) {
	// Zu Begin die Suchkarte leeren
	clearSearchMap();

	/*
	Punktliste
	An das Ende werden weitere "Offene" Suchpunkte eingetragen.
	Alle vornedran sind bereits fertig abgesucht und werden nur noch für die
	bestimmung des fertigen Weges benötigt.
	*/
	container<Waypoint> waypoints;

	int32_t waypoints_finishOffset = -1;

	// Genügend Speicher Reservieren, damit es nicht zu unnötigen reallocs kommt
	waypoints.reserve(map.getWidth() * map.getHeight());

	// Startpunkt in Suchliste eintragen
	waypoints.push_back(Waypoint(START_WAYPOINT, position1));

	uint32_t position = 0;

	const uint16_t mapWidth = map.getWidth();


	while (true) {
		// Das nächste Ende für die Offen/Abgeschlossenliste merken
		int32_t nextFinishOffset = waypoints.size() - 1;

		for (int32_t i = waypoints.size() - 1; i >= 0; --i) {
			int32_t currentEnd = waypoints.size() - 1;

			// Hole aktuell zu untersuchende Position
			position1 = waypoints[i].position;

			// Prüfe ob bereits alle offenen Wegpunkte untersucht wurden
			if (i == currentEnd && i == waypoints_finishOffset) {
				// Abbrechen, da es keinen Weg gibt
				return false;
			}

			if (i == waypoints_finishOffset)
				break;


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

		// Das Ende der Offen/Geschlossen Liste verschieben, da alle Offenen abgesucht, dafür neue eingetragen wurden
		waypoints_finishOffset = nextFinishOffset;
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
}

void TSearcher::clearSearchMap() {
	memset(searchMap, 0, mapSize);
}
