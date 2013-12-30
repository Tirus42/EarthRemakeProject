#ifndef SCREENRENDERERHELPER_H_INCLUDED
#define SCREENRENDERERHELPER_H_INCLUDED

#include <irrlicht.h>

class ScreenRendererHelper {
	private:
		/// Inventierte Bildbreite und Höhe (1 / width, 1 / height)
		float invWidth;
		float invHeight;

	public:
		ScreenRendererHelper(irr::s32 width, irr::s32 height);
		~ScreenRendererHelper();

		/// Setzt die neue Bildgröße
		void setScreenSize(irr::s32 width, irr::s32 height);

		/// Setzt die Vertices der Triangles so das diese ein Quader entsprechend des rect bilden
		void buildQuad(const irr::core::recti& rect, irr::core::triangle3df& t1, irr::core::triangle3df& t2) const;

};


#endif // SCREENRENDERERHELPER_H_INCLUDED
