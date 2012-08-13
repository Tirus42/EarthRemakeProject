#include "e2150/main.h"

#include "e2150/Map.h"
#include "e2150/TestServer.h"

#include "tf/network.h"
#include <iostream>
#include <cstdlib>

using namespace std;

int main(int argc, char *argv[]) {
	std::cout << "Hello world!" << std::endl;

	Map* m = new Map(1024, 1024);

	bool result = m->loadHeightMapRAW("map1024x1024.bin");

	cout << "Laden der Map: " << result << endl;

	InitNetwork();

	int32_t server = CreateTCPServer(2000, true);

	if (server == 0) {
		cout << "Konnte TCP-Server nicht starten, Port belegt?" << endl;
		return EXIT_FAILURE;
	}

	TestServer gameServer(server);

	gameServer.run(m);


	return EXIT_SUCCESS;
}
