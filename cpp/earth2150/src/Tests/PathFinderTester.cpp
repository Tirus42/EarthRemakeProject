#include "Tests/PathFinderTester.h"

#include "Map/Map.h"
#include "PathFinder/INavigator.h"

#include "tf/time.h"
#include <stdio.h>

PathFinderTester::PathFinderTester(const Map& map) :
	map(map),
	pathFinders(),
	searchPoints(),
	searchNames() {
}

void PathFinderTester::registerPathFinderNavigator(INavigator* navigator, const char* name) {
	pathFinders.push_back(std::pair<INavigator*, std::string>(navigator, std::string(name)));
}

void PathFinderTester::addSearchPoints(const MapPosition& p1, const MapPosition& p2, const char* description) {
	searchPoints.push_back(std::pair<MapPosition, MapPosition>(p1, p2));
	searchNames.push_back(std::string(description));
}

void PathFinderTester::runTests(uint32_t countRuns) const {
	printf("Teste PathFinder Algorithmen (je %u durchlaeufe)\n", countRuns);
	printf("Zeitangaben in us (Mikrosekunden)\n");

	if (!InitHighResolutionTimer()) {
		printf("Konnte HighResolutionTimer nicht erstellen!\n");
		return;
	}

	// Iteriere für alle Suchpaare
	for (uint32_t i = 0; i < searchPoints.size(); ++i) {

		const MapPosition& p1 = searchPoints[i].first;
		const MapPosition& p2 = searchPoints[i].second;

		if (!p1.isValidOnMap(map) || !p2.isValidOnMap(map)) {
			printf("Weg von (%u, %u) -> (%u, %u) ist nicht gueltig!\n", p1.getX(), p1.getY(), p2.getX(), p2.getY());
			continue;
		}

		uint32_t pos1 = map.position(p1.getX(), p1.getY());
		uint32_t pos2 = map.position(p2.getX(), p2.getY());

		printf("Test: %s (%u, %u) -> (%u, %u)\n", searchNames[i].c_str(), p1.getX(), p1.getY(), p2.getX(), p2.getY());
		printf("min\tmax\tavg\tfields\tfound\n");

		for (uint32_t nav = 0; nav < pathFinders.size(); ++nav) {
			INavigator* navigator = pathFinders[nav].first;

			// Variablen für Zeitmessung
			uint64_t sTime, eTime;
			uint64_t minTime = -1;
			uint64_t maxTime = 0;
			uint64_t sumTime = 0;

			bool wayFound = false;
			std::list<uint32_t> path_list;

			for (uint32_t j = 0; j < countRuns; ++j) {
				path_list.clear();

				// Zeit messen bei der Wegsuche
				HighResolutionTime(&sTime);
				wayFound = navigator->getPath(pos1, pos2, path_list);
				HighResolutionTime(&eTime);

				uint64_t diff = HighResolutionDiffNanoSec(sTime, eTime);

				minTime = std::min(minTime, diff);
				maxTime = std::max(maxTime, diff);

				sumTime += diff;
			}

			uint64_t avgTime = sumTime / countRuns;

			// Ergebnis ausgeben (Nanosekunden / 1000 -> Mikrosekunden)
			printf("%u\t%u\t%u\t%u\t%u\t(%s)\n", uint32_t(minTime / 1000), uint32_t(maxTime / 1000), uint32_t(avgTime / 1000), uint32_t(path_list.size()), uint32_t(wayFound), pathFinders[nav].second.c_str());

		}

	}
}
