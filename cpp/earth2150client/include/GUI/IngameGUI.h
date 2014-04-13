#ifndef INGAMEGUI_H_INCLUDED
#define INGAMEGUI_H_INCLUDED

#include "IGUI.h"

#include <irrlicht.h>

class ResearchWindow;

/**
* Ingame GUI
* H�lt alle Zeiger auf die einzelnen GUI Elemente,
* die Ingame verf�gbar sind.
*/
class IngameGUI : public IGUI {
	friend class IngameGUIEventReceiver;
	private:
		irr::gui::IGUIWindow* panel;
		irr::scene::ICameraSceneNode* mainCamera;

		ResearchWindow* researchWindow;

		void buildGUI();

		IngameGUI(const IngameGUI&);
		IngameGUI operator=(const IngameGUI&);
	public:
		enum {
			GUI_PANEL,
			GUI_TABBER,
			GUI_TEST_BTN1,
			GUI_TEST_BTN2,
			GUI_TEST_RESEARCH_WINDOW,
			GUI_TEST_CAMPOS_1,
			GUI_TEST_CAMPOS_2,
			GUI_TEST_CAMPOS_3,
			GUI_TEST_CAMPOS_4,
			GUI_TEST_CAMPOS_PERFORMANCETEST,
			GUI_TEST_ALPHA_CONTROL
		};

		IngameGUI(irr::gui::IGUIEnvironment* guiEnv, irr::scene::ICameraSceneNode* mainCam);
		virtual ~IngameGUI();

		/// Skaliert die GUI auf eine neue Gr��e (Fenstergr��e)
		void onResize(const irr::core::dimension2du& newSize);

		/// �ffnet bzw. Erstellt das Forschungsfenster
		void openResearchWindow();

		/// Schlie�t das Forschungsfenster
		void closeResearchWindow();

		irr::scene::ICameraSceneNode* getMainCamera() const {return mainCamera;}

};


#endif // INGAMEGUI_H_INCLUDED
