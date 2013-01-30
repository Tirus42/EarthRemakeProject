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
			GUI_CLOSE
		};

		IngameGUI(irr::gui::IGUIEnvironment* guiEnv);
		virtual ~IngameGUI();

};


#endif // INGAMEGUI_H_INCLUDED
