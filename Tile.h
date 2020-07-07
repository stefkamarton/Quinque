#ifndef TILE_H
#define TILE_H
#include <iostream>
#include <Windows.h>
#include "Colors.h"
#include "Player.h"
#include <vector>
const HANDLE  hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

class Tile {
	std::vector<std::vector<Player*>> Fields;

public:
	static const int FieldSize = 2;
	Tile();
	Tile(const Tile& tile);
	void printLine(int linenum, bool justmini=false, int minicursor = 0, bool secondgreen=false);
	void setFields(Player* cPlayer, int x, int y);
	Player* getFields(int x, int y);
	bool notEmpty();
	void goNulled();
	int getNumberOfParachute();

private:

};
#endif // !TILE_H
