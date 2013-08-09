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
