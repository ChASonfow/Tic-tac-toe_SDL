#pragma once
#include "SDL_Handler.h"

class Game : public SDL_Handler
{
	typedef unsigned int uint16_t;

	enum class GameState
	{
		STOP = 0,
		MAIN_MENU = 1,
		FIELD_MENU = 2,
		PLAYERS_MENU = MAIN_MENU | FIELD_MENU,
		GAMEPLAY = 4,
		RESULT = GAMEPLAY | MAIN_MENU
	};

public:
	Game(const std::string& title, const uint16_t& width, const uint16_t& height);
	~Game();

	void Run() override;

protected:
	bool Init() override;
	void Clear() override;

private:
	void Update(const float& dt);

	void Render();
	void Render(const float& dt);
	void RenderMenu(float dt);
	void RenderGame(float dt);

	void HandleEvents(const SDL_Event& ev);
	void HandleMouseEvents(const SDL_Event& ev);
	void HandleKeyBoardEvents(const SDL_Event& ev);

private:
	SDL_Surface* m_bgSrfc;
	SDL_Texture* m_bgTex;

	GameState m_state;
};
