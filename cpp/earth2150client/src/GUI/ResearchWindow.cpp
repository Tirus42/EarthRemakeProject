#include "GUI/ResearchWindow.h"

#include "GUI/IngameGUI.h"

using namespace irr;

ResearchWindow::ResearchWindow(gui::IGUIEnvironment* guiEnv, IngameGUI* ingameGUI) :
	IGUIWindow(guiEnv),
	ingameGUI(ingameGUI) {

	buttoncount = 0;
	buttontop = 10;
	buttonheight = 30;


	forschungsfenster = guiEnv->addWindow(core::rect<s32>(100, 100, 500, 600), false, L"Forschungsfenster",0,GUI_RESEARCHWINDOW);

	gui::IGUITabControl* tab = guiEnv->addTabControl(core::rect<s32>(0, 20, 400, 500), forschungsfenster, true, true, GUI_TABBER);

	tab->setTabExtraWidth(70);
//	tab->setTabHeight(10);

	gui::IGUITab* t1 = tab->insertTab(0, L"Chasis");
		// ED
		guiEnv->addButton(core::rect<s32>(10, buttontop, 110, buttonheight),t1, GUI_BTN_Pamir, L"Pamir");
		buttoncount++;
		buttontop = 30*buttoncount+10;
		buttonheight = buttontop+20;
		guiEnv->addButton(core::rect<s32>(10, buttontop, 110, buttonheight),t1, GUI_BTN_Taiga, L"Taiga");
		buttoncount++;
		buttontop = 30*buttoncount+10;
		buttonheight = buttontop+20;
		guiEnv->addButton(core::rect<s32>(10, buttontop, 110, buttonheight),t1, GUI_BTN_Kaukasus, L"Kaukasus");
		buttoncount++;
		buttontop = 30*buttoncount+10;
		buttonheight = buttontop+20;
		guiEnv->addButton(core::rect<s32>(10, buttontop, 110, buttonheight),t1, GUI_BTN_Ural, L"Ural");
		buttoncount++;
		buttontop = 30*buttoncount+10;
		buttonheight = buttontop+20;
		guiEnv->addButton(core::rect<s32>(10, buttontop, 110, buttonheight),t1, GUI_BTN_Sibiria, L"Sibiria");

	gui::IGUITab* t2 = tab->insertTab(1, L"Waffen");
	gui::IGUITab* t3 = tab->insertTab(2, L"Munition");
	gui::IGUITab* t4 = tab->insertTab(3, L"Sonstiges");


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
