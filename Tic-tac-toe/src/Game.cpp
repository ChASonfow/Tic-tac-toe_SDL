#include "Game.h"

Game::Game(const std::string& title, const uint16_t& width, const uint16_t& height)
	: SDL_Handler(title, width, height), m_state(GameState::MAIN_MENU), m_mode(GameMode::IDLE)
{
	m_running = Game::Init();
}

Game::~Game()
{
	Game::Clear();
}

bool Game::Init()
{
	CATCH_ERROR(TTF_Init(), "TTF_Init");
	CATCH_ERROR(!(m_font = TTF_OpenFont("res/unispace.ttf", 28)), "TTF_OpenFont");

#pragma region UI initialization
	const uint16_t rectX = m_width * 0.45, rectY = m_height * 0.3;
	const uint16_t rectW = m_width * 0.1, rectH = m_height * 0.1;
	const SDL_Color textColor{ 0, 204, 255, 255 };

	m_uiButtons[0] = { "START", {rectX, rectY, rectW, rectH}, true
		, [](GameState& state, GameMode& mode) { state = GameState::FIELD_MENU; mode = GameMode::IDLE; } };
	CATCH_ERROR(!(m_uiButtons[0].texture = GetTextTexture(m_uiButtons[0], textColor))
		, m_uiButtons[0].name + " button GetTextTexture");

	m_uiButtons[1] = { "EXIT", {rectX, rectY + rectH, rectW, rectH}, true
		, [](GameState& state, GameMode& mode) { state = GameState::STOP; mode = GameMode::IDLE; } };
	CATCH_ERROR(!(m_uiButtons[1].texture = GetTextTexture(m_uiButtons[1], textColor))
		, m_uiButtons[1].name + " button GetTextTexture");

	m_uiButtons[2] = { "3 X 3", {rectX, rectY, rectW, rectH}, false
		, [](GameState& state, GameMode& mode) { state = GameState::PLAYERS_MENU; mode = GameMode::SMALL_FIELD; } };
	CATCH_ERROR(!(m_uiButtons[2].texture = GetTextTexture(m_uiButtons[2], textColor))
		, m_uiButtons[2].name + " button GetTextTexture");

	m_uiButtons[3] = { "5 X 5", {rectX, rectY + rectH, rectW, rectH}, false
		, [](GameState& state, GameMode& mode) { state = GameState::PLAYERS_MENU; mode = GameMode::LARGE_FIELD; } };
	CATCH_ERROR(!(m_uiButtons[3].texture = GetTextTexture(m_uiButtons[3], textColor))
		, m_uiButtons[3].name + " button GetTextTexture");

	m_uiButtons[4] = { "PVP", {rectX, rectY, rectW, rectH}, false
		, [](GameState& state, GameMode& mode) { 
			state = GameState::GAMEPLAY; 
			mode = static_cast<GameMode>(static_cast<uint16_t>(mode) | static_cast<uint16_t>(GameMode::PLAYER_VS_PLAYER)); 
	} };
	CATCH_ERROR(!(m_uiButtons[4].texture = GetTextTexture(m_uiButtons[4], textColor))
		, m_uiButtons[4].name + " button GetTextTexture");

	m_uiButtons[5] = { "SOLO", {rectX, rectY + rectH, rectW, rectH}, false
		, [](GameState& state, GameMode& mode) {
			state = GameState::GAMEPLAY;
			mode = static_cast<GameMode>(static_cast<uint16_t>(mode) | static_cast<uint16_t>(GameMode::PLAYER_VS_AI));
	} };
	CATCH_ERROR(!(m_uiButtons[5].texture = GetTextTexture(m_uiButtons[5], textColor))
		, m_uiButtons[5].name + " button GetTextTexture");

	m_uiButtons[6] = { "AUTO", {rectX, rectY + rectH * 2, rectW, rectH}, false
		, [](GameState& state, GameMode& mode) {
			state = GameState::GAMEPLAY;
			mode = static_cast<GameMode>(static_cast<uint16_t>(mode) | static_cast<uint16_t>(GameMode::AI_VS_AI));
	} };
	CATCH_ERROR(!(m_uiButtons[6].texture = GetTextTexture(m_uiButtons[6], textColor))
		, m_uiButtons[6].name + " button GetTextTexture");

	m_uiButtons[7] = { "MENU", {rectX, rectY + rectH * 3, rectW, rectH}, false
		, [](GameState& state, GameMode& mode) { state = GameState::MAIN_MENU; mode = GameMode::IDLE; } };
	CATCH_ERROR(!(m_uiButtons[7].texture = GetTextTexture(m_uiButtons[7], textColor))
		, m_uiButtons[7].name + " button GetTextTexture");
#pragma endregion

	return true;
}


