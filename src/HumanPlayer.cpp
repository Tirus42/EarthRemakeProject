#include "HumanPlayer.h"


HumanPlayer::HumanPlayer(const std::string& name, const sockaddr_in& networkAddress) :
		Player(name),
		networkAdress(networkAdress) {
}
