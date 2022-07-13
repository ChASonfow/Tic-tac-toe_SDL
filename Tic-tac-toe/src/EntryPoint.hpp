#pragma once

#include "Game.h"

namespace TicTacToe
{
	static void Play()
	{
		Game ticTacToe{ "Tic-Tac-Toe game", 800, 600 };
		ticTacToe.Run();
	}
}