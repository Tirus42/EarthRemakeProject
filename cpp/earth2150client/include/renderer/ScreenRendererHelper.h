#ifndef SCREENRENDERERHELPER_H_INCLUDED
#define SCREENRENDERERHELPER_H_INCLUDED

#include <irrlicht.h>

class ScreenRendererHelper {
	private:
		/// Inventierte Bildbreite und Höhe (1 / width, 1 / height)
		irr::core::dimension2df invSize;

	public:
		ScreenRendererHelper(const irr::core::dimension2du& newSize);

		/// Setzt die neue Bildgröße
		void setScreenSize(const irr::core::dimension2du& newSize);

		/// Setzt die Vertices der Triangles so das diese ein Quader entsprechend des rect bilden
		void buildQuad(const irr::core::recti& rect, irr::core::triangle3df& t1, irr::core::triangle3df& t2) const;

		void buildFullScreenQuad(irr::core::triangle3df& t1, irr::core::triangle3df& t2) const;

};


#endif // SCREENRENDERERHELPER_H_INCLUDED
