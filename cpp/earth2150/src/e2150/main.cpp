#include "e2150/main.h"

#include "e2150/MapImpl.h"
#include "e2150/TestServer.h"
#include "e2150/UnitChassis.h"
#include "tf/network.h"
#include <iostream>
#include <cstdlib>

int main(int argc, char *argv[]) {
	MapImpl m(1024, 1024);
	bool result = m.loadHeightMapRAW("map1024x1024.bin");
	std::cout << "Laden der Map " << (result ? "erfolgreich" : "fehlgeschlagen") << std::endl;

	if (!result)
		return EXIT_FAILURE;
	// Komplette Wegkarte berechnen
	m.updateMovementMapWithBorder();

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
	UnitChassis unit_LCUCR3(1, "Crather3", "Crater III", 27777, 1000000, 0, 1000); //Einheit 10s für komplette Drehung
	UnitChassis unit_EDGRUZ(2, "ed_gruz_mk1", "Gruz Baufahrzeug", 27777, 1000000, 0, 800);


	TestServer gameServer(server, m);

	gameServer.addUnitChassis(unit_LCUCR3);
	gameServer.addUnitChassis(unit_EDGRUZ);

	gameServer.run();

	return EXIT_SUCCESS;
}
