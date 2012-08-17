#include "e2150/main.h"

#include "e2150/Map.h"
#include "e2150/UnitChassis.h"
#include "e2150/TestServer.h"

#include "tf/network.h"
#include <iostream>
#include <cstdlib>

using std::cout;
using std::endl;

int main(int argc, char *argv[]) {

	Map* m = new Map(1024, 1024);
	bool result = m->loadHeightMapRAW("map1024x1024.bin");

	cout << "Laden der Map " << (result ? "erfolgreich" : "fehlgeschlagen") << endl;

	m->updateMovementMapWithBorder();

	InitNetwork();

	int32_t server = CreateTCPServer(2000, true);

	if (server == 0) {
		cout << "Konnte TCP-Server nicht starten, Port belegt?" << endl;
		return EXIT_FAILURE;
	}

	TestServer gameServer(server);

	UnitChassis unit_LCUCR3(1, "LCUCR3", 27777, 1000000);		//Einheit braucht zum Test 10 seks für eine komplette Drehung
	UnitChassis unit_EDGRUZ(2, "ed_gruz_mk1", 27777, 1000000);

	gameServer.run(m);

	return EXIT_SUCCESS;
}
