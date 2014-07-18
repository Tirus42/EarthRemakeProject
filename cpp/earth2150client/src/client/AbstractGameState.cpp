#include "client/AbstractGameState.h"

#include "client/EngineData.h"

#include <irrlicht.h>

AbstractGameState::AbstractGameState(EngineData& engineData) :
	changeToGameState(0),
	nextGameState(false),
	engineData(engineData) {

	// Setzt diese Klasse damit diese nun die Events empfängt
	if (engineData.getIrrlichtDevice())
		engineData.getIrrlichtDevice()->setEventReceiver(this);
}

AbstractGameState::~AbstractGameState() {
}

void AbstractGameState::changeGameState(AbstractGameState* newState) {
	changeToGameState = newState;
	nextGameState = true;
}
