#ifndef FLYINGOBJECTS_H_INCLUDED
#define FLYINGOBJECTS_H_INCLUDED

#include <irrlicht.h>

class VisualMap;

using namespace irr;

/**
* Test/Demo Klasse um Objekte über die Karte fliegen zu lassen.
* Diese Objekte Kreisen um ein Zentrum und halten dabei immer eine
* bestimmte Höhe über dem Kartenboden.
*/
class FlyingObjects {
	private:
		const VisualMap& map;
		scene::ISceneManager* smgr;

		core::array<scene::ISceneNode*> rotationAnimation;

		FlyingObjects(const FlyingObjects&);
		FlyingObjects operator=(const FlyingObjects&);

	public:
		FlyingObjects(const VisualMap& map, scene::ISceneManager* smgr);
		~FlyingObjects();

		void createRotatingObjects(scene::ISceneNode* object, const core::vector2df& center, irr::f32 radius, irr::f32 speed, irr::u32 count);

		/// passt die Höhe der Objekte an ihrer aktuellen Kartenposition an
		void update();

		void removeAll();

		const core::array<scene::ISceneNode*>& getNodeArray() const {
			return rotationAnimation;
		}

};


#endif // FLYINGOBJECTS_H_INCLUDED
