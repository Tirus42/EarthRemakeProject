#include "client/MousePicker.h"

#include "client/VisualMap.h"

using namespace irr;

MousePicker::MousePicker(const VisualMap& map, irr::scene::ISceneCollisionManager* colmgr) :
	map(map),
	colmgr(colmgr),
	pickedPosition(),
	pickedType(PickedNothing) {

	colmgr->grab();
}

MousePicker::~MousePicker() {
	colmgr->drop();
}

bool MousePicker::pick(core::position2di& mousePosition) {
	core::line3df ray = colmgr->getRayFromScreenCoordinates(mousePosition);

	MapPosition pos = map.pickMapPosition(ray.start, ray.getVector());

	// Todo: Prüfen ob auch mit einer Einheit/Gebäude/ect. auf diesem Feld geschnitten

	if (!pos.isValid()) {
		if (pos != pickedPosition) {
			pickedPosition = pos;
			pickedType = PickedNothing;
			return true;
		}

		return false;
	}

	if (pos != pickedPosition || pickedType != PickedGround) {
		pickedType = PickedGround;
		pickedPosition = pos;
		return true;
	}

	return false;
}
