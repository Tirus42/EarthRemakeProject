#ifndef NORMALSCREENRENDERER_H_INCLUDED
#define NORMALSCREENRENDERER_H_INCLUDED

#include "renderer/IScreenRenderer.h"

/**
* Rendert die Szene und die GUI im "normalen" Modus.
*/
class NormalScreenRenderer : public IScreenRenderer {
	public:
		NormalScreenRenderer(irr::IrrlichtDevice* device, irr::video::SColor backgroundColor);
		~NormalScreenRenderer();

		/// Rendert die Szene auf den Bildschirm
		void render();
};


#endif // NORMALSCREENRENDERER_H_INCLUDED