#ifndef TESTGAMESTATE_INCLUDED
#define TESTGAMESTATE_INCLUDED

#include "client/AbstractGameState.h"

class TestGameState : public AbstractGameState {
	private:

	public:
		TestGameState(irr::IrrlichtDevice* device);
		~TestGameState();

		/// Main Methode des Zustandes, Rückgabe bestimmt den Folgezustand.
		virtual AbstractGameState* run();

		/// Irrlicht Events von GUI, Maus, Tastatur ect...
		bool OnEvent(const irr::SEvent& event);

};

#endif // TESTGAMESTATE_INCLUDED
