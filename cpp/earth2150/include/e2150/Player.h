#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include <string>

/**
* Klasse stellt jede Art von Spieler dar (mit z.T. abstrakten Funktionen)
* Bots und Menschliche Spieler erben von dieser Klasse und implementieren die
* erforderlichen funktionen
*/
class Player {
	private:
		std::string name;

	public:
		Player(const std::string& name) : name(name){};
		virtual ~Player();

		const std::string& getName() const {return name;}
};

#endif
