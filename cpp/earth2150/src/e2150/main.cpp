#include "e2150/main.h"

#include "e2150/Map.h"
#include "e2150/TestServer.h"
#include "e2150/UnitChassis.h"
#include "tf/network.h"
#include <iostream>
#include <cstdlib>

int main(int argc, char *argv[]) {
	Map* m = new Map(1024, 1024);
	bool result = m->loadHeightMapRAW("map1024x1024.bin");

	std::cout << "Laden der Map " << (result ? "erfolgreich" : "fehlgeschlagen") << std::endl;

	m->updateMovementMapWithBorder();

	InitNetwork();

	int32_t server = CreateTCPServer(2000, true);

	if (server == 0) {
		std::cout << "Konnte TCP-Server nicht starten, Port belegt?" << std::endl;
		return EXIT_FAILURE;
	}

	TestServer gameServer(server);

	UnitChassis unit_LCUCR3("LCUCR3", "Crater III", 27777, 1000000); //Einheit 10s für komplette Drehung
	UnitChassis unit_EDGRUZ("ed_gruz_mk1", "Gruz Baufahrzeug", 27777, 1000000);

	gameServer.run(m);

	return EXIT_SUCCESS;
}
