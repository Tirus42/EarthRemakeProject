#ifndef ISCREENRENDERER_H_INCLUDED
#define ISCREENRENDERER_H_INCLUDED

#include <irrlicht.h>

#include <stdint.h>

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

		IScreenRenderer(const IScreenRenderer&);
		IScreenRenderer* operator=(const IScreenRenderer&);
	public:
		IScreenRenderer(irr::IrrlichtDevice* device, irr::video::SColor backgroundColor) :
			device(device),
			backgroundColor(backgroundColor),
			lastRenderTime(0) {

			device->grab();
		}

		virtual ~IScreenRenderer() {
			device->drop();
		}

		/// Rendert die Szene und gibt diese auf dem Bildschirm aus
		virtual void render() = 0;

		/// Gibt die Zeit zurück, wie lange das letzte Rendern in Nanosekunden gedauert hat
		uint64_t getLastRenderTime() const {
			return lastRenderTime;
		}

};


#endif // ISCREENRENDERER_H_INCLUDED
