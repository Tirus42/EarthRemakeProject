#ifndef ISCREENRENDERER_H_INCLUDED
#define ISCREENRENDERER_H_INCLUDED

#include <irrlicht.h>

#include <stdint.h>

#include "renderer/RenderViewport.h"
#include "renderer/LightManager.h"


class VisualMap;

/**
* Interface für Verschiedene ScreenRenderer.
* Dient dazu, neben dem Normalen Rendering auch Stereoskopischen Rendern zu ermöglichen.
*/
class IScreenRenderer {
	protected:
		irr::IrrlichtDevice* device;

		/// Farbe mit der der Hintergrund gefüllt werden soll
		irr::video::SColor backgroundColor;

		/// Die Zeit in Nanosekunden wielange das letzte Rendern gedauert hat
		uint64_t lastRenderTime;

		/// Array der zu zeichnenden Kameras
		irr::core::array<RenderViewport*> renderViewports;

		LightManager lightManager;

		IScreenRenderer(const IScreenRenderer&);
		IScreenRenderer* operator=(const IScreenRenderer&);
	public:
		IScreenRenderer(irr::IrrlichtDevice* device, irr::video::SColor backgroundColor) :
			device(device),
			backgroundColor(backgroundColor),
			lastRenderTime(0),
			renderViewports(),
			lightManager() {

			device->grab();
		}

		virtual ~IScreenRenderer() {
			device->drop();
		}

		/// Initiiert den Renderer
		virtual bool init() = 0;

		/// Ändert die Ausgabeauflösung des Renderers
		virtual void resize(const irr::core::dimension2du& newSize) = 0;

		/// Rendert die Szene und gibt diese auf dem Bildschirm aus
		virtual void render(const VisualMap& map) = 0;

		/// Fügt einen Viewport hinzu, gibt dessen Index zurück
		irr::u32 addRenderViewport(const irr::scene::ICameraSceneNode* camera, const VisualMap& map, const irr::core::recti& viewport) {
			renderViewports.push_back(new RenderViewport(camera, map, viewport));

			return renderViewports.size() - 1;
		}

		/// Entfernt einen Viewport
		void removeRenderViewport(irr::u32 index) {
			delete renderViewports[index];
			renderViewports.erase(index);
		}

		LightManager& getLightManager() {
			return lightManager;
		}

		/// Gibt die Zeit zurück, wie lange das letzte Rendern in Nanosekunden gedauert hat
		uint64_t getLastRenderTime() const {
			return lastRenderTime;
		}

};


#endif // ISCREENRENDERER_H_INCLUDED
