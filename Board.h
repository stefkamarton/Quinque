#ifndef BOARD_H
#define BOARD_H
#include <stdlib.h>
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
	bool doAssign();

	int LasTMarkX;
	int LastMarkY;

	int Rounds;

	Tile** Tiles;
	bool** Connections;
	Board();
	~Board();
	void NextPlayer();
	void NextStep();
	bool AddPoint();
	void Moving(bool mini = false);

	bool Assign();
	void Sizing();
	int AllUsedTileNum();
	void doSizing(int direction);

	int NeighbourCheckX(int x, int y, int minix, int miniy, std::string mark, int step);
	int NeighbourCheckY(int x, int y, int minix, int miniy, std::string mark, int step);
	int NeighbourCheckDiagonal(int x, int y, int minix, int miniy, std::string mark, int step);
	int NeighbourCheckDiagonalInverse(int x, int y, int minix, int miniy, std::string mark, int step);
	bool isWin(int x, int y, int minix, int miniy, std::string mark);
	int isConnectedCounter(int x, int y, bool main);
	bool Win;
	std::string WinnerId;
public:
	Player* getCurrentPlayer();

	static Board* getInstance();
	void printBoard(bool printCursor = false, bool mini=false);
	void printSecondStep();
	void printFirstStep();
	int getCurrentStep();
	bool getWin();
	std::string getWinnerId();
	int hadNeighbour(int x, int y);
	Player* getPlayerById(int id);
	
	

};
#endif // !BOARD_H
