#include "GUI/IngameGUI.h"

#include "GUI/ResearchWindow.h"

using namespace irr;

IngameGUI::IngameGUI(gui::IGUIEnvironment* guiEnv, IrrlichtDevice* device, scene::ICameraSceneNode* mainCam) :
	BasicGameInterface(guiEnv, device),
	testTabControl(0),
	testAlphaControl(0),
	mainCamera(mainCam),
	researchWindow(0) {

	mainCamera->grab();

	buildGUI();
}

IngameGUI::~IngameGUI() {
	mainCamera->drop();
}

void IngameGUI::buildGUI() {
	// Karten Panel holen
	gui::IGUIWindow* mapPanel = getMapPanel();
	mapPanel->setAlignment(gui::EGUIA_UPPERLEFT, gui::EGUIA_UPPERLEFT, gui::EGUIA_LOWERRIGHT, gui::EGUIA_LOWERRIGHT);

	const core::recti mapRect = mapPanel->getRelativePosition();

	// Infotext als "Karte" einügen
	env->addStaticText(L"Mit der Rechten Maustaste kann die Kamera von der Maus geloest werden, und damit diese GUI bedient werden",
						 core::recti(10, 10, mapRect.getWidth() - 10, mapRect.getHeight() - 10), true, true, mapPanel);


	// ControlPanel holen
	gui::IGUIWindow* panel = getControlPanel();

	// Panel Rect holen
	const core::recti panelRect = panel->getRelativePosition();

	// Einige Test Elemente eingfügen

	// Tab Control mit 3 Tabs
	testTabControl = env->addTabControl(core::rect<s32>(10, 10, panelRect.getWidth() - 40, panelRect.getHeight() - 10), panel, true, true, GUI_TABBER);

	gui::IGUITabControl* tab = testTabControl;
	tab->setAlignment(gui::EGUIA_UPPERLEFT, gui::EGUIA_LOWERRIGHT, gui::EGUIA_LOWERRIGHT, gui::EGUIA_LOWERRIGHT);

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

	env->addButton(core::rect<s32>(10, 10, 110, 30), t3, GUI_TEST_RESEARCH_WINDOW, L"Öffne Forschungsfenster");

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
	env->addButton(core::rect<s32>(120, 10, 220, 30), camTab, GUI_TEST_CAMPOS_PERFORMANCETEST, L"Performance Test Position", L"Position von wo aus die ganze Karte sichtbar ist");

	// Erstelle eine Scrollbar um die Durchsichtigkeit der gesammten GUI einzustellen
	testAlphaControl = env->addScrollBar(false, core::recti(panelRect.getWidth() - 30, 10, panelRect.getWidth() - 10, panelRect.getHeight() - 10), panel, GUI_TEST_ALPHA_CONTROL);
	testAlphaControl->setMax(255);
	testAlphaControl->setPos(127);
	testAlphaControl->setToolTipText(L"Bestimmt die Opazität der GUI");
	testAlphaControl->setAlignment(gui::EGUIA_LOWERRIGHT, gui::EGUIA_LOWERRIGHT, gui::EGUIA_UPPERLEFT, gui::EGUIA_LOWERRIGHT);
}

void IngameGUI::onResize(const core::dimension2du& newSize) {
	BasicGameInterface::onResize(newSize);

	// Panel größe holen
	const core::recti panelRect = getControlPanel()->getRelativePosition();

	testTabControl->setRelativePosition(core::recti(10, 10, panelRect.getWidth() - 40, panelRect.getHeight() - 10));

	testAlphaControl->setRelativePosition(core::recti(panelRect.getWidth() - 30, 10, panelRect.getWidth() - 10, panelRect.getHeight() - 10));
}

void IngameGUI::openResearchWindow() {
	if (!researchWindow)
		researchWindow = new ResearchWindow(env, this);
}

void IngameGUI::closeResearchWindow() {
	if (!researchWindow)
		return;

	delete researchWindow;
	researchWindow = 0;
}


bool IngameGUI::OnEvent(const irr::SEvent& event) {
	if (event.EventType == EET_GUI_EVENT) {
		s32 id = event.GUIEvent.Caller->getID();

		if (event.GUIEvent.EventType == gui::EGET_BUTTON_CLICKED) {
			switch (id) {
				case IngameGUI::GUI_TEST_BTN1:
					printf("Button 1 gedrueckt!\n");
					return true;

				case IngameGUI::GUI_TEST_BTN2:
					printf("Button 2 gedrueckt!\n");
					return true;

				case IngameGUI::GUI_TEST_RESEARCH_WINDOW:
					openResearchWindow();
					return true;

				case IngameGUI::GUI_TEST_CAMPOS_1:
					getMainCamera()->setPosition(core::vector3df(50, 50, 950));
					getMainCamera()->setTarget(core::vector3df(50, 30, 1000));
					return true;

				case IngameGUI::GUI_TEST_CAMPOS_2:
					getMainCamera()->setPosition(core::vector3df(950, 50, 950));
					getMainCamera()->setTarget(core::vector3df(950, 30, 1000));
					return true;

				case IngameGUI::GUI_TEST_CAMPOS_3:
					getMainCamera()->setPosition(core::vector3df(50, 50, 50));
					getMainCamera()->setTarget(core::vector3df(50, 30, 100));
					return true;

				case IngameGUI::GUI_TEST_CAMPOS_4:
					getMainCamera()->setPosition(core::vector3df(950, 50, 50));
					getMainCamera()->setTarget(core::vector3df(950, 30, 100));
					return true;

				case IngameGUI::GUI_TEST_CAMPOS_PERFORMANCETEST:
					getMainCamera()->setPosition(core::vector3df(-100, 100, -100));
					getMainCamera()->setTarget(core::vector3df(1000, 00, 1000));
					return true;
			}

		} else if (event.GUIEvent.EventType == gui::EGET_SCROLL_BAR_CHANGED) {
			// Momentan haben wir nur eine Scroll Bar -> Alpha Control
			u32 newAlpha = 255 - ((gui::IGUIScrollBar*)event.GUIEvent.Caller)->getPos();

			// Hole Ptr auf Skin der GUI
			gui::IGUISkin* skin = getGUIEnvironment()->getSkin();

			// Setze von allen GUI Elementen den neuen Alpha Wert
			for (u32 i = 0; i < gui::EGDC_COUNT; ++i) {
				// Hole aktuelle Farbe
				video::SColor col = skin->getColor((gui::EGUI_DEFAULT_COLOR)i);

				col.setAlpha(newAlpha);

				skin->setColor((gui::EGUI_DEFAULT_COLOR)i, col);
			}

			return true;
		}

		if (researchWindow)
			researchWindow->OnEvent(event);

	}

	// Fange Log-Eintrag Event beim Ändern der Auflösung ab
	if (event.EventType == EET_LOG_TEXT_EVENT) {
		// we check for log message like "Resizing window (640 480)"
		const char* b = "Resizing window (";
		const u32 strLength = 17;

		core::string<c8> s = event.LogEvent.Text;

		if (s.equalsn(b, strLength)) {
			s = s.subString(strLength, s.size() - strLength);

			u32 width = 0;
			u32 height = 0;

			sscanf(s.c_str(), "%u %u)", &width, &height);
			onResize(core::dimension2du(width, height));

			// Camera richtig skalieren
			getMainCamera()->setAspectRatio((float)width / (float)height);
		}
	 }


	return false;
}
