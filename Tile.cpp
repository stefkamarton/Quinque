#include "Tile.h"

Tile::Tile() {
	Tile::Fields.resize(FieldSize);
	for (int i = 0; i < Tile::Fields.size(); i++)
	{
		Tile::Fields[i].resize(FieldSize);
		for (int j = 0; j < Tile::Fields[i].size(); j++)
		{
			Tile::Fields[i][j] = new Player(0);
		}
	}

}
Tile::Tile(const Tile& tile) {
	Tile::Fields.resize(FieldSize);
	for (int i = 0; i < Tile::Fields.size(); i++)
	{
		Tile::Fields[i].resize(FieldSize);
		for (int j = 0; j < Tile::Fields[i].size(); j++)
		{
			Tile::Fields[i][j] = tile.Fields[i][j];
		}
	}
}
void Tile::setFields(Player* cPlayer, int x, int y) {
	if (cPlayer->AddParachute()) {
		Fields[y][x] = cPlayer;
	}
	
}
Player* Tile::getFields(int x, int y) {
	return Tile::Fields[y][x];
}
bool Tile::notEmpty() {
	for (int i = 0; i < Tile::FieldSize; i++)
	{
		for (int j = 0; j < Tile::FieldSize; j++)
		{
			if (Tile::Fields[i][j]->getId() != " ") {
				return true;
			}
		}
	}
	return false;
}
int Tile::getNumberOfParachute() {
	int num = 0;
	for (int i = 0; i < Tile::FieldSize; i++)
	{
		for (int j = 0; j < Tile::FieldSize; j++)
		{
			if (Tile::Fields[i][j]->getId() != " ") {
				num += 1;
			}
		}
	}
	return num;
}
void Tile::printLine(int linenum, bool justmini, int minicursor, bool secondgreen) {
	CONSOLE_SCREEN_BUFFER_INFO info;
	CONSOLE_SCREEN_BUFFER_INFO begin;
	CONSOLE_SCREEN_BUFFER_INFO bp;
	GetConsoleScreenBufferInfo(hConsole, &begin);
	for (int i = 0; i < Tile::FieldSize; i++)
	{
		if (((justmini && i == minicursor) || !justmini) || begin.wAttributes==Colors::BlackOnGreen) {
			SetConsoleTextAttribute(hConsole, begin.wAttributes);
		}
		else if (i != minicursor && secondgreen) {
			SetConsoleTextAttribute(hConsole, Colors::BlackOnGreen);
		}
		else {
			SetConsoleTextAttribute(hConsole, Colors::WhiteOnBlack);
		}
		if (Tile::Fields[linenum][i] == 0) {
			std::cout << "   ";
		}
		else {
			GetConsoleScreenBufferInfo(hConsole, &bp);
			if (Tile::Fields[linenum][i]->getId() == "X") {
				SetConsoleTextAttribute(hConsole, Colors::RedOnBlack);
				if (bp.wAttributes == Colors::BlackOnCyan) {
					SetConsoleTextAttribute(hConsole, Colors::RedOnCyan);
				}
			}
			else if (Tile::Fields[linenum][i]->getId() == "O") {
				SetConsoleTextAttribute(hConsole, Colors::BlueOnBlack);
				if (bp.wAttributes == Colors::BlackOnCyan) {
					SetConsoleTextAttribute(hConsole, Colors::BlueOnCyan);
				}
			}
			std::cout << " " << Tile::Fields[linenum][i]->getId() << " ";
			SetConsoleTextAttribute(hConsole, bp.wAttributes);

		}

		if (justmini && i == minicursor) {
			SetConsoleTextAttribute(hConsole, Colors::WhiteOnBlack);
		}
		GetConsoleScreenBufferInfo(hConsole, &info);
		SetConsoleTextAttribute(hConsole, Colors::WhiteOnBlack);
		if (Tile::FieldSize - 1 != i) {
			std::cout << char(179);
		}
		
		SetConsoleTextAttribute(hConsole, info.wAttributes);

	}
	
	SetConsoleTextAttribute(hConsole, Colors::WhiteOnBlack);
}