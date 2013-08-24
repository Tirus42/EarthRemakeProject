#ifndef INGAMEGUI_H_INCLUDED
#define INGAMEGUI_H_INCLUDED

#include "IGUI.h"

#include <irrlicht.h>

/**
* Ingame GUI
* H�lt alle Zeiger auf die einzelnen GUI Elemente,
* die Ingame verf�gbar sind.
*/
class IngameGUI : public IGUI {
	private:
		irr::gui::IGUIWindow* panel;
		irr::scene::ICameraSceneNode* mainCamera;

		void buildGUI();


	public:
		enum {
			GUI_PANEL,
			GUI_TABBER,
			GUI_TEST_BTN1,
			GUI_TEST_BTN2,
			GUI_TEST_CAMPOS_1,
			GUI_TEST_CAMPOS_2,
			GUI_TEST_CAMPOS_3,
			GUI_TEST_CAMPOS_4,
			GUI_TEST_CAMPOS_PERFORMANCETEST
		};

		IngameGUI(irr::gui::IGUIEnvironment* guiEnv, irr::scene::ICameraSceneNode* mainCam);
		virtual ~IngameGUI();

		// Skaliert die GUI auf eine neue Gr��e (Fenstergr��e)
		void resize(irr::s32 newWidth, irr::s32 newHeight);

		irr::scene::ICameraSceneNode* getMainCamera() const {return mainCamera;}

};


#endif // INGAMEGUI_H_INCLUDED
