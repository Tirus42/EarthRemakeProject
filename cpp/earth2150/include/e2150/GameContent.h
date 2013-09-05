#ifndef GAMECONTENT_H_INCLUDED
#define GAMECONTENT_H_INCLUDED

#include "e2150/FactionManager.h"

class GameContent {
	private:
		GameContent(const GameContent&);
		GameContent operator=(const GameContent&);

		FactionManager factionManager;

	public:
		GameContent();
		~GameContent();

		/// Gibt den FactionManager zurück
		const FactionManager& getFactionManager() const {
			return factionManager;
		}

		/// Gibt den FactionManager zurück
		FactionManager& getFactionManager() {
			return factionManager;
		}




};


#endif // GAMECONTENT_H_INCLUDED
