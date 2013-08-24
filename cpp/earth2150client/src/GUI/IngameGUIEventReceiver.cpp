#include "GUI/IngameGUIEventReceiver.h"

#include "GUI/IngameGUI.h"

#include <stdio.h>

using namespace irr;

IngameGUIEventReceiver::IngameGUIEventReceiver(IngameGUI* gui) :
	gui(gui) {
}

IngameGUIEventReceiver::~IngameGUIEventReceiver() {

}

bool IngameGUIEventReceiver::OnEvent(const irr::SEvent& event) {
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

				case IngameGUI::GUI_TEST_CAMPOS_1:
					gui->getMainCamera()->setPosition(core::vector3df(50, 50, 950));
					gui->getMainCamera()->setTarget(core::vector3df(50, 30, 1000));
					return true;

				case IngameGUI::GUI_TEST_CAMPOS_2:
					gui->getMainCamera()->setPosition(core::vector3df(950, 50, 950));
					gui->getMainCamera()->setTarget(core::vector3df(950, 30, 1000));
					return true;

				case IngameGUI::GUI_TEST_CAMPOS_3:
					gui->getMainCamera()->setPosition(core::vector3df(50, 50, 50));
					gui->getMainCamera()->setTarget(core::vector3df(50, 30, 100));
					return true;

				case IngameGUI::GUI_TEST_CAMPOS_4:
					gui->getMainCamera()->setPosition(core::vector3df(950, 50, 50));
					gui->getMainCamera()->setTarget(core::vector3df(950, 30, 100));
					return true;

				case IngameGUI::GUI_TEST_CAMPOS_PERFORMANCETEST:
					gui->getMainCamera()->setPosition(core::vector3df(-100, 100, -100));
					gui->getMainCamera()->setTarget(core::vector3df(1000, 00, 1000));
					return true;

			}

		}


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
			gui->resize(width, height);

			// Camera richtig skalieren
			gui->getMainCamera()->setAspectRatio((float)width / (float)height);
		}
	 }


	return false;
}
