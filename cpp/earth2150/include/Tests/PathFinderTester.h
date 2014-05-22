#ifndef PATHFINDERTESTER_H_INCLUDED
#define PATHFINDERTESTER_H_INCLUDED

#include <vector>
#include <string>

#include "Map/MapPosition.h"

class Map;
class INavigator;

/**
* Klasse welche die Performance und die gültigkeit von PathFindern
* durchprobiert und die Ergebnisse ausgibt.
* Kann als Benchmark zwischen den Suchalgorithmen genutzt werden.
* Es werden alle eingetragenenen Algorithmen auf den angegebenen Wegen getestet,
* die Dauer des Suchaufrufs, sowie deren Rückgabe ob Weg gefunden sowie dessen Länge
* wird auf der Konsole ausgegeben.
*/
class PathFinderTester {
	private:
		const Map& map;

		std::vector<std::pair<INavigator*, std::string> > pathFinders;
		std::vector<std::pair<MapPosition, MapPosition> > searchPoints;
		std::vector<std::string> searchNames;

		void searchPath(INavigator* navigator, const MapPosition& p1, const MapPosition& p2);

		PathFinderTester(const PathFinderTester&);
		PathFinderTester operator=(const PathFinderTester&);

	public:
		/// Erstellen einen PathFinderTester für die angegebene Karte
		PathFinderTester(const Map& map);

		/// Fügt einen PathFinderNavigator zu den zu Testenden Suchalgorithmen hinzu
		void registerPathFinderNavigator(INavigator* navigator, const char* name);

		/// Fügt zwei Wegpunkte zwischen denen der Weg gesucht werden soll zu der Testliste hinzu
		void addSearchPoints(const MapPosition& p1, const MapPosition& p2, const char* description);

		/// Führt alle Tests aus, gibt die Ergebnisse auf stdout aus
		void runTests(uint32_t countRuns = 5) const;
};


#endif // PATHFINDERTESTER_H_INCLUDED
