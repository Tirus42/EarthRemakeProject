#include "renderer/ScreenRendererHelper.h"

using namespace irr;

ScreenRendererHelper::ScreenRendererHelper(const core::dimension2du& newSize) :
	invSize() {

	setScreenSize(newSize);
}


void ScreenRendererHelper::setScreenSize(const core::dimension2du& newSize) {
	invSize.Width = 1.f / newSize.Width;
	invSize.Height = 1.f / newSize.Height;
}


void ScreenRendererHelper::buildQuad(const core::recti& rect, core::triangle3df& t1, core::triangle3df& t2) const {
	// Hole alle Eckpunkte als Referenz
	core::vector3df& v0 = t1.pointA;
	core::vector3df& v1 = t1.pointB;
	core::vector3df& v2 = t1.pointC;
	core::vector3df& v3 = t2.pointB;

	// Setze Eckpunkte entsprechend der Ecken
	v0.X = v1.X = -1 + rect.UpperLeftCorner.X * invSize.Width * 2.f;
	v2.X = v3.X = -1 + rect.LowerRightCorner.X * invSize.Width * 2.f;

	v0.Y = v2.Y = 1 - rect.LowerRightCorner.Y * invSize.Height * 2.f;
	v1.Y = v3.Y = 1 - rect.UpperLeftCorner.Y * invSize.Height * 2.f;

	v0.Z = v1.Z = v2.Z = v3.Z = 0;

	// Kopiere Eckpunkte in t2 zurück (alles andere ist bereits eine passende Referenz)
	t2.pointA = v1;
	t2.pointC = v2;
}

void ScreenRendererHelper::buildFullScreenQuad(core::triangle3df& t1, core::triangle3df& t2) const {
	t1.pointA.set(-1, -1, 0);
	t1.pointB.set(-1, 1, 0);
	t1.pointC.set(1, -1, 0);

	t2.pointA = t1.pointB;
	t2.pointC = t1.pointC;
	t2.pointB.set(1, 1, 0);

	//core::vector3df v0(-1, -1, 0);
	//core::vector3df v1(-1, 1, 0);
	//core::vector3df v2(1, -1, 0);
	//core::vector3df v3(1, 1, 0);

	//core::triangle3df trig1(v0, v1, v2);
	//core::triangle3df trig2(v1, v3, v2);
}

void ScreenRendererHelper::drawViewFrustum(irr::video::IVideoDriver* driver, const irr::scene::SViewFrustum& frustum) const {
	driver->draw3DLine(frustum.getNearLeftUp(), frustum.getNearRightUp());
	driver->draw3DLine(frustum.getNearRightUp(), frustum.getNearRightDown());
	driver->draw3DLine(frustum.getNearRightDown(), frustum.getNearLeftDown());
	driver->draw3DLine(frustum.getNearLeftDown(), frustum.getNearLeftUp());

	driver->draw3DLine(frustum.getFarLeftUp(), frustum.getFarRightUp());
	driver->draw3DLine(frustum.getFarRightUp(), frustum.getFarRightDown());
	driver->draw3DLine(frustum.getFarRightDown(), frustum.getFarLeftDown());
	driver->draw3DLine(frustum.getFarLeftDown(), frustum.getFarLeftUp());

	driver->draw3DLine(frustum.getNearLeftUp(), frustum.getFarLeftUp());
	driver->draw3DLine(frustum.getNearLeftDown(), frustum.getFarLeftDown());
	driver->draw3DLine(frustum.getNearRightUp(), frustum.getFarRightUp());
	driver->draw3DLine(frustum.getNearRightDown(), frustum.getFarRightDown());
}
