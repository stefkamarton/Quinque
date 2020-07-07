#include "Player.h"
Player::Player(std::string id):Id(id) {};
Player::Player(int null) :Id(" ") {};
std::string Player::getId() {
	return Player::Id;
}
Player::~Player() {}
int Player::getParachute() {
	return Player::ParachutesNum;
}
int Player::getRemainingParachute() {
	return Player::MAX_PARACHUTE-Player::ParachutesNum;
}
bool Player::AddParachute() {
	if (Player::ParachutesNum + 1 <= Player::MAX_PARACHUTE) {
		Player::ParachutesNum += 1;
		return true;
	}
	return false;

}