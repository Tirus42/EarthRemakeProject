#pragma once

#include "renderer/IScreenRenderer.h"

/**
* Engine Renderer im "kompatibelen" Modus.
* Also unter vermeidung moderner GPU-Features.
*/
class CompatibleScreenRenderer : public IScreenRenderer {
	public:
		CompatibleScreenRenderer(irr::IrrlichtDevice* device, irr::video::SColor backgroundColor);
		~CompatibleScreenRenderer();

		bool init();

		virtual void resize(const irr::core::dimension2du& newSize);

		/// Rendert die Szene auf den Bildschirm
		void render(const VisualMap& map);
};
