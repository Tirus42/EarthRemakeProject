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

	protected:
		/// Engine Daten mit Config und Device Ptr
		EngineData& engineData;

	public:
		AbstractGameState(EngineData& engineData);
		virtual ~AbstractGameState();

		/// Main Methode des Zustandes, Rückgabe bestimmt den Folgezustand.
		virtual AbstractGameState* run() = 0;

		/// Irrlicht Events von GUI, Maus, Tastatur ect...
		virtual bool OnEvent(const irr::SEvent& event) = 0;

};

#endif // ABSTRACTGAMESTATE_H_INCLUDED
