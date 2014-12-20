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

		/// Zeichnet die Ränder des ViewFrustum als Linien
		static void drawViewFrustum(irr::video::IVideoDriver* driver, const irr::scene::SViewFrustum& frustum);

		/// Prüft ob der Punkt in dem ViewFrustum liegt
		static bool isInViewFrustum(const irr::scene::SViewFrustum& frustum, const irr::core::vector3df& point);

		/// Prüft ob die Sphere in dem ViewFrustum liegt
		static bool isInViewFrustum(const irr::scene::SViewFrustum& frustum, const irr::core::vector3df& poin, irr::f32 radius);

		/// Prüft ob die AABB in dem ViewFrustum liegt
		static bool isInViewFrustum(const irr::scene::SViewFrustum& frustum, const irr::core::aabbox3df& aabb);

};


#endif // SCREENRENDERERHELPER_H_INCLUDED
