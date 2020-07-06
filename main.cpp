#include <iostream>
#include <Windows.h>
#include <conio.h>
#include "Colors.h"
#include "Board.h"
#include "Player.h"

int main()
{

	Board* board = Board::getInstance();
	/*std::cout << "Enter a character";
	std::cout << _getch();*/
	bool win = false;
	while (!win) {
		board->Moving(false);
	}

	for (int i = 0; i < 255; i++)
	{
		SetConsoleTextAttribute(hConsole, i);
		std::cout << i << std::endl;
	}

}
