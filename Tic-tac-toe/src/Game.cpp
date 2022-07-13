#include "Game.h"
#include <SDL.h>

Game::Game(const std::string& title, const uint16_t& width, const uint16_t& height)
	: SDL_Handler(title, width, height), m_state(GameState::MAIN_MENU)
{
	m_running = Game::Init();
}

Game::~Game()
{
	Game::Clear();
}

bool Game::Init()
{


	return m_running;
}

void Game::Clear()
{

}

void Game::Run()
{
	SDL_Event ev{};
	SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 0xFF);

	while (m_running)
	{
		while (SDL_PollEvent(&ev))
		{
			HandleEvents(ev);
		}
		SDL_RenderClear(m_renderer);
		//Render();
		SDL_RenderPresent(m_renderer);
	}
}

void Game::HandleEvents(const SDL_Event& ev)
{
	switch (ev.type)
	{
	case SDL_QUIT:
		m_running = false;
		break;
	case SDL_KEYDOWN:
		HandleKeyBoardEvents(ev);
		break;
	case SDL_MOUSEBUTTONDOWN:
		HandleMouseEvents(ev);
		break;
	default:
		break;
	}
}

void Game::HandleMouseEvents(const SDL_Event& ev)
{
	switch (ev.button.button)
	{
	case SDL_BUTTON_LEFT:
		break;
	default:
		break;
	}
}

void Game::HandleKeyBoardEvents(const SDL_Event& ev)
{
	switch (ev.key.keysym.sym)
	{
	case SDLK_ESCAPE:
		m_running = false;
		break;
	default:
		break;
	}
}