#ifndef INGAMEGUI_H_INCLUDED
#define INGAMEGUI_H_INCLUDED

#include "BasicGameInterface.h"

#include <irrlicht.h>

class ResearchWindow;

/**
* Ingame GUI
* Hält alle Zeiger auf die einzelnen GUI Elemente,
* die Ingame verfügbar sind.
*/
class IngameGUI : public BasicGameInterface {
	private:
		irr::gui::IGUITabControl* testTabControl;

		irr::gui::IGUIScrollBar* testAlphaControl;

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

		IngameGUI(irr::gui::IGUIEnvironment* guiEnv, irr::IrrlichtDevice* device, irr::scene::ICameraSceneNode* mainCam);
		virtual ~IngameGUI();

		/// Skaliert die GUI auf eine neue Größe (Fenstergröße)
		void onResize(const irr::core::dimension2du& newSize);

		/// Öffnet bzw. Erstellt das Forschungsfenster
		void openResearchWindow();

		/// Schließt das Forschungsfenster
		void closeResearchWindow();

		irr::scene::ICameraSceneNode* getMainCamera() const {return mainCamera;}

		virtual bool OnEvent(const irr::SEvent& event);

};


#endif // INGAMEGUI_H_INCLUDED
