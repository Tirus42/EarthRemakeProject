#include "client/GameState/ProfileSelect.h"

#include "client/GameState/GameStateFactory.h"

#include "client/EngineData.h"
#include <irrlicht.h>

using namespace irr;

using namespace video;
using namespace gui;
using namespace core;

ProfileSelect::ProfileSelect(EngineData& engineData) :
	AbstractGameState(engineData) {
}

AbstractGameState* ProfileSelect::run() {
	IrrlichtDevice* device = engineData.getIrrlichtDevice();
	IVideoDriver* driver = device->getVideoDriver();
	IGUIEnvironment* guienv = device->getGUIEnvironment();

	createGUI(guienv);

	while (device->run() && !shouldChangeGameState()) {
		driver->beginScene(true, true);

		guienv->drawAll();

		driver->endScene();
	}

	removeGUI();

	return getNextGameState();
}

void ProfileSelect::createGUI(irr::gui::IGUIEnvironment * guienv) {
	IGUIWindow* window = guienv->addWindow(recti(50, 50, 450, 650));
	guiElements[GUI_WINDOW_MAIN] = window;

	window->setDrawTitlebar(false);
	window->setDraggable(false);
	window->getCloseButton()->setVisible(false);

	guienv->addStaticText(L"Verfügbare Profile:", core::recti(10, 5, 390, 25), false, true, window, -1, true);

	guiElements[GUI_COMBOBOX_USERSELECT] = guienv->addListBox(recti(10, 35, 390, 300), window, -1, true);

	guiElements[GUI_BUTTON_NEWPROFILE] = guienv->addButton(recti(20, 320, 380, 360), window, -1, L"Neues Profil erstellen");
	guiElements[GUI_BUTTON_USEPROFILE] = guienv->addButton(recti(20, 380, 380, 420), window, -1, L"Ausgewähltes Profil verwenden");
	guiElements[GUI_BUTTON_BACK] = guienv->addButton(recti(20, 440, 380, 480), window, -1, L"Zurück zum Hauptmenü");

	guiElements[GUI_BUTTON_USEPROFILE]->setEnabled(false);
}

void ProfileSelect::removeGUI() {
	guiElements[GUI_WINDOW_MAIN]->remove();
}

bool ProfileSelect::OnEvent(const irr::SEvent& event) {
	if (event.EventType != EET_GUI_EVENT)
		return false;

	IGUIElement* caller = event.GUIEvent.Caller;

	// User in Combobox ausgewählt:
	if (caller == guiElements[GUI_COMBOBOX_USERSELECT] &&
		event.GUIEvent.EventType == gui::EGET_LISTBOX_CHANGED) {

		guiElements[GUI_BUTTON_USEPROFILE]->setEnabled(true);
		return true;
	}

	// Ab hier nurnoch behandlung von Buttons
	if (event.GUIEvent.EventType != gui::EGET_BUTTON_CLICKED)
		return false;

	if (caller == guiElements[GUI_BUTTON_BACK]) {
		changeGameState(GameStateFactory::createGameState(GameStateFactory::GS_MainMenu, engineData));
		return true;
	}

	return false;
}

u32 ProfileSelect::addProfile(const wchar_t* name) {
	gui::IGUIListBox* box = (gui::IGUIListBox*)guiElements[GUI_COMBOBOX_USERSELECT];

	return box->addItem(name);
}
