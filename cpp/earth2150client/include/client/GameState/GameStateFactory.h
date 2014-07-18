#ifndef GAMESTATEFACTORY_H_INCLUDED
#define GAMESTATEFACTORY_H_INCLUDED

class AbstractGameState;
class EngineData;

/**
* Factory Klasse um GameStates zu erzeugen.
*/
class GameStateFactory {
	public:
		enum GameState {
			GS_MainMenu,
			GS_ProfileSelect,
		};

		static AbstractGameState* createGameState(GameState newState, EngineData& engineData);
};


#endif // GAMESTATEFACTORY_H_INCLUDED
