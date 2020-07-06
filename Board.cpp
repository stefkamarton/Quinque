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

	/*Set Current Player*/
	Board::CurrentPlayerId = 0;

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
		return true;
	}
	else {
		std::cout << "Hiba0";
		system("pause");
	}
	return false;
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