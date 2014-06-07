#ifndef TESTGAMESTATE_INCLUDED
#define TESTGAMESTATE_INCLUDED

#include "client/AbstractGameState.h"

#include <position2d.h>

namespace irr {
class IEventReceiver;
namespace scene {
class ISceneManager;
class ICameraSceneNode;
}
namespace gui {
class ICursorControl;
}
}

class TestGameState : public AbstractGameState {
	private:
		TestGameState(const TestGameState&);
		TestGameState& operator=(const TestGameState&);

		/// Aktive Kamera
		irr::scene::ICameraSceneNode* camera;

		/// Speichere die Maus Position für die GUI behandlung
		irr::core::position2di mousePosition;

		/// Cursor Control um die Mouse beeinflussen zu können
		irr::gui::ICursorControl* cursorControl;

		/// Event Receiver für die GUI
		irr::IEventReceiver* subEventReceiver;

		/// Testobjekte
		bool testFlyingObjects:1;

		/// Erstellt die Frei bewegbare Kamera
		void createCamera(irr::scene::ISceneManager* smgr);

		/// Entfernt die Kamera
		void removeCamera();

		/// Setzt den EventReceiver an den Events weitergereicht werden, die nicht von dieser
		/// Klasse behandelt wurden
		void setSubEventReceiver(irr::IEventReceiver* receiver);

	public:
		TestGameState(EngineData& engineData, bool testCreateFlyingObjects = false);
		~TestGameState();

		/// Main Methode des Zustandes, Rückgabe bestimmt den Folgezustand.
		virtual AbstractGameState* run();

		/// Irrlicht Events von GUI, Maus, Tastatur ect...
		bool OnEvent(const irr::SEvent& event);

};

#endif // TESTGAMESTATE_INCLUDED
