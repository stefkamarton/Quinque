#ifndef PLAYER_H
#define PLAYER_H
#include <string>
#include <iostream>
class Player {
	std::string Id = "";
	int ParachutesNum = 0;
	const int MAX_PARACHUTE = 18;
public:
	~Player();
	bool AddParachute();
	Player(int null);
	Player(std::string id);
	std::string getId();
	int getParachute();
	int getRemainingParachute();

};
#endif // !PLAYER_H
