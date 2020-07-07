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

	Board::Rounds = 1;

	/*Create Tiles*/
	Board::CurrentTileWidth = MinTileWidth;
	Board::CurrentTileHeight = MinTileHeight;

	Board::Tiles = new Tile * [MinTileHeight];

	for (int i = 0; i < MinTileHeight; i++)
	{
		Board::Tiles[i] = new Tile[MinTileWidth];
	}

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
		((CursorY - 1 >= 0 && Board::Tiles[CursorY - 1][CursorX].notEmpty()) ||
			(CursorX - 1 >= 0 && Board::Tiles[CursorY][CursorX - 1].notEmpty()) ||
			(CursorY + 1 < Board::CurrentTileHeight - 1 && Board::Tiles[CursorY + 1][CursorX].notEmpty()) ||
			(CursorX + 1 < Board::CurrentTileWidth - 1 && Board::Tiles[CursorY][CursorX + 1].notEmpty()) ||
			Board::Tiles[CursorY][CursorX].notEmpty()) || Board::AllUsedTileNum() == 0) {
		Board::Tiles[CursorY][CursorX].setFields(Board::Players[Board::CurrentPlayerId], MiniCursorX, MiniCursorY);
		Board::Sizing();
		return true;
	}
	else {
		std::cout << "Hiba0";
		system("pause");
	}
	return false;
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
	/*std::cout << hstart << std::endl;
	std::cout << wstart << std::endl;
	std::cout << cwidth << std::endl;
	std::cout << cheight << std::endl;

	system("pause");*/
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
					SetConsoleTextAttribute(hConsole, Colors::BlackOnGreen);
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
	std::cout << "Current player is " << Board::Players[Board::CurrentPlayerId]->getId() << std::endl;
	std::cout << "Remaining parachutes is " << Board::Players[Board::CurrentPlayerId]->getRemainingParachute() << std::endl;
}
void Board::printFirstStep() {
	system("cls");
	Board::printBoard(false, false);
	std::cout << "Press M to marking a skydiver" << std::endl;
	if (Board::AllUsedTileNum() >= 2) {
		std::cout << "Press N to moving a tile" << std::endl;
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
void Board::printSecondStep() {
	//system("pause");
	system("cls");
	Board::printBoard(false, false);
	if (Board::AllUsedTileNum() >= 2) {
		std::cout << "Press N to moving a tile" << std::endl;
		std::cout << "Press S to skip this step" << std::endl;
		switch (_getch())
		{
		case Controls::S:
			//Board::printBoard(true, true);
			//Board::Moving(true);
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
			quit = true;
			break;
		}

		system("cls");
		std::cout << "X: " << CursorX << std::endl;
		std::cout << "Y: " << CursorY << std::endl;
		std::cout << "mX: " << MiniCursorX << std::endl;
		std::cout << "mY: " << MiniCursorY << std::endl;
	}
}
bool Board::Assign() {
	if (!Assigning) {
		if (Board::Tiles[CursorY][CursorX].notEmpty()) {
			AssignedX = CursorX;
			AssignedY = CursorY;
			Assigning = true;
		}
		else {
			std::cout << "Hiba1";
			system("pause");
		}
	}
	else {
		if (!Board::Tiles[CursorY][CursorX].notEmpty()) {
			switch (Board::Tiles[Board::AssignedY][Board::AssignedX].getNumberOfParachute())
			{
			case 1:
				/*if()*/
				break;
			case 2:
				break;
			case 3:
				break;
			case 4:
				break;
			}
			Board::Tiles[CursorY][CursorX] = Board::Tiles[Board::AssignedY][Board::AssignedX];
			Board::Tiles[Board::AssignedY][Board::AssignedX].goNulled();
			Board::AssignedY = -1;
			Board::AssignedX = -1;
			Board::Assigning = false;
			Board::Sizing();
			return true;
		}
		else {
			std::cout << "Hiba2";
			system("pause");
		}
	}
	return false;
}