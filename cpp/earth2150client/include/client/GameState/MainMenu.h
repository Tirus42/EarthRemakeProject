#ifndef MAINMENU_H_INCLUDED
#define MAINMENU_H_INCLUDED

#include "client/AbstractGameState.h"

namespace irr {
namespace gui {
class IGUIElement;
class IGUIEnvironment;
}
}

/**
* Erstellt das Hauptmenü und Verwaltet die Nutzereingaben in diesem.
*/
class MainMenu : public AbstractGameState {
	private:
		MainMenu(const MainMenu&);
		MainMenu& operator=(const MainMenu&);

		/// Enumerator für die GUI Elemente des Menüs
		enum {
			GUI_WINDOW_MAIN,
			GUI_BUTTON_NEWGME,
			GUI_BUTTON_NETWORKGAME,
			GUI_BUTTON_QUIT,
			GUI_COUNT
		};

		/// Array der GUI Elemente.
		irr::gui::IGUIElement* guiElements[GUI_COUNT];

		/// Erstellt die GUI Elemente
		void createGUI(irr::gui::IGUIEnvironment * guienv);

		/// Entfernt die GUI Elemente
		void removeGUI();

	public:
		MainMenu(EngineData& engineData);

		/// Main Methode des Zustandes, Rückgabe bestimmt den Folgezustand.
		virtual AbstractGameState* run();

		/// Irrlicht Events von GUI, Maus, Tastatur ect...
		bool OnEvent(const irr::SEvent& event);
};


#endif // MAINMENU_H_INCLUDED
