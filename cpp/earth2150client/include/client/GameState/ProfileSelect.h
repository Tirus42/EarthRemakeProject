#ifndef PROFILESELECT_H_INCLUDED
#define PROFILESELECT_H_INCLUDED

#include "client/AbstractGameState.h"

namespace irr {
namespace gui {
class IGUIElement;
class IGUIEnvironment;
}
}

class ProfileSelect : public AbstractGameState {
	private:
		ProfileSelect(const ProfileSelect&);
		ProfileSelect& operator=(const ProfileSelect&);

		/// Enumerator für die GUI Elemente des Menüs
		enum {
			GUI_WINDOW_MAIN,
			GUI_COMBOBOX_USERSELECT,
			GUI_BUTTON_NEWPROFILE,
			GUI_BUTTON_USEPROFILE,
			GUI_BUTTON_BACK,
			GUI_COUNT
		};

		/// Array der GUI Elemente.
		irr::gui::IGUIElement* guiElements[GUI_COUNT];

		/// Erstellt die GUI Elemente
		void createGUI(irr::gui::IGUIEnvironment * guienv);

		/// Entfernt die GUI Elemente
		void removeGUI();

	public:
		ProfileSelect(EngineData& engineData);

		/// Main Methode des Zustandes, Rückgabe bestimmt den Folgezustand.
		virtual AbstractGameState* run();

		/// Irrlicht Events von GUI, Maus, Tastatur ect...
		bool OnEvent(const irr::SEvent& event);
};



#endif // PROFILESELECT_H_INCLUDED
