#include "GUI/BasicGameInterface.h"

#include "GUI/ResizeEvent.h"

using namespace irr;

// Todo: Positionsbestimmung der Panels über Parameter / Struct lösen
const u32 MENUPANEL_HEIGHT = 26;
const u32 CONTROLPANEL_HEIGHT = 172;

BasicGameInterface::BasicGameInterface(gui::IGUIEnvironment* guiEnv, IrrlichtDevice* device) :
	IGUI(guiEnv),
	device(device),	// Todo: Mit Irrlicht 1.9 nötigkeit prüfen und ggf. entfernen
	menuPanel(0),
	mapName(0),
	menuButton(0),
	mapPanel(0),
	controlPanel(0) {

	buildGUI();
}

BasicGameInterface::~BasicGameInterface() {
	menuPanel->remove();

	if (mapPanel)
		mapPanel->remove();

	if (controlPanel)
		controlPanel->remove();
}

void BasicGameInterface::buildGUI() {
	buildMenuPanel();
	buildMapPanel();
	buildControlPanel();
}

void BasicGameInterface::buildMenuPanel() {
	const core::recti screen = env->getRootGUIElement()->getAbsolutePosition();

	menuPanel = env->addWindow(core::recti(0, screen.getHeight() - MENUPANEL_HEIGHT, screen.getWidth(), screen.getHeight()), false, L"Menü Panel");
	makeWindowToPanel(menuPanel);
	menuPanel->setAlignment(gui::EGUIA_UPPERLEFT, gui::EGUIA_LOWERRIGHT, gui::EGUIA_LOWERRIGHT, gui::EGUIA_LOWERRIGHT);

	mapName = env->addStaticText(L"<Kartenname>", core::recti(95, 1, 95 + 158, MENUPANEL_HEIGHT - 1), true, false, menuPanel);
	mapName->setTextAlignment(gui::EGUIA_CENTER, gui::EGUIA_CENTER);

	const u32 menuButton_x = screen.getWidth() - 476;

	menuButton = env->addComboBox(core::recti(menuButton_x, 1, menuButton_x + 207, MENUPANEL_HEIGHT - 1), menuPanel);
	menuButton->setTextAlignment(gui::EGUIA_CENTER, gui::EGUIA_CENTER);
	menuButton->setAlignment(gui::EGUIA_LOWERRIGHT, gui::EGUIA_LOWERRIGHT, gui::EGUIA_LOWERRIGHT, gui::EGUIA_LOWERRIGHT);
}

// Passe Menü Panel an neue Auflösung an
void BasicGameInterface::resizeMenuPanel(const irr::core::dimension2du& newSize) {
	menuPanel->setRelativePosition(core::recti(0, newSize.Height - MENUPANEL_HEIGHT, newSize.Width, newSize.Height));

	const u32 menuButton_x = newSize.Width - 476;

	menuButton->setRelativePosition(core::recti(menuButton_x, 1, menuButton_x + 207, MENUPANEL_HEIGHT - 1));
}

void BasicGameInterface::buildMapPanel() {
	const core::recti screen = env->getRootGUIElement()->getAbsolutePosition();

	mapPanel = env->addWindow(core::recti(0, screen.getHeight() - MENUPANEL_HEIGHT - CONTROLPANEL_HEIGHT, 172, screen.getHeight() - MENUPANEL_HEIGHT), false, L"Map Panel");
	makeWindowToPanel(mapPanel);
}

void BasicGameInterface::resizeMapPanel(const irr::core::dimension2du& newSize) {
	mapPanel->setRelativePosition(core::recti(0, newSize.Height - MENUPANEL_HEIGHT - CONTROLPANEL_HEIGHT, 172, newSize.Height - MENUPANEL_HEIGHT));
}

void BasicGameInterface::buildControlPanel() {
	const core::recti screen = env->getRootGUIElement()->getAbsolutePosition();

	controlPanel = env->addWindow(core::recti(172, screen.getHeight() - MENUPANEL_HEIGHT - CONTROLPANEL_HEIGHT, screen.getWidth(), screen.getHeight() - MENUPANEL_HEIGHT), false, L"Control Panel");
    makeWindowToPanel(controlPanel);
    controlPanel->setAlignment(gui::EGUIA_UPPERLEFT, gui::EGUIA_LOWERRIGHT, gui::EGUIA_LOWERRIGHT, gui::EGUIA_LOWERRIGHT);
}

void BasicGameInterface::resizeControlPanel(const irr::core::dimension2du& newSize) {
	controlPanel->setRelativePosition(core::recti(172, newSize.Height - MENUPANEL_HEIGHT - CONTROLPANEL_HEIGHT, newSize.Width, newSize.Height - MENUPANEL_HEIGHT));
}

void BasicGameInterface::setMapName(const wchar_t* name) {
	mapName->setText(name);
}

void BasicGameInterface::onResize(const irr::core::dimension2du& newSize) {
	resizeMenuPanel(newSize);

	if (mapPanel)
		resizeMapPanel(newSize);

	if (controlPanel)
		resizeControlPanel(newSize);
}

bool BasicGameInterface::isMouseOverGUIElement(const core::position2di& mousePosition) const {
	if (menuPanel->getAbsoluteClippingRect().isPointInside(mousePosition))
		return true;

	if (mapPanel && mapPanel->getAbsolutePosition().isPointInside(mousePosition))
		return true;

	if (controlPanel && controlPanel->getAbsoluteClippingRect().isPointInside(mousePosition))
		return true;

	return false;
}

bool BasicGameInterface::OnEvent(const SEvent& event) {
	// Fange Log-Eintrag Event beim Ändern der Auflösung ab
	// Ist aufgrund eines Fehlers in Irrlicht nötig, sollte mit 1.9 behoben sein
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

			const core::dimension2du newSize(width, height);

			onResize(newSize);

			SEvent resizeEvent;

			ResizeEvent::createResizeEvent(resizeEvent, newSize);

			device->postEventFromUser(resizeEvent);
		}
	 }

	 return false;
}

void BasicGameInterface::makeWindowToPanel(gui::IGUIWindow* window) const {
	// Titelleiste und close Button nicht zeichen
	window->setDrawTitlebar(false);
	window->getCloseButton()->setVisible(false);

	// Fenster nicht verschiebbar machen
	window->setDraggable(false);
}
