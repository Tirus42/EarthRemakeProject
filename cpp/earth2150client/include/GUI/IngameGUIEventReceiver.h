#ifndef INGAMEGUIEVENTRECEIVER_H_INCLUDED
#define INGAMEGUIEVENTRECEIVER_H_INCLUDED

#include <irrlicht.h>

class IngameGUI;

/**
* Event Receiver Klasse f�r die Ingame GUI
* Reagiert auf alle GUI Events und f�hrt entsprechende Funktionen aus.
*/
class IngameGUIEventReceiver : public irr::IEventReceiver {
	private:
		IngameGUI* gui;

	public:
		IngameGUIEventReceiver(IngameGUI* gui);
		virtual ~IngameGUIEventReceiver();

		virtual bool OnEvent(const irr::SEvent& event);
};


#endif // INGAMEGUIEVENTRECEIVER_H_INCLUDED
