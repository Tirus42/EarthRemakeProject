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

	// Tab Control mit 3 Tabs
	gui::IGUITabControl* tab = env->addTabControl(core::rect<s32>(200, 10, 700, 190), panel, true, true, GUI_TABBER);

	// Tab Leisten Breite und Höhe festlegen
	tab->setTabExtraWidth(60);
	tab->setTabHeight(20);

	gui::IGUITab* t1 = tab->insertTab(0, L"Tab1");
	gui::IGUITab* t2 = tab->insertTab(1, L"Tab2");
	gui::IGUITab* t3 = tab->insertTab(2, L"Tab3");

	// Tab 2 eine Farbe geben
	video::SColor color;
	color.set(127, 255, 0, 0);
	t2->setBackgroundColor(color);
	t2->setDrawBackground(true);

	// Tab 3 auch eine Farbe geben
	color.set(127, 0, 0, 255);
	t3->setBackgroundColor(color);
	t3->setDrawBackground(true);

	// Test Button in Tab 1 und 2 einfügen
	env->addButton(core::rect<s32>(10, 10, 110, 30), t1, GUI_TEST_BTN1, L"Test Button 1");
	env->addButton(core::rect<s32>(10, 10, 110, 30), t2, GUI_TEST_BTN2, L"Test Button 2");

	env->addStaticText(L"Mit der Rechten Maustaste kann die Kamera von der Maus geloest werden, und damit diese GUI bedient werden",
						 core::rect<s32>(10, 10, 190, 190), true, true, panel, -1, true);

	// Weiteren Tab für Kamera-Positionierung einfügen
	gui::IGUITab* camTab = tab->insertTab(3, L"Kamera Position");
	color.set(63, 0, 255, 0);
	camTab->setBackgroundColor(color);
	camTab->setDrawBackground(true);

	env->addButton(core::rect<s32>(10, 10, 110, 30), camTab, GUI_TEST_CAMPOS_1, L"@50:50", L"Setzt die Kamera an die Spielfeldposition 50:50");
	env->addButton(core::rect<s32>(10, 40, 110, 60), camTab, GUI_TEST_CAMPOS_2, L"@950:50", L"Setzt die Kamera an die Spielfeldposition 950:50");
	env->addButton(core::rect<s32>(10, 70, 110, 90), camTab, GUI_TEST_CAMPOS_3, L"@50:950", L"Setzt die Kamera an die Spielfeldposition 50:950");
	env->addButton(core::rect<s32>(10, 100, 110, 120), camTab, GUI_TEST_CAMPOS_4, L"@950:950", L"Setzt die Kamera an die Spielfeldposition 950:950");

	// Einen Position, welche die gesammte Karte überblickt
	env->addButton(core::rect<s32>(10, 130, 110, 150), camTab, GUI_TEST_CAMPOS_PERFORMANCETEST, L"Performance Test Position", L"Position von wo aus die ganze Karte sichtbar ist");

}

void IngameGUI::resize(s32 newWidth, s32 newHeight) {
	panel->setRelativePosition(core::rect<s32>(0, newHeight - 200, newWidth, newHeight));
}
