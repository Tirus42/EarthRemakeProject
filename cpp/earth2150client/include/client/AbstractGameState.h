#ifndef ABSTRACTGAMESTATE_H_INCLUDED
#define ABSTRACTGAMESTATE_H_INCLUDED

#include <stdint.h>

#include <IEventReceiver.h>

class EngineData;

/**
* Stellt den Zustand der Anwendung dar.
* Zustände können Hauptmenü, Ladebildschirm, in Spiel ect. sein.
* Die run() Methode ist die "main" des jeweiligen Zustandes.
* In der OnEvent() werden sämtliche Externe Inputs von GUI, Maus und Tastatur empfangen (siehe Irrlicht Doku).
*/
class AbstractGameState : public irr::IEventReceiver {
	private:
		AbstractGameState (const AbstractGameState&);
		AbstractGameState& operator= (const AbstractGameState&);

		/// GameState in den gewechselt werden soll
		AbstractGameState* changeToGameState;

		/// Wird gesetzt wenn ein folgender GameState gesetzt wurde
		bool nextGameState;
	protected:
		/// Engine Daten mit Config und Device Ptr
		EngineData& engineData;

		void changeGameState(AbstractGameState* newState);

		/// Gibt true zurück wenn ein Folgezustand gesetzt wurde
		bool shouldChangeGameState() const {
			return nextGameState;
		}

		/// Gibt das Objekt des folgezustandes zurück
		AbstractGameState* getNextGameState() const {
			return changeToGameState;
		}

	public:
		AbstractGameState(EngineData& engineData);
		virtual ~AbstractGameState();

		/// Main Methode des Zustandes, Rückgabe bestimmt den Folgezustand.
		virtual AbstractGameState* run() = 0;

		/// Irrlicht Events von GUI, Maus, Tastatur ect...
		virtual bool OnEvent(const irr::SEvent& event) = 0;

};

#endif // ABSTRACTGAMESTATE_H_INCLUDED
