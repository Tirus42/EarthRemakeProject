#ifndef ENTITY_H
#define ENTITY_H

#include "e2150/MapPosition.h"

class Entity {
	private:
		int id;
		MapPosition mapPosition;
	public:
		Entity();
		virtual ~Entity();

		uint32_t getX() const{return mapPosition.getX();}
		uint32_t getY() const{return mapPosition.getY();}
};

#endif
