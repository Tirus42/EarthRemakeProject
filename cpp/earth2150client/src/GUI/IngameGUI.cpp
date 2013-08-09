#include "GUI/IngameGUI.h"

using namespace irr;

IngameGUI::IngameGUI(gui::IGUIEnvironment* guiEnv, scene::ICameraSceneNode* mainCam) :
	IGUI(guiEnv),
	mainCamera(mainCam) {

	mainCamera->grab();

	buildGUI();
}

IngameGUI::~IngameGUI() {
	mainCamera->drop();
}

void IngameGUI::buildGUI() {
	// Untere Panele erstellen
	panel = env->addWindow(core::rect<s32>(0, 568, 1024, 768), false, L"TestFenster", 0, GUI_PANEL);

	// Tielleiste und Close Button nicht zeichen/nutzen
	panel->setDrawTitlebar(false);
	panel->getCloseButton()->setVisible(false);

	// Fenster nicht verschiebbar machen
	panel->setDraggable(false);

	// Einige Test Elemente eingfügen
	env->addButton(core::rect<s32>(200, 20, 300, 40), panel, GUI_TEST_BTN1, L"Test Button 1");
	env->addButton(core::rect<s32>(200, 50, 300, 70), panel, GUI_TEST_BTN2, L"Test Button 2");

	env->addStaticText(L"Mit der Rechten Maustaste kann die Kamera von der Maus geloest werden, und damit diese GUI bedient werden",
						 core::rect<s32>(320, 20, 500, 70), true, true, panel, -1, true);

}

void IngameGUI::resize(s32 newWidth, s32 newHeight) {
	panel->setRelativePosition(core::rect<s32>(0, newHeight - 200, newWidth, newHeight));
}
