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
	bool end = false;
	while (!win && !end) {
		/*std::cout <<"Current step:" <<board->getCurrentStep()<<std::endl;
		std::cout << "Current player:" << board->getCurrentPlayer()->getId() << std::endl;*/
		switch (board->getCurrentStep())
		{
		case 0:
			board->printFirstStep();
			win = board->getWin();
			break;
		case 1:
			board->printSecondStep();
			win = board->getWin();
			break;
		}
		if (board->getPlayerById(0)->getRemainingParachute() == 0 && board->getPlayerById(1)->getRemainingParachute() == 0) {
			end = true;
		}
		
		//win = true;
	}
	std::cout <<board->getWinnerId();
	for (int i = 0; i < 255; i++)
	{
		SetConsoleTextAttribute(hConsole, i);
		std::cout << i << std::endl;
	}

}
