#include "tests/FlyingObjects.h"

#include "client/VisualMap.h"
#include "Map/MapPositionFloat.h"

using namespace irr;

FlyingObjects::FlyingObjects(const VisualMap& map, scene::ISceneManager* smgr) :
	map(map),
	smgr(smgr),
	rotationAnimation() {

	smgr->grab();
}

FlyingObjects::~FlyingObjects() {
	removeAll();

	smgr->drop();
}


void FlyingObjects::createRotatingObjects(scene::ISceneNode* object, const core::vector2df& center, f32 radius, f32 speed, u32 count) {
	core::vector3df rCenter(center.X, 50, center.Y);

	for (u32 i = 0; i < count; ++i) {
		scene::ISceneNode * centerObj = smgr->addEmptySceneNode();
		centerObj->setPosition(rCenter);

		scene::ISceneNode * mObj = object->clone(centerObj);
		mObj->setPosition(core::vector3df(1, 0, i));

		scene::ISceneNodeAnimator * animator = smgr->createFlyCircleAnimator(core::vector3df(), radius, speed, core::vector3df(0, 1, 0), i / (f32)count);

		mObj->addAnimator(animator);

		rotationAnimation.push_back(mObj);
	}
}

void FlyingObjects::update() {
	for (u32 i = 0; i < rotationAnimation.size(); ++i) {
		scene::ISceneNode* object = rotationAnimation[i];
		core::vector3df pos = object->getAbsolutePosition();

		MapPositionFloat mPos(pos.X, map.getHeight() - pos.Z);

		if (mPos.isValidOnMap(map)) {
			uint16_t height = map.getFieldHeight(map.position(mPos.getX(), mPos.getY()), mPos.getDX(), mPos.getDY());
			scene::ISceneNode * parent = object->getParent();

			pos = parent->getPosition();
			pos.Y = height / 2560.0 + 20;
			parent->setPosition(pos);
		}
	}
}

void FlyingObjects::removeAll() {
	for (u32 i = 0; i < rotationAnimation.size(); ++i) {
		scene::ISceneNode* parent = rotationAnimation[i]->getParent();

		parent->removeAll();
	}
}
