#include "e2150/main.h"

#include "Map/Map.h"
#include "Network/TestServer.h"
#include "Unit/UnitChassis.h"
#include "tf/network.h"
#include <iostream>
#include <cstdlib>
#include <unistd.h>	// für getopt()

#include "Tests/PathFinderTester.h"
#include "PathFinder/NavigatorFactory.h"

void benchmark(const Map& map) {
	PathFinderTester pt(map);

	pt.registerPathFinderNavigator(NavigatorFactory::getNavigator(map, NavigatorFactory::NAVIGATOR_ASTAR), "AStar");
	pt.registerPathFinderNavigator(NavigatorFactory::getNavigator(map, NavigatorFactory::NAVIGATOR_BSEARCH), "Breitensuche");
	pt.registerPathFinderNavigator(NavigatorFactory::getNavigator(map, NavigatorFactory::NAVIGATOR_JPS), "Jump Point Search");

	pt.addSearchPoints(MapPosition(10, 10), MapPosition(20, 10), "Kurze Suche ohne Hindernis");
	pt.addSearchPoints(MapPosition(20, 500), MapPosition(1015, 500), "Horizontal ueber ganze Karte (ohne Hindernis)");
	pt.addSearchPoints(MapPosition(1, 1), MapPosition(1021, 1021), "Diagonal ueber ganze Karte");
	pt.addSearchPoints(MapPosition(148, 20), MapPosition(302, 238), "GroundHill Diagonal");
	pt.addSearchPoints(MapPosition(225, 24), MapPosition(225, 234), "GroundHill Vertikal");
	pt.addSearchPoints(MapPosition(10, 20), MapPosition(25, 56), "Berg in Taal (links oben)");
	pt.addSearchPoints(MapPosition(772, 115), MapPosition(897, 127), "Spiralberg (rechts oben) von unten nach oben");
	pt.addSearchPoints(MapPosition(790, 227), MapPosition(963, 430), "Durch Labyrinth (rechts oben)");
	pt.addSearchPoints(MapPosition(774, 433), MapPosition(718, 371), "Durch Spirale ins innere (rechts oben)");
	pt.addSearchPoints(MapPosition(719, 371), MapPosition(903, 141), "Aus Spirale auf Spiralberg");
	pt.addSearchPoints(MapPosition(903, 141), MapPosition(719, 371), "Von Spiralberg in Spirale");
	pt.addSearchPoints(MapPosition(300, 957), MapPosition(399, 983), "Karte \"Teufelsschucht\" einmal durch (links unten)");
	pt.addSearchPoints(MapPosition(22, 957), MapPosition(23, 954), "Felder nahe aber grosser Umweg (small Worst Case)");
	pt.addSearchPoints(MapPosition(26, 20), MapPosition(22, 20), "Suche ohne mgl. Weg (beschraenkt)");
	pt.addSearchPoints(MapPosition(22, 20), MapPosition(26, 20), "Suche ohne mgl. Weg (viel Fl.) (Worst Case!)");

	pt.runTests(10);
}

int main(int argc, char *argv[]) {

	bool runBenchmark = false;
	{
		int c;

		while ((c = getopt(argc, argv, "b")) != -1) {
			if (c == 'b')
				runBenchmark = true;
		}
	}

	Map m(1024, 1024);

	bool result = m.loadHeightMapRAW("GameData/map1024x1024.bin");

	std::cout << "Laden der Map " << (result ? "erfolgreich" : "fehlgeschlagen") << std::endl;

	if (!result)
		return EXIT_FAILURE;

	// Komplette Wegkarte berechnen
	m.updateMovementMap();

	if (runBenchmark) {
		// m.exportPassablesToBMP("map.bmp");
		benchmark(m);
		return 0;
	}

	m.addSpawnPoint(MapPosition(5, 5));
	m.addSpawnPoint(MapPosition(20, 10));
	m.addSpawnPoint(MapPosition(5, 20));
	m.addSpawnPoint(MapPosition(25, 55));	//Innerhalb der Schlucht

	InitNetwork();

	int32_t server = CreateTCPServer(2000, true);

	if (server == 0) {
		std::cout << "Konnte TCP-Server nicht starten, Port belegt?" << std::endl;
		return EXIT_FAILURE;
	}

	//Themporär an BB Prog anpassen (dort die unit/test1.txt)
	UnitChassis unit_LCUCR3(1, "Crather3", "Crater III", 27, 1000, 0, 1000); //Einheit 10s für komplette Drehung
	UnitChassis unit_EDGRUZ(2, "ed_gruz_mk1", "Gruz Baufahrzeug", 27, 1000, 0, 800);
	UnitChassis unit_TESTBOX(3, "testbox", "Testbox", 3, 250, 0, 10);


	TestServer gameServer(server, m);

	gameServer.addUnitChassis(unit_LCUCR3);
	gameServer.addUnitChassis(unit_EDGRUZ);
	gameServer.addUnitChassis(unit_TESTBOX);

	gameServer.run();

	return EXIT_SUCCESS;
}
