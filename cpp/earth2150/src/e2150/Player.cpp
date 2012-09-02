#include "e2150/Player.h"
#include <cassert>


Player::~Player() {

}

void Player::removeVisibleMap(const Map& map) {
	for (std::vector<const Map*>::iterator i = maps.begin(); i != maps.end(); ++i) {
		if ((*i) == &map) {
			maps.erase(i);
			return;
		}
	}

	assert(false);
}
