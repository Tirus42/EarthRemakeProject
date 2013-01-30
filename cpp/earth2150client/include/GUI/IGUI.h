#ifndef GUI_H_INCLUDED
#define GUI_H_INCLUDED

#include <irrlicht.h>

/**
* Oberklasse f�r alle Unterschiedlichen Ingame-GUIs
* Dazu z�hlt das Hauptmen�, Ingamemen�, MultiplayerSession usw.
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
