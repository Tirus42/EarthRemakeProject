#ifndef GUI_H_INCLUDED
#define GUI_H_INCLUDED

#include <irrlicht.h>

/**
* Oberklasse für alle Unterschiedlichen Ingame-GUIs
* Dazu zählt das Hauptmenü, Ingamemenü, MultiplayerSession usw.
*/
class IGUI {
	private:

	protected:
		irr::gui::IGUIEnvironment* env;

	public:
		IGUI(irr::gui::IGUIEnvironment* guiEnv) : env(guiEnv) {env->grab();};
		virtual ~IGUI() {env->drop();};

		void show() {};
		void hide() {};

};


#endif // GUI_H_INCLUDED
