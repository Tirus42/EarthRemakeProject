#include "client/AbstractGameState.h"

#include "client/EngineData.h"

#include <irrlicht.h>

AbstractGameState::AbstractGameState(EngineData& engineData) :
	engineData(engineData) {

	// Setzt diese Klasse damit diese nun die Events empfängt
	if (engineData.getIrrlichtDevice())
		engineData.getIrrlichtDevice()->setEventReceiver(this);
}

AbstractGameState::~AbstractGameState() {
}
