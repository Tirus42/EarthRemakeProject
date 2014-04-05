#include "config/ClientConfig.h"

#include "client/GameState/MainMenu.h"
#include "client/GameState/TestGameState.h"

#include <unistd.h>	// für getopt()
#include <irrlicht.h>

#include "tf/time.h"


using namespace irr;

/*
There are 5 sub namespaces in the Irrlicht Engine. Take a look
at them, you can read a detailed description of them in the
documentation by clicking on the top menu item 'Namespace List'
or using this link: http://irrlicht.sourceforge.net/docu/namespaces.html.
Like the irr Namespace, we do not want these 5 sub namespaces now,
to keep this example simple. Hence we tell the compiler again
that we do not want always to write their names:
*/
using namespace core;
using namespace video;
using namespace io;
using namespace gui;

/// Läd die Konfiguration des Clienten, erstellt die Datei falls noch nicht vorhanden
void loadConfig(ClientConfig& config) {
	// Erzeuge ein Irrlicht Null-Device um zugriff auf das Dateisystem zu erhalten
	IrrlichtDevice *device = createDevice(EDT_NULL);

	IXMLReader *configReader = device->getFileSystem()->createXMLReader("clientconfig.xml");

	// Wenn Datei lesbar, dann Config lesen
	if (configReader) {
		config.loadXMLFile(configReader);

		configReader->drop();
	} else {
		// Standardwerte setzten und Config Datei schreiben
		config.setDefaultValues();

		IXMLWriter *configWriter = device->getFileSystem()->createXMLWriter("clientconfig.xml");

		if (configWriter) {
			config.saveXMLFile(configWriter);

			configWriter->drop();
		}
	}

	device->drop();
}

int main(int argc, char** argv) {

	bool skipMenu = false;

	{
		int c;

		while ((c = getopt(argc, argv, "s")) != -1) {
			if (c == 's')
				skipMenu = true;
		}
	}

	ClientConfig config;

	// Config Datei Laden wenn vorhanden, andernfalls mit Standardwerten erstellen.
	loadConfig(config);

	// Hochauflösenden Timer initiieren (für exakte Zeitmessungen)
	InitHighResolutionTimer();

    IrrlichtDevice *device = createDeviceEx(config.Parameter());

	// Wenn Device nicht gestartet werden konnte, dann abbrechen
	if (!device)
		return EXIT_FAILURE;

	IVideoDriver* driver = device->getVideoDriver();

	// Mache Fenster frei Skalierbar
	device->setResizable(true);

	core::stringw caption("Earth 2150 Remake Projekt - ");
	caption += driver->getName();

	device->setWindowCaption(caption.c_str());

	// Erstelle das Hauptmenü
	AbstractGameState* currentGameState = 0;

	if (skipMenu) {
		currentGameState = new TestGameState(device);
	} else {
		currentGameState = new MainMenu(device);
	}

	// Alles weitere übernimmt der aktuelle GameState
	while (currentGameState != 0) {
		AbstractGameState* nextGameState = currentGameState->run();
		delete currentGameState;

		currentGameState = nextGameState;
	}

    device->drop();

    return 0;
}

