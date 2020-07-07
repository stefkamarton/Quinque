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
		std::cout <<"Current step:" <<board->getCurrentStep()<<std::endl;
		std::cout << "Current player:" << board->getCurrentPlayer()->getId() << std::endl;
		switch (board->getCurrentStep())
		{
		case 0:
			
			board->printFirstStep();
			break;
		case 1:
			board->printSecondStep();
			break;
		}
	}

	for (int i = 0; i < 255; i++)
	{
		SetConsoleTextAttribute(hConsole, i);
		std::cout << i << std::endl;
	}

}
