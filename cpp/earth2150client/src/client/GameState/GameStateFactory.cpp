#include "client/GameState/GameStateFactory.h"

#include "client/GameState/MainMenu.h"
#include "client/GameState/ProfileSelect.h"

AbstractGameState* GameStateFactory::createGameState(GameState newState, EngineData& engineData) {
	switch (newState) {
		case GS_MainMenu:
			return new MainMenu(engineData);

		case GS_ProfileSelect:
			return new ProfileSelect(engineData);
	}

	return 0;
}