SDL_Texture* Game::GetTextTexture(const RectObject& obj, const SDL_Color& textColor)
{
	SDL_Surface* textSurface = nullptr;
	SDL_Texture* textTexture = nullptr;

	if (	!(textSurface = TTF_RenderText_Solid(m_font, obj.name.c_str(), textColor)) 
		||	!(textTexture = SDL_CreateTextureFromSurface(m_renderer, textSurface)))
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "%s Error: %s"
			, obj.name + " button TTF_RenderText_Solid or SDL_CreateTextureFromSurface", SDL_GetError());
	}

	SDL_FreeSurface(textSurface);
	return textTexture;
}

void Game::Clear()
{
	m_playField.clear();

	TTF_CloseFont(m_font);
	TTF_Quit();
}

void Game::Run()
{
	SDL_Event ev{};

	while (m_running)
	{
		while (SDL_PollEvent(&ev))
		{
			HandleEvents(ev);
		}
		Render();
	}
}

void Game::Render()
{
	static uint64_t lastTime = 0;
	const uint64_t currentTime = SDL_GetPerformanceCounter();
	const double deltaTime = static_cast<double>(currentTime - lastTime) * 1000.0 / static_cast<double>(SDL_GetPerformanceFrequency());

	SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 0);
	SDL_RenderClear(m_renderer);

	RenderUI();

	SDL_RenderPresent(m_renderer);
	lastTime = currentTime;
}

void Game::RenderUI()
{
	switch (m_state)
	{
	case Game::GameState::MAIN_MENU:
		for (size_t i = 0; i < m_uiButtons.size(); ++i)
		{
			m_uiButtons[i].active = i < 2;
		}
		break;
	case Game::GameState::FIELD_MENU:
		for (size_t i = 0; i < m_uiButtons.size(); ++i)
		{
			m_uiButtons[i].active = (i >= 2 && i < 4) || i == 7;
		}
		break;
	case Game::GameState::PLAYERS_MENU:
		for (size_t i = 0; i < m_uiButtons.size(); ++i)
		{
			m_uiButtons[i].active = i >= 4;
		}
		break;
	case Game::GameState::RESULT:
		for (size_t i = 0; i < m_uiButtons.size(); ++i)
		{
			m_uiButtons[i].active = i == 7;
		}
		break;
	case Game::GameState::GAMEPLAY:
		for (size_t i = 0; i < m_uiButtons.size(); ++i)
		{
			m_uiButtons[i].active = false;
		}
		break;
	case Game::GameState::STOP:
		m_running = false;
		return;
	}

	for (auto& button : m_uiButtons)
	{
		if (button.active)
		{
			SDL_RenderCopy(m_renderer, button.texture, nullptr, &button.rect);
		}
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
		HandleKeyboardEvents(ev);
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
	if (ev.button.button == SDL_BUTTON_LEFT)
	{
		int16_t index = 0;

		switch (m_state)
		{
		case Game::GameState::MAIN_MENU:	// fallthrough
		case Game::GameState::FIELD_MENU:	// fallthrough
		case Game::GameState::PLAYERS_MENU:	// fallthrough
		case Game::GameState::RESULT:
			if ((index = FindClickedRectObject(m_uiButtons, ev.button.x, ev.button.y)) != -1)
			{
				m_uiButtons[index].action(m_state, m_mode);
			}
			break;
		case Game::GameState::GAMEPLAY:
			if ((index = FindClickedRectObject(m_playField, ev.button.x, ev.button.y)) != -1)
			{
				m_playField[index].action(m_state, m_mode);
			}
			break;
		default:
			break;
		}
	}
}

void Game::HandleKeyboardEvents(const SDL_Event& ev)
{
	if (ev.key.keysym.sym == SDLK_ESCAPE)
	{
		m_running = false;
	}
}

