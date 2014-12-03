#include "PathFinder/TSearcher.h"

#include "Map/Map.h"

#include <memory.h>
#include <cassert>

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
	Waypoint(uint32_t position) :
		position(position) {}

	/// Position des betrachteten Feldes
	uint32_t position;
};

bool TSearcher::FindWay(uint32_t position1, uint32_t position2, std::list<uint32_t>& path_list) {
	const uint32_t startPosition = position1;

	// Zu Begin die Suchkarte leeren
	clearSearchMap();

	// Liste der aktuell zu prüfenden Wegpunkte
	container<Waypoint> currentPoints;

	// Liste der in der nächsten iteration zu prüfenden Wegpunkte
	container<Waypoint> nextPoints;

	// Genügend Speicher Reservieren, damit es nicht zu unnötigen reallocs kommt
	// 2x Kartenbreite ist ein guter Wert der fast nie überschritten wird
	currentPoints.reserve(map.getWidth() * 2);
	nextPoints.reserve(map.getWidth() * 2);

	// Startpunkt in Suchliste eintragen
	nextPoints.push_back(Waypoint(position1));

	uint32_t position = 0;

	const uint16_t mapWidth = map.getWidth();

	while (true) {

		// Listen vertauschen und Alte leeren
		currentPoints.swap(nextPoints);
		nextPoints.clear();

		// Wenn nichts mehr in Liste, dann gibt es keine weiteren möglichen Wege -> Abbruch
		if (currentPoints.size() == 0)
			return false;

		for (int32_t i = currentPoints.size() - 1; i >= 0; --i) {
			// Hole aktuell zu untersuchende Position
			position1 = currentPoints[i].position;

			uint8_t byte = map.getDirections(position1);

			/*
			Untersuche einzelne Richtungen nach Bewegungsmöglichkeit
			*/
			if (byte & Map::NORTH_EAST) {
				position = position1 - mapWidth + 1;

				if (searchMap[position] == 0xFF) {
					nextPoints.push_back(Waypoint(position));

					searchMap[position] = Map::SOUTH_WEST;
				}

				if (position == position2)
					goto way_found;
			}

			if (byte & Map::SOUTH_EAST) {
				position = position1 + mapWidth + 1;

				if (searchMap[position] == 0xFF) {
					nextPoints.push_back(Waypoint(position));

					searchMap[position] = Map::NORTH_WEST;
				}

				if (position == position2)
					goto way_found;
			}

			if (byte & Map::SOUTH_WEST) {
				position = position1 + mapWidth - 1;

				if (searchMap[position] == 0xFF) {
					nextPoints.push_back(Waypoint(position));

					searchMap[position] = Map::NORTH_EAST;
				}

				if (position == position2)
					goto way_found;
			}

			if (byte & Map::NORTH_WEST) {
				position = position1 - mapWidth - 1;

				if (searchMap[position] == 0xFF) {
					nextPoints.push_back(Waypoint(position));

					searchMap[position] = Map::SOUTH_EAST;
				}

				if (position == position2)
					goto way_found;
			}

			if (byte & Map::NORTH) {
				position = position1 - mapWidth;

				if (searchMap[position] == 0xFF) {
					nextPoints.push_back(Waypoint(position));

					searchMap[position] = Map::SOUTH;
				}

				if (position == position2)
					goto way_found;
			}

			if (byte & Map::EAST) {
				position = position1 + 1;

				if (searchMap[position] == 0xFF) {
					nextPoints.push_back(Waypoint(position));

					searchMap[position] = Map::WEST;
				}

				if (position == position2)
					goto way_found;
			}

			if (byte & Map::SOUTH) {
				position = position1 + mapWidth;

				if (searchMap[position] == 0xFF) {
					nextPoints.push_back(Waypoint(position));

					searchMap[position] = Map::NORTH;
				}

				if (position == position2)
					goto way_found;
			}

			if (byte & Map::WEST) {
				position = position1 - 1;

				if (searchMap[position] == 0xFF) {
					nextPoints.push_back(Waypoint(position));

					searchMap[position] = Map::EAST;
				}

				if (position == position2)
					goto way_found;
			}

		}

	}

	// Weg gefunden
	way_found:;

	// Von Ziel zum Start gehen
	uint32_t mPos = position2;

	// Ergebnis der Breitensuche Rückwerts ablaufen und in Ausgabe eintragen
	while (mPos != startPosition) {
		path_list.push_back(mPos);

		mPos = map.addDirection(mPos, (Map::WaymapDirection)searchMap[mPos]);
	}

	return true;
}

void TSearcher::clearSearchMap() {
	memset(searchMap, 0xFF, mapSize);
}
