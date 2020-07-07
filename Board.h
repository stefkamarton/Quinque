#ifndef BOARD_H
#define BOARD_H
#include "Player.h"

#include "Tile.h"
#include "Borders.h"
#include "Controls.h"
#include "Colors.h"
#include <conio.h>
#include <vector>


class Board {
	static Board* Instance;

	const int MinTileWidth = 1;
	const int MinTileHeight = 1;
	const int DefaultCursorX = 0;
	const int DefaultCursorY = 0;
	const int DefaultMiniCursorX = 0;
	const int DefaultMiniCursorY = 0;

	int CurrentTileWidth;
	int CurrentTileHeight;

	int CursorX = DefaultCursorX;
	int CursorY = DefaultCursorY;
	int MiniCursorX = DefaultMiniCursorX;
	int MiniCursorY = DefaultMiniCursorY;

	std::vector<Player*> Players;

	int CurrentPlayerId;
	int CurrentStepNum=0;

	int AssignedX;
	int AssignedY;
	bool Assigning;

	int Rounds;

	Tile** Tiles;
	Board();
	
public:
	Player* getCurrentPlayer();
	void NextPlayer();
	void NextStep();
	bool AddPoint();
	void Moving(bool mini=false);
	static Board* getInstance();
	void printBoard(bool printCursor = false, bool mini=false);
	void printSecondStep();
	void printFirstStep();
	int getCurrentStep();
	bool Assign();
	void Sizing();
	int AllUsedTileNum();
	void doSizing(int direction);


};
#endif // !BOARD_H
