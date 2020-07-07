#include <iostream>
#include <Windows.h>
#include <conio.h>
#include "Colors.h"
#include "Board.h"
#include "Player.h"

int main()
{
	Board* board = Board::getInstance();
	bool win = false;
	bool end = false;
	while (!win && !end) {
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
	}
	if (win == true) {

		if (board->getWinnerId() == "X") {
			SetConsoleTextAttribute(hConsole, Colors::RedOnGold);
		}
		else {
			SetConsoleTextAttribute(hConsole, Colors::BlueOnGold);
		}
		std::cout << "Winner is: ";
		std::cout << board->getWinnerId() << std::endl;
		SetConsoleTextAttribute(hConsole, Colors::WhiteOnBlack);

	}
	else if (end == true) {
		SetConsoleTextAttribute(hConsole, Colors::WhiteOnRed);
		std::cout << "GAME OVER";
		SetConsoleTextAttribute(hConsole, Colors::WhiteOnBlack);
	}
}