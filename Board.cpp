#include "Board.h"
Player* NullPlayer = new Player(0);
Board* Board::Instance = 0;
Board* Board::getInstance()
{
	if (Board::Instance == 0)
	{
		Board::Instance = new Board();
	}

	return Board::Instance;
}

Board::Board() {
	/*Create Players*/
	Board::Players.push_back(new Player("X"));
	Board::Players.push_back(new Player("O"));

	/*Set Current Step Number*/
	Board::CurrentStepNum = 0;

	/*Set Current Player*/
	Board::CurrentPlayerId = 0;

	/*Set Assigns*/
	Board::AssignedX = -1;
	Board::AssignedY = -1;
	Board::Assigning = false;

	Board::LasTMarkX = -1;
	Board::LastMarkY = -1;

	Board::Rounds = 1;
	Board::Win = false;

	/*Create Tiles*/
	Board::CurrentTileWidth = MinTileWidth;
	Board::CurrentTileHeight = MinTileHeight;

	Board::Tiles = new Tile * [MinTileHeight];

	for (int i = 0; i < MinTileHeight; i++)
	{
		Board::Tiles[i] = new Tile[MinTileWidth];
	}

}
Board::~Board() {
	for (int i = 0; i < CurrentTileHeight; i++)
	{
		for (int j = 0; j < CurrentTileWidth; j++)
		{
			Board::Tiles[i][j].~Tile();
		}
		delete[] Board::Tiles[i];
	}
	delete[] Board::Tiles;

	Board::Players.clear();
	delete Board::Instance;
}
Player* Board::getCurrentPlayer() {
	return Board::Players[CurrentPlayerId];
}
void Board::NextPlayer() {
	if (Board::CurrentPlayerId == 0) {
		Board::CurrentPlayerId += 1;
	}
	else {
		Board::CurrentPlayerId = 0;
	}
}
bool Board::AddPoint() {
	if (Board::Players[Board::CurrentPlayerId]->getRemainingParachute() > 0 && (Board::Tiles[CursorY][CursorX].getFields(MiniCursorX, MiniCursorY)->getId()) == " " &&
		(Board::hadNeighbour(CursorX,CursorY)>0 || Board::AllUsedTileNum() < 2)) {
		Board::Tiles[CursorY][CursorX].setFields(Board::Players[Board::CurrentPlayerId], MiniCursorX, MiniCursorY);
		Board::LasTMarkX = CursorX;
		Board::LastMarkY = CursorY;
		Board::Win = Board::isWin(CursorX, CursorY, MiniCursorX, MiniCursorY, Board::Players[Board::CurrentPlayerId]->getId());
		system("pause");
		Board::Sizing();
		return true;
	}
	else {
		SetConsoleTextAttribute(hConsole, Colors::WhiteOnRed);
		std::cout << "You can't allow to mark! Choose a green field!"<<std::endl;					
		SetConsoleTextAttribute(hConsole, Colors::WhiteOnBlack);
		system("pause");
	}
	return false;
}
int Board::isConnectedCounter(int x, int y, bool main=false) {
	if (main) {
		Board::Connections = new bool* [Board::CurrentTileHeight];
		for (int i = 0; i < Board::CurrentTileHeight; i++)
		{
			Board::Connections[i] = new bool[Board::CurrentTileWidth];
			for (int j = 0; j < Board::CurrentTileWidth; j++)
			{
				Board::Connections[i][j] = false;
				std::cout << "?";
			}
		}
	}
	int counter = 1;
	Board::Connections[y][x] = true;
	//Connections[y][x] = true;
	/*std::cout << "X:" << x << std::endl;
	std::cout << "Y:" << y << std::endl;
	std::cout << "sad" << Board::Tiles[y][x + 1].notEmpty() << std::endl;
	system("pause");*/

	if (x+1 < Board::CurrentTileWidth && Board::Tiles[y][x+1].notEmpty() && Connections[y][x + 1] == false) {
		counter += isConnectedCounter(x + 1,y);
	}
	if (x - 1 >= 0 && Board::Tiles[y][x - 1].notEmpty() && Connections[y][x-1]==false) {
		counter += isConnectedCounter(x - 1, y);
	}
	if (y + 1 < Board::CurrentTileHeight && Board::Tiles[y+1][x].notEmpty() && Connections[y+1][x] == false) {
		counter += isConnectedCounter(x, y+1);
	}
	if (y - 1 >= 0 && Board::Tiles[y-1][x].notEmpty() && Connections[y-1][x] == false) {
		counter += isConnectedCounter(x, y-1);
	}

	if (main) {
		for (int i = 0; i < CurrentTileHeight; i++)
		{
			delete[] Board::Connections[i];
		}
		delete[] Board::Connections;
	}
	return counter;
}
bool Board::getWin() {
	return Board::Win;
}
void Board::doSizing(int direction) {

	Tile** newTiles = new Tile * [CurrentTileHeight];

	for (int i = 0; i < CurrentTileHeight; i++)
	{
		newTiles[i] = new Tile[CurrentTileWidth];
	}
	int cwidth = CurrentTileWidth;
	int wstart = 0;
	int cheight = CurrentTileHeight;
	int hstart = 0;

	switch (direction)
	{
	case 1://Left
		wstart += 1;
		Board::CursorX += 1;
		break;
	case 2://Up
		hstart += 1;
		Board::CursorY += 1;
		break;
	case 3://Right
		cwidth -= 1;
		break;
	case 4://Down
		cheight -= 1;
		break;
	}
	for (int i = hstart; i < cheight; i++)
	{
		for (int j = wstart; j < cwidth; j++)
		{
			switch (direction)
			{
			case 1://Left
				newTiles[i][j] = Board::Tiles[i][j - 1];
				break;
			case 2://Up
				newTiles[i][j] = Tile(Board::Tiles[i - 1][j]);
				break;
			case 3://Right
				newTiles[i][j] = Tile(Board::Tiles[i][j]);
				break;
			case 4://Down
				newTiles[i][j] = Tile(Board::Tiles[i][j]);
				break;
			}
		}
	}
	for (int i = 0; i < cheight - hstart; i++)
	{
		delete[] Board::Tiles[i];
	}
	delete[] Board::Tiles;

	Board::Tiles = newTiles;
}
void Board::Sizing() {
	int pluswidth = 0;
	int plusheight = 0;
	int x = Board::CursorX;
	int y = Board::CursorY;
	if (x == 0) {
		Board::CurrentTileWidth += 1;
		pluswidth += 1;
		Board::doSizing(1);
	}
	if (y == 0) {
		Board::CurrentTileHeight += 1;
		plusheight += 1;
		Board::doSizing(2);
	}
	if (x == Board::CurrentTileWidth - 1 - pluswidth) {
		Board::CurrentTileWidth += 1;
		Board::doSizing(3);
	}
	if (y == Board::CurrentTileHeight - 1 - plusheight) {
		Board::CurrentTileHeight += 1;
		Board::doSizing(4);
	}
}
void Board::NextStep() {
	if (Board::CurrentStepNum == 1) {
		Board::CurrentStepNum = 0;
		Board::NextPlayer();
		Board::LasTMarkX=-1;
		Board::LastMarkY=-1;
		Board::Rounds += 1;
	}
	else {
		Board::CurrentStepNum += 1;
	}
}
void Board::printBoard(bool printCursor, bool justmini) {
	for (int row = 0; row < Board::CurrentTileHeight; row++)
	{
		if (row == 0) {
			std::cout << char(Borders::cornerTopLeft);

			for (int topi = 1; topi < Tile::FieldSize * 4 * Board::CurrentTileWidth; topi++)
			{
				if (topi % 4 == 0) {
					std::cout << char(Borders::crossDoubleDown);
				}
				else {
					std::cout << char(Borders::doubleHorizontal);
				}
			}
			std::cout << char(Borders::cornerTopRight);
			std::cout << std::endl;
		}
		bool secondgreen = false;
		for (int line = 0; line < Tile::FieldSize; line++)
		{
			std::cout << char(Borders::doubleVertical);

			for (int col = 0; col < Board::CurrentTileWidth; col++)
			{
				if (row - 1 >= 0 && Board::Tiles[row - 1][col].notEmpty() ||
					row + 1 < Board::CurrentTileHeight && Board::Tiles[row + 1][col].notEmpty() ||
					col - 1 >= 0 && Board::Tiles[row][col - 1].notEmpty() ||
					col + 1 < Board::CurrentTileWidth && Board::Tiles[row][col + 1].notEmpty() || Board::Tiles[row][col].notEmpty()) {
					secondgreen = true;
					if (Board::AssignedX == -1 && Board::AssignedY == -1 && printCursor &&justmini) {
						SetConsoleTextAttribute(hConsole, Colors::BlackOnGreen);
					}
				}
				if (Board::AssignedX != -1 && Board::AssignedY != -1) {
					switch (Board::Tiles[Board::AssignedY][Board::AssignedX].getNumberOfParachute())
					{
					case 1:
						if (((abs(col - Board::AssignedX) == 1 && abs(row - Board::AssignedY) == 0) ||
							(abs(col - Board::AssignedX) == 0 && abs(row - Board::AssignedY) == 1)) && Board::hadNeighbour(col,row)>0) {
							if (!Board::Tiles[row][col].notEmpty()) {
								SetConsoleTextAttribute(hConsole, Colors::RedOnGold);
							}
						}
						break;
					case 2:
						if (((abs(col - Board::AssignedX) > 0 && abs(row - Board::AssignedY) == 0) ||
							(abs(col - Board::AssignedX) == 0 && abs(row - Board::AssignedY) > 0)) && Board::hadNeighbour(col, row)>0) {
							if (!Board::Tiles[row][col].notEmpty()) {
								SetConsoleTextAttribute(hConsole, Colors::RedOnGold);
							}
						}
						break;
					case 3:
						if ((abs(col - Board::AssignedX) == abs(row - Board::AssignedY)) && Board::hadNeighbour(col, row)>0) {
							if (!Board::Tiles[row][col].notEmpty()) {
								SetConsoleTextAttribute(hConsole, Colors::RedOnGold);
							}
							SetConsoleTextAttribute(hConsole, Colors::RedOnGold);
						}
						break;
					case 4:
						if (((abs(col - Board::AssignedX) > 0 && abs(row - Board::AssignedY) == 0) ||
							(abs(col - Board::AssignedX) == 0 && abs(row - Board::AssignedY) > 0) ||
							(abs(col - Board::AssignedX) == abs(row - Board::AssignedY))) && Board::hadNeighbour(col, row)>0) {
							if (!Board::Tiles[row][col].notEmpty()) {
								SetConsoleTextAttribute(hConsole, Colors::RedOnGold);
							}
						}
						break;
					}
				}
				if (printCursor && CursorX == col && CursorY == row && !justmini) {
					SetConsoleTextAttribute(hConsole, Colors::BlackOnCyan);
				}
				if (printCursor && justmini && CursorX == col && CursorY == row && MiniCursorY == line) {
					SetConsoleTextAttribute(hConsole, Colors::BlackOnCyan);
				}
				if (Board::Tiles[row][col].notEmpty()) {
					secondgreen = true;
				}
				if (printCursor && Board::AssignedX == col && AssignedY == row && !justmini) {
					SetConsoleTextAttribute(hConsole, Colors::RedOnGray);
				}

				Board::Tiles[row][col].printLine(line, justmini, MiniCursorX, secondgreen);
				secondgreen = false;
				std::cout << char(Borders::doubleVertical);
			}
			std::cout << std::endl;
			if (Board::CurrentTileHeight - 1 == row && Tile::FieldSize - 1 == line) {
				std::cout << char(Borders::cornerBottomLeft);
			}
			else {
				std::cout << char(Borders::crossRight);
			}

			for (int hori = 1; hori < Tile::FieldSize * 4 * Board::CurrentTileWidth; hori++)
			{
				if (line % 2 == 1) {

					if (hori % 4 == 0) {
						if (Board::CurrentTileHeight - 1 != row) {
							std::cout << char(Borders::crossDouble);
						}
						else {
							std::cout << char(Borders::crossDoubleUp);
						}
					}
					else {
						std::cout << char(Borders::doubleHorizontal);
					}

				}
				else {
					if (hori % 4 == 0 && (hori / 4) % 2 == 1) {
						std::cout << char(Borders::crossSimple);
					}
					else {
						if (hori % 4 == 0 && (hori / 4) % 2 == 0) {
							std::cout << char(Borders::crossDouble);
						}
						else {
							std::cout << char(Borders::simpleHorizontal);
						}
					}
				}
			}
			if (Board::CurrentTileHeight - 1 == row && Tile::FieldSize - 1 == line) {
				std::cout << char(Borders::cornerBottomRight);
			}
			else {
				std::cout << char(Borders::crossLeft);
			}


			std::cout << std::endl;


		}
	}
	std::cout << "Current player is ";
	if (Board::CurrentPlayerId == 1) {
		SetConsoleTextAttribute(hConsole, Colors::BlueOnBlack);
	}
	else {
		SetConsoleTextAttribute(hConsole, Colors::RedOnBlack);
	}
	std::cout << Board::Players[Board::CurrentPlayerId]->getId() << std::endl;
	SetConsoleTextAttribute(hConsole, Colors::WhiteOnBlack);
	std::cout << "Remaining parachutes is ";
	SetConsoleTextAttribute(hConsole, Colors::WhiteOnPurple);
	std::cout << Board::Players[Board::CurrentPlayerId]->getRemainingParachute() << std::endl;
	SetConsoleTextAttribute(hConsole, Colors::WhiteOnBlack);
	std::cout << "---" << std::endl;
}
void Board::printFirstStep() {
	system("cls");
	Board::printBoard(false, false);
	std::cout << "Press ";
	SetConsoleTextAttribute(hConsole, Colors::WhiteOnPurple);
	std::cout << "M";
	SetConsoleTextAttribute(hConsole, Colors::WhiteOnBlack);
	std::cout<<" to marking a skydiver" << std::endl;

	if (Board::AllUsedTileNum() >= 2) {
		std::cout << "Press ";
		SetConsoleTextAttribute(hConsole, Colors::WhiteOnPurple);
		std::cout << "N";
		SetConsoleTextAttribute(hConsole, Colors::WhiteOnBlack);
		std::cout <<" to moving a tile" << std::endl;
	}
	switch (_getch())
	{
	case Controls::M: //Marking
		//Board::printBoard(true, true);
		Board::Moving(true);
		break;
	case Controls::N:
		if (Board::AllUsedTileNum() >= 2) {
			Board::Moving(false);
		}
		else {
			Board::NextStep();
		}
		break;
	}

}
int Board::hadNeighbour(int x, int y) {
	int counter = 0;
	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			if (i != j && abs(i)!=j && abs(j)!=i && y + i >= 0 && y + i < Board::CurrentTileHeight && x + j >= 0 && x + j < Board::CurrentTileWidth) {
				if ((Board::AssignedX == x + j && Board::AssignedY == y + i)) {
					/*return false;*/
				}else{
					if (Board::Tiles[y + i][x + j].notEmpty()) {
						counter++;
					}
				}
			}
		}
	}
	return counter;
}
void Board::printSecondStep() {
	system("cls");
	Board::printBoard(false, false);
	if (Board::AllUsedTileNum() >= 2) {
		std::cout << "Press ";
		SetConsoleTextAttribute(hConsole, Colors::WhiteOnPurple);
		std::cout << "N";
		SetConsoleTextAttribute(hConsole, Colors::WhiteOnBlack);
		std::cout << " to moving a tile" << std::endl;
		std::cout << "Press ";
		SetConsoleTextAttribute(hConsole, Colors::WhiteOnPurple);
		std::cout << "S";
		SetConsoleTextAttribute(hConsole, Colors::WhiteOnBlack);
		std::cout << " to skip this step" << std::endl;
		switch (_getch())
		{
		case Controls::S:
			Board::NextStep();
			break;
		case Controls::N:
			Board::Moving(false);
			break;
		}
	}
	else {
		Board::NextStep();
	}
}
int Board::getCurrentStep() {
	return Board::CurrentStepNum;
}
int Board::AllUsedTileNum() {
	int counter = 0;
	for (int i = 0; i < Board::CurrentTileHeight; i++)
	{
		for (int j = 0; j < Board::CurrentTileWidth; j++)
		{
			if (Board::Tiles[i][j].notEmpty()) {
				counter += 1;
			}
		}
	}
	return counter;
}
void Board::Moving(bool ismarking) {
	bool quit = false;
	while (!quit) {
		system("cls");
		Board::printBoard(true, ismarking);
		std::cout << "Press ";
		SetConsoleTextAttribute(hConsole, Colors::WhiteOnPurple);
		std::cout << "C";
		SetConsoleTextAttribute(hConsole, Colors::WhiteOnBlack);
		std::cout << " to back" << std::endl;
		switch (_getch())
		{
		case Controls::Up:
			if (ismarking) {
				if (Board::MiniCursorY - 1 < 0) {
					if (Board::CursorY - 1 >= 0) {
						Board::CursorY -= 1;
						Board::MiniCursorY = Tile::FieldSize - 1;
					}
				}
				else {
					Board::MiniCursorY -= 1;
				}
			}
			else {
				if (Board::CursorY - 1 >= 0) {
					Board::CursorY -= 1;
				}
			}
			break;
		case Controls::Down:
			if (ismarking) {
				if (Board::MiniCursorY + 1 >= Tile::FieldSize) {
					if (Board::CursorY + 1 <= Board::CurrentTileHeight - 1) {
						Board::CursorY += 1;
						Board::MiniCursorY = 0;
					}
				}
				else {
					Board::MiniCursorY += 1;
				}
			}
			else {
				if (Board::CursorY + 1 <= Board::CurrentTileHeight - 1) {
					Board::CursorY += 1;
				}
			}
			break;
		case Controls::Right:
			if (ismarking) {
				if (Board::MiniCursorX + 1 >= Tile::FieldSize) {
					if (Board::CursorX + 1 <= Board::CurrentTileWidth - 1) {
						Board::CursorX += 1;
						Board::MiniCursorX = 0;
					}
				}
				else {
					Board::MiniCursorX += 1;
				}
			}
			else {
				if (Board::CursorX + 1 <= Board::CurrentTileWidth - 1) {
					Board::CursorX += 1;
				}
			}
			break;
		case Controls::Left:
			if (ismarking) {
				if (Board::MiniCursorX - 1 < 0) {
					if (Board::CursorX - 1 >= 0) {
						Board::CursorX -= 1;
						Board::MiniCursorX = Tile::FieldSize - 1;
					}
				}
				else {
					Board::MiniCursorX -= 1;
				}
			}
			else {
				if (Board::CursorX - 1 >= 0) {
					Board::CursorX -= 1;
				}
			}
			break;
		case Controls::Enter:
			if (ismarking) {
				if (Board::AddPoint()) {
					Board::NextStep();
					quit = true;
				}
			}
			else {
				quit = Board::Assign();
				if (quit) {
					Board::NextStep();
				}
			}
			break;
		case Controls::C:
			Board::AssignedY = -1;
			Board::AssignedX = -1;
			Board::Assigning = false;
			quit = true;
			break;
		}
		system("cls");
	}
}
std::string Board::getWinnerId() {
	return Board::WinnerId;
}
bool Board::isWin(int x, int y, int minix, int miniy, std::string mark) {
	int yNum = Board::NeighbourCheckY(x, y, minix, miniy, mark, 1) + Board::NeighbourCheckY(x, y, minix, miniy, mark, -1) - 1;
	int xNum = Board::NeighbourCheckX(x, y, minix, miniy, mark, 1) + Board::NeighbourCheckX(x, y, minix, miniy, mark, -1) - 1;
	int diagNum = Board::NeighbourCheckDiagonal(x, y, minix, miniy, mark, 1) + Board::NeighbourCheckDiagonal(x, y, minix, miniy, mark, -1) - 1;
	int diagNumInv = Board::NeighbourCheckDiagonalInverse(x, y, minix, miniy, mark, 1) + Board::NeighbourCheckDiagonalInverse(x, y, minix, miniy, mark, -1) - 1;

	if (yNum >= 5 || xNum >= 5 || diagNum >= 5 || diagNumInv >= 5) {
		Board::WinnerId = mark;
		return true;
	}
	return false;
}
int Board::NeighbourCheckDiagonal(int x, int y, int minix, int miniy, std::string mark, int step) {
	int counter = 0;
	if (mark == Board::Tiles[y][x].getFields(minix, miniy)->getId()) {
		counter += 1;
		if (minix + step < 0) {
			minix = 1;
			if (x + step >= 0 && x + step <= Board::CurrentTileWidth - 1 && Board::AllUsedTileNum() > 0) {
				x += step;
			}
			else {
				return counter;
			}
		}
		else if (minix + step > 1) {
			minix = 0;
			if (x + step >= 0 && x + step <= Board::CurrentTileWidth - 1 && Board::AllUsedTileNum() > 0) {
				x += step;
			}
			else {
				return counter;
			}
		}
		else {
			minix += step;
		}

		if (miniy + step < 0) {
			miniy = 1;
			if (y + step >= 0 && y + step <= Board::CurrentTileHeight - 1 && Board::AllUsedTileNum() > 0) {
				y += step;
			}
			else {
				return counter;
			}
		}
		else if (miniy + step > 1) {
			miniy = 0;
			if (y + step >= 0 && y + step <= Board::CurrentTileHeight - 1 && Board::AllUsedTileNum() > 0) {
				y += step;
			}
			else {
				return counter;
			}
		}
		else {
			miniy += step;
		}
		counter += Board::NeighbourCheckDiagonal(x, y, minix, miniy, mark, step);
		return counter;
	}
	else {
		return counter;
	}
}
int Board::NeighbourCheckDiagonalInverse(int x, int y, int minix, int miniy, std::string mark, int step) {
	int counter = 0;
	if (mark == Board::Tiles[y][x].getFields(minix, miniy)->getId()) {
		counter += 1;
		if (minix + step * -1 < 0) {
			minix = 1;
			if (x + step * -1 >= 0 && x + step * -1 <= Board::CurrentTileWidth - 1 && Board::AllUsedTileNum() > 0) {
				x += step * -1;
			}
			else {
				return counter;
			}
		}
		else if (minix + step * -1 > 1) {
			minix = 0;
			if (x + step * -1 >= 0 && x + step * -1 <= Board::CurrentTileWidth - 1 && Board::AllUsedTileNum() > 0) {
				x += step * -1;
			}
			else {
				return counter;
			}
		}
		else {
			minix += step * -1;
		}

		if (miniy + step < 0) {
			miniy = 1;
			if (y + step >= 0 && y + step <= Board::CurrentTileHeight - 1 && Board::AllUsedTileNum() > 0) {
				y += step;
			}
			else {
				return counter;
			}
		}
		else if (miniy + step > 1) {
			miniy = 0;
			if (y + step >= 0 && y + step <= Board::CurrentTileHeight - 1 && Board::AllUsedTileNum() > 0) {
				y += step;
			}
			else {
				return counter;
			}
		}
		else {
			miniy += step;
		}
		counter += Board::NeighbourCheckDiagonal(x, y, minix, miniy, mark, step);
		return counter;
	}
	else {
		return counter;
	}
}
int Board::NeighbourCheckX(int x, int y, int minix, int miniy, std::string mark, int step) {
	int counter = 0;
	if (mark == Board::Tiles[y][x].getFields(minix, miniy)->getId()) {
		counter += 1;
		if (minix + step < 0) {
			minix = 1;
			if (x + step >= 0 && x + step <= Board::CurrentTileWidth - 1 && Board::AllUsedTileNum() > 0) {
				x += step;
			}
			else {
				return counter;
			}
		}
		else if (minix + step > 1) {
			minix = 0;
			if (x + step >= 0 && x + step <= Board::CurrentTileWidth - 1 && Board::AllUsedTileNum() > 0) {
				x += step;
			}
			else {
				return counter;
			}
		}
		else {
			minix += step;
		}
		counter += Board::NeighbourCheckX(x, y, minix, miniy, mark, step);
		return counter;
	}
	else {
		return counter;
	}
}
int Board::NeighbourCheckY(int x, int y, int minix, int miniy, std::string mark, int step) {
	int counter = 0;
	if (mark == Board::Tiles[y][x].getFields(minix, miniy)->getId()) {
		counter += 1;
		if (miniy + step < 0) {
			miniy = 1;
			if (y + step >= 0 && y + step <= Board::CurrentTileHeight - 1 && Board::AllUsedTileNum() > 0) {
				y += step;
			}
			else {
				return counter;
			}
		}
		else if (miniy + step > 1) {
			miniy = 0;
			if (y + step >= 0 && y + step <= Board::CurrentTileHeight - 1 && Board::AllUsedTileNum() > 0) {
				y += step;
			}
			else {
				return counter;
			}
		}
		else {
			miniy += step;
		}
		counter += Board::NeighbourCheckY(x, y, minix, miniy, mark, step);
		return counter;
	}
	else {
		return counter;
	}
}
bool Board::doAssign() {

	Board::Tiles[CursorY][CursorX] = Board::Tiles[Board::AssignedY][Board::AssignedX];
	Board::Tiles[Board::AssignedY][Board::AssignedX].goNulled();

	//std::cout << "Kapcsolodó:" << isConnectedCounter(CursorX, CursorY, true) << std::endl;
	if (Board::AllUsedTileNum() == isConnectedCounter(CursorX, CursorY, true)) {
		Board::AssignedY = -1;
		Board::AssignedX = -1;
		Board::Assigning = false;
		Board::Sizing();
	}
	else {
		Board::Tiles[Board::AssignedY][Board::AssignedX] = Board::Tiles[CursorY][CursorX];
		Board::Tiles[CursorY][CursorX].goNulled();
		SetConsoleTextAttribute(hConsole, Colors::WhiteOnRed);
		std::cout << "All tiles need to be connect to all these" << std::endl;
		SetConsoleTextAttribute(hConsole, Colors::WhiteOnBlack);
		system("pause");
		return false;
	}


	system("pause");
	for (int i = 0; i < Tile::FieldSize; i++)
	{
		for (int j = 0; j < Tile::FieldSize; j++)
		{
			Board::Win = Board::isWin(CursorX, CursorY, i, j, Board::Players[Board::CurrentPlayerId]->getId());
			if (Board::Win) {
				return true;
			}
		}
	}
	int enemy = 0;
	if (Board::CurrentPlayerId == 0) {
		enemy = 1;
	}
	for (int i = 0; i < Tile::FieldSize; i++)
	{
		for (int j = 0; j < Tile::FieldSize; j++)
		{
			Board::Win = Board::isWin(CursorX, CursorY, i, j, Board::Players[enemy]->getId());
			if (Board::Win) {
				return true;
			}
		}
	}
	return true;
}
Player* Board::getPlayerById(int id) {
	return Board::Players[id];
}
bool Board::Assign() {
	if (!Assigning) {
		if (Board::Tiles[CursorY][CursorX].notEmpty() && hadNeighbour(CursorX,CursorY)!=4) {
			if (LasTMarkX != CursorX && LastMarkY != CursorY) {
				AssignedX = CursorX;
				AssignedY = CursorY;
				Assigning = true;
			}
			else {
				SetConsoleTextAttribute(hConsole, Colors::WhiteOnRed);
				std::cout << "You can't allow to move!" << std::endl;
				SetConsoleTextAttribute(hConsole, Colors::WhiteOnBlack);
				system("pause");
			}
		}
		else if (hadNeighbour(CursorX, CursorY) == 4) {
			SetConsoleTextAttribute(hConsole, Colors::WhiteOnRed);
			std::cout << "You can't allow to move!" << std::endl;
			SetConsoleTextAttribute(hConsole, Colors::WhiteOnBlack);
			system("pause");
		}
		else {
			SetConsoleTextAttribute(hConsole, Colors::WhiteOnRed);
			std::cout << "You can't allow to select it, because this tile is empty!"<<std::endl;
			SetConsoleTextAttribute(hConsole, Colors::WhiteOnBlack);
			system("pause");
		}
	}
	else {
		if (!Board::Tiles[CursorY][CursorX].notEmpty()) {
			switch (Board::Tiles[Board::AssignedY][Board::AssignedX].getNumberOfParachute())
			{
			case 1:
				if (((abs(Board::CursorX - Board::AssignedX) == 1 && abs(Board::CursorY - Board::AssignedY) == 0) ||
					(abs(Board::CursorX - Board::AssignedX) == 0 && abs(Board::CursorY - Board::AssignedY) == 1)) && Board::hadNeighbour(Board::CursorX, Board::CursorY)>0) {
					return Board::doAssign();
				}
				else {
					SetConsoleTextAttribute(hConsole, Colors::WhiteOnRed);
					std::cout << "You can't allow to move selected tile here! Choose a gold field!" << std::endl;
					SetConsoleTextAttribute(hConsole, Colors::WhiteOnBlack);
					system("pause");
				}
				break;
			case 2:
				if (((abs(Board::CursorX - Board::AssignedX) > 0 && abs(Board::CursorY - Board::AssignedY) == 0) ||
					(abs(Board::CursorX - Board::AssignedX) == 0 && abs(Board::CursorY - Board::AssignedY) > 0)) && Board::hadNeighbour(Board::CursorX, Board::CursorY)>0) {
					return Board::doAssign();
				}
				else {
					SetConsoleTextAttribute(hConsole, Colors::WhiteOnRed);
					std::cout << "You can't allow to move selected tile here! Choose a gold field!" << std::endl;
					SetConsoleTextAttribute(hConsole, Colors::WhiteOnBlack);
					system("pause");
				}
				break;
			case 3:
				if ((abs(Board::CursorX - Board::AssignedX) == abs(Board::CursorY - Board::AssignedY)) && Board::hadNeighbour(Board::CursorX, Board::CursorY)>0) {
					return Board::doAssign();
				}
				else {
					SetConsoleTextAttribute(hConsole, Colors::WhiteOnRed);
					std::cout << "You can't allow to move selected tile here! Choose a gold field!" << std::endl;
					SetConsoleTextAttribute(hConsole, Colors::WhiteOnBlack);
					system("pause");
				}
				break;
			case 4:
				if (((abs(Board::CursorX - Board::AssignedX) > 0 && abs(Board::CursorY - Board::AssignedY) == 0) ||
					(abs(Board::CursorX - Board::AssignedX) == 0 && abs(Board::CursorY - Board::AssignedY) > 0) ||
					(abs(Board::CursorX - Board::AssignedX) == abs(Board::CursorY - Board::AssignedY))) && Board::hadNeighbour(Board::CursorX, Board::CursorY)>0) {
					return Board::doAssign();
				}
				else {
					SetConsoleTextAttribute(hConsole, Colors::WhiteOnRed);
					std::cout << "You can't allow to move selected tile here! Choose a gold field!" << std::endl;
					SetConsoleTextAttribute(hConsole, Colors::WhiteOnBlack);
					system("pause");
				}
				break;
			}
		}
		else {
			SetConsoleTextAttribute(hConsole, Colors::WhiteOnRed);
			std::cout << "You can't allow to move selected tile here beacuse this tile is not empty! Choose an empty gold field!" << std::endl;
			SetConsoleTextAttribute(hConsole, Colors::WhiteOnBlack);
			system("pause");
		}
	}
	return false;
}