#ifndef ENTITY_H
#define ENTITY_H

#include "e2150/MapPosition.h"

class Entity {
	public:
		Entity();
		virtual ~Entity();
	private:
		int id;
		MapPosition mapPosition;
};

#endif
