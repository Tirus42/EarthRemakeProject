#include "renderer/ScreenRendererHelper.h"

using namespace irr;

ScreenRendererHelper::ScreenRendererHelper(s32 width, s32 height) :
	invWidth(),
	invHeight() {

	setScreenSize(width, height);
}

ScreenRendererHelper::~ScreenRendererHelper() {

}


void ScreenRendererHelper::setScreenSize(s32 width, s32 height) {
	invWidth = 1.f / width;
	invHeight = 1.f / height;
}


void ScreenRendererHelper::buildQuad(const core::recti& rect, core::triangle3df& t1, core::triangle3df& t2) const {
	// Hole alle Eckpunkte als Referenz
	core::vector3df& v0 = t1.pointA;
	core::vector3df& v1 = t1.pointB;
	core::vector3df& v2 = t1.pointC;
	core::vector3df& v3 = t2.pointB;

	// Setze Eckpunkte entsprechend der Ecken
	v0.X = v1.X = -1 + rect.UpperLeftCorner.X * invWidth * 2.f;
	v2.X = v3.X = -1 + rect.LowerRightCorner.X * invWidth * 2.f;

	v0.Y = v2.Y = 1 - rect.LowerRightCorner.Y * invHeight * 2.f;
	v1.Y = v3.Y = 1 - rect.UpperLeftCorner.Y * invHeight * 2.f;

	v0.Z = v1.Z = v2.Z = v3.Z = 0;

	// Kopiere Eckpunkte in t2 zurück (alles andere ist bereits eine passende Referenz)
	t2.pointA = v1;
	t2.pointC = v2;
}
