#pragma once
#include "Game.h"

namespace TicTacToe
{
	static const char* APP_TITLE	= "Tic-tac-toe game";
	static const int SCREEN_WIDTH	= 800;
	static const int SCREEN_HEIGHT	= 600;

	static void Play()
	{
		Game instance{ APP_TITLE, SCREEN_WIDTH, SCREEN_HEIGHT };
		instance.Run();
	}
}