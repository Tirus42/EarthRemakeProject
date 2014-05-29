#ifndef RENDERVIEWPORT_H_INCLUDED
#define RENDERVIEWPORT_H_INCLUDED

#include <rect.h>

namespace irr {
namespace scene {
class ICameraSceneNode;
}
}

class VisualMap;

/**
* Stellt eine Speicherstruktur für die Render Positionen dar.
* Speichert eine Viewport Angabe für jede Kamera.
*/
class RenderViewport {
	private:
		const irr::scene::ICameraSceneNode* camera;
		const VisualMap& map;
		irr::core::recti viewport;

		RenderViewport(const RenderViewport&);
		RenderViewport operator=(const RenderViewport&);
	public:
		RenderViewport(const irr::scene::ICameraSceneNode* camera, const VisualMap& map, const irr::core::recti& viewport);
		~RenderViewport();

		/// Gibt die Kamera zurück
		const irr::scene::ICameraSceneNode* getCamera() const {
			return camera;
		}

		/// Gibt die zu Zeichende Karte zurück
		const VisualMap& getMap() const {
			return map;
		}

		/// Gibt den Viewport der Kamera zurück
		const irr::core::recti& getViewport() const {
			return viewport;
		}

		/// Setzt den Viewport der Kamera
		void setViewport(const irr::core::recti& newViewport) {
			viewport = newViewport;
		}
};

#endif // RENDERVIEWPORT_H_INCLUDED
