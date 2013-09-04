#ifndef RESEARCHWINDOW_H_INCLUDED
#define RESEARCHWINDOW_H_INCLUDED

#include "GUI/IGUIWindow.h"

class IngameGUI;

/**
* Klasse welche das Forschungsfenster erstellt und auf Events in diesem reagiert.
*/
class ResearchWindow : IGUIWindow {
	private:
		irr::gui::IGUIWindow* forschungsfenster;
		ResearchWindow(const ResearchWindow&);
		ResearchWindow operator=(const ResearchWindow&);

		IngameGUI* ingameGUI;


		int buttoncount;
		int buttontop;
		int buttonheight;

	public:

		enum {
			// Chasis Buttons
			// ED
				GUI_BTN_Pamir,
				GUI_BTN_Taiga,
				GUI_BTN_Kaukasus,
				GUI_BTN_Ural,
				GUI_BTN_Sibiria,
				GUI_BTN_Minenleger_ED,
				GUI_BTN_Don,
				GUI_BTN_Wolga,
				GUI_BTN_Stealth,
				GUI_BTN_Oka,
				GUI_BTN_Baikal,
				GUI_BTN_Kiev,
				GUI_BTN_Kozak,
				GUI_BTN_Ataman,
				GUI_BTN_Thor,
				GUI_BTN_Han,
				GUI_BTN_Einheitentransporter_ED,
				GUI_BTN_Scout_ED,
				GUI_BTN_Bojar,
				GUI_BTN_Gruz,
			// LC
				GUI_BTN_Lunar,
				GUI_BTN_Moon,
				GUI_BTN_Crather,
				GUI_BTN_Crusher,
				GUI_BTN_FatGirl,
				GUI_BTN_NewHope,
				GUI_BTN_Crion,
				GUI_BTN_Meteor,
				GUI_BTN_Superfighter,
				GUI_BTN_Thunderer,
				GUI_BTN_Einheitentransporter_LC,
				GUI_BTN_Sonde,
				GUI_BTN_Versorgungsflugzeug,
				GUI_BTN_TunnelDigger,
			// UCS
				GUI_BTN_Tiger,
				GUI_BTN_Spider,
				GUI_BTN_Panther,
				GUI_BTN_Jaguar,
				GUI_BTN_CargoSalamander,
				GUI_BTN_Minenleger_UCS,
				GUI_BTN_Shark,
				GUI_BTN_Hydra,
				GUI_BTN_Orca,
				GUI_BTN_Gargoil,
				GUI_BTN_Bat,
				GUI_BTN_Dragon,
				GUI_BTN_Erntefahrzeug,
				GUI_BTN_Erntefahrzeug_fly,
				GUI_BTN_Einheitentransporter_UCS,
				GUI_BTN_Scout_UCS,
				GUI_BTN_Condor,
				GUI_BTN_Mammut,
			// Waffen Buttons
			// ED

			// LC

			// UCS
			// Munition Buttons
			// Sonstige Buttons

			GUI_TABBER,
			GUI_TEST_BTN2,
			GUI_RocketLauncher,
			GUI_RESEARCHWINDOW
		};
		ResearchWindow(irr::gui::IGUIEnvironment* guiEnv, IngameGUI* ingameGUI);
		virtual ~ResearchWindow();

		virtual bool OnEvent(const irr::SEvent& event);
};


#endif // RESEARCHWINDOW_H_INCLUDED
