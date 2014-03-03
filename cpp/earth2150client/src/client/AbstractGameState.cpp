#include "client/AbstractGameState.h"

#include <irrlicht.h>

AbstractGameState::AbstractGameState(irr::IrrlichtDevice* device) :
	device(device) {

	device->grab();

	// Setzt diese Klasse damit diese nun die Events empfängt
	device->setEventReceiver(this);
}

AbstractGameState::~AbstractGameState() {
	device->drop();
}
