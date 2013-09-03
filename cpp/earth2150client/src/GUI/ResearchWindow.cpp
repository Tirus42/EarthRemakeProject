#include "GUI/ResearchWindow.h"

#include "GUI/IngameGUI.h"

using namespace irr;

ResearchWindow::ResearchWindow(gui::IGUIEnvironment* guiEnv, IngameGUI* ingameGUI) :
	IGUIWindow(guiEnv),
	ingameGUI(ingameGUI) {

	guiEnv->addWindow(core::rect<s32>(100, 100, 500, 400), false, L"Forschungsfenster");

}

ResearchWindow::~ResearchWindow() {

}


bool ResearchWindow::OnEvent(const SEvent& event) {
	// Event für Fenster geschlossen abfangen
	if (event.GUIEvent.EventType == gui::EGDT_WINDOW_CLOSE) {
		if (event.GUIEvent.Element == window)
			ingameGUI->closeResearchWindow();

	}

	// Todo: Fenster Element-Events abfangen
	return false;
}
