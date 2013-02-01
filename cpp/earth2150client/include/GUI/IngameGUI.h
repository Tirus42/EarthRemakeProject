#ifndef INGAMEGUI_H_INCLUDED
#define INGAMEGUI_H_INCLUDED

#include "IGUI.h"

#include <irrlicht.h>

/**
* Ingame GUI
* Hält alle Zeiger auf die einzelnen GUI Elemente,
* die Ingame verfügbar sind.
*/
class IngameGUI : public IGUI {
	private:
		irr::gui::IGUIWindow* panel;

		void buildGUI();


	public:
		enum {
			GUI_PANEL,
			GUI_TEST_BTN1,
			GUI_TEST_BTN2
		};

		IngameGUI(irr::gui::IGUIEnvironment* guiEnv);
		virtual ~IngameGUI();

		// Skaliert die GUI auf eine neue Größe (Fenstergröße)
		void resize(irr::s32 newWidth, irr::s32 newHeight);

};


#endif // INGAMEGUI_H_INCLUDED
