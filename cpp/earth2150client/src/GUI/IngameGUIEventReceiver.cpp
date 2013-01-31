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

	return false;
}
