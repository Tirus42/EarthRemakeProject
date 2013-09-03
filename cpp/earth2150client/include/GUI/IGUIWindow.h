#ifndef IGUIWINDOW_H_INCLUDED
#define IGUIWINDOW_H_INCLUDED

#include <irrlicht.h>

class IGUIWindow : irr::IEventReceiver {
	private:
		IGUIWindow(const IGUIWindow&);
		IGUIWindow& operator=(const IGUIWindow&);

	protected:
		/// Referenz auf Irrlicht IGUIEnvironment
		irr::gui::IGUIEnvironment* guiEnv;

		/// Das Fenster
		irr::gui::IGUIWindow* window;

	public:
		IGUIWindow(irr::gui::IGUIEnvironment* guiEnv) :
			guiEnv(guiEnv),
			window(0) {
			guiEnv->grab();
			};

		virtual ~IGUIWindow() {
			guiEnv->drop();
		};

		irr::gui::IGUIWindow* getWindow() const {return window;}

};


#endif // IGUIWINDOW_H_INCLUDED
