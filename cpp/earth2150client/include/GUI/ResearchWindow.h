#ifndef RESEARCHWINDOW_H_INCLUDED
#define RESEARCHWINDOW_H_INCLUDED

#include "GUI/IGUIWindow.h"

class IngameGUI;

/**
* Klasse welche das Forschungsfenster erstellt und auf Events in diesem reagiert.
*/
class ResearchWindow : IGUIWindow {
	private:
		ResearchWindow(const ResearchWindow&);
		ResearchWindow operator=(const ResearchWindow&);

		IngameGUI* ingameGUI;

	public:
		ResearchWindow(irr::gui::IGUIEnvironment* guiEnv, IngameGUI* ingameGUI);
		virtual ~ResearchWindow();

		virtual bool OnEvent(const irr::SEvent& event);
};


#endif // RESEARCHWINDOW_H_INCLUDED
