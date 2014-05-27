#ifndef GUI_H_INCLUDED
#define GUI_H_INCLUDED

#include <irrlicht.h>

/**
* Oberklasse für alle Unterschiedlichen Ingame-GUIs
* Dazu zählt das Hauptmenü, Ingamemenü, MultiplayerSession usw.
*/
class IGUI {
	private:
		IGUI(const IGUI&);
		IGUI& operator=(const IGUI&);
	protected:
		irr::gui::IGUIEnvironment* env;

	public:
		IGUI(irr::gui::IGUIEnvironment* guiEnv) : env(guiEnv) {env->grab();};
		virtual ~IGUI() {env->drop();};

		irr::gui::IGUIEnvironment* getGUIEnvironment() {
			return env;
		}

		/// Methode um die GUI an eine neue Auflösung anzupassen
		virtual void onResize(const irr::core::dimension2du& newSize) = 0;

		/// Gibt an, ob die Maus sich über einem GUI Element befindet oder nicht
		virtual bool isMouseOverGUIElement(const irr::core::position2di& mousePosition) const = 0;

		void show() {};
		void hide() {};

};


#endif // GUI_H_INCLUDED
