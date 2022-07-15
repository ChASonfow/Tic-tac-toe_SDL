#include "Game.h"

#pragma region Constructor & Destructor
Game::Game(const std::string& title, const Uint16& width, const Uint16& height)
	: SDL_Handler(title, width, height), m_state(GameState::MAIN_MENU), m_mode(GameMode::IDLE)
	, m_bgRect({}), m_bgColor({}), m_labels()
{
	m_running = Game::Init();
}

Game::~Game()
{
	Game::Clear();
}
#pragma endregion

#pragma region Inits & Clear
void Game::Clear()
{
	m_uiButtons.clear();
	m_labels.clear();
	m_playField.clear();

	TTF_CloseFont(m_font);
	TTF_Quit();
}

bool Game::Init()
{
	CATCH_ERROR(TTF_Init(), "TTF_Init");
	CATCH_ERROR(!(m_font = TTF_OpenFont("res/unispace.ttf", 28)), "TTF_OpenFont");

	return InitUI() && InitLabels();
}

bool Game::InitUI()
{
	const Uint16 rectX = m_width * 0.45, rectY = m_height * 0.4;
	const Uint16 rectW = m_width * 0.1, rectH = m_height * 0.1;
	const SDL_Color uiTextColor{ 0, 200, 255, 255 };

	m_uiButtons = {
		{ "START", { rectX, rectY, rectW, rectH }, true, nullptr
			, [](GameState& state, GameMode& mode) { state = GameState::FIELD_MENU; mode = GameMode::IDLE; }},
		{ "EXIT", { rectX, rectY + rectH, rectW, rectH }, true, nullptr
			, [](GameState& state, GameMode& mode) { state = GameState::STOP; mode = GameMode::IDLE; }},
		{ "3 X 3", { rectX, rectY, rectW, rectH }, false, nullptr
			, [](GameState& state, GameMode& mode) { state = GameState::PLAYERS_MENU; mode = GameMode::SMALL_FIELD; }},
		{ "5 X 5", { rectX, rectY + rectH, rectW, rectH }, false, nullptr
			, [](GameState& state, GameMode& mode) { state = GameState::PLAYERS_MENU; mode = GameMode::LARGE_FIELD; }},
		{ "PVP", { rectX, rectY, rectW, rectH }, false, nullptr
			, [](GameState& state, GameMode& mode) {
				state = GameState::TEAM_MENU;
				mode = static_cast<GameMode>(static_cast<Uint8>(mode) | static_cast<Uint8>(GameMode::PLAYER_VS_PLAYER));
			}},
		{ "SOLO", { rectX, rectY + rectH, rectW, rectH }, false, nullptr
			, [](GameState& state, GameMode& mode) {
				state = GameState::TEAM_MENU;
				mode = static_cast<GameMode>(static_cast<Uint8>(mode) | static_cast<Uint8>(GameMode::PLAYER_VS_AI));
			}},
		{ "AUTO", { rectX, rectY + rectH * 2, rectW, rectH }, false, nullptr
			, [](GameState& state, GameMode& mode) {
				state = GameState::PREGAMEPLAY;
				mode = static_cast<GameMode>(static_cast<Uint8>(mode) | static_cast<Uint8>(GameMode::AI_VS_AI));
			}},
		{"CIRCLES", { rectX, rectY, rectW, rectH }, false, nullptr
			, [](GameState& state, GameMode& mode) { state = GameState::PREGAMEPLAY; }},
		{"SQUARES", { rectX, rectY + rectH, rectW, rectH }, false, nullptr
			, [](GameState& state, GameMode& mode) { state = GameState::PREGAMEPLAY; }},
		{"MENU", { rectX, rectY + rectH * 3, rectW, rectH }, false, nullptr
			, [](GameState& state, GameMode& mode) { state = GameState::MAIN_MENU; mode = GameMode::IDLE; }},
	};

	for (Uint8 i = 0; i < m_uiButtons.size(); ++i)
	{
		CATCH_ERROR(!(m_uiButtons.at(i).texture = CreateTextTexture(m_uiButtons.at(i), uiTextColor))
			, (m_uiButtons.at(i).name + " button creating").c_str());
	}

	return true;
}

SDL_Texture* Game::CreateTextTexture(const LabelObject& obj, const SDL_Color& textColor)
{
	SDL_Surface* textSurface = nullptr;
	SDL_Texture* textTexture = nullptr;

	if (	!(textSurface = TTF_RenderText_Solid(m_font, obj.name.c_str(), textColor)) 
		||	!(textTexture = SDL_CreateTextureFromSurface(m_renderer, textSurface)))
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "%s texture in CreateTextTexture() Error: %s"
			, obj.name.c_str(), SDL_GetError());
	}

	SDL_FreeSurface(textSurface);
	return textTexture;
}

bool Game::InitLabels()
{
	const Uint16 rectX = m_width * 0.3, rectY = m_height * 0.1;
	const Uint16 rectW = m_width * 0.4, rectH = m_height * 0.15;
	const SDL_Color labelTextColor{ 0, 100, 255, 255 };

	m_labels = {
		{"TIC-TAC-TOE", {rectX, rectY, rectW, rectH}, true, nullptr},
		{"CREATED BY CH.A.S.", {rectX, m_height - rectY, rectW, rectH / 3}, true, nullptr },
		{"X-PLAYER TURN", {rectX, rectY, rectW, rectH}, false, nullptr },
		{"O-PLAYER TURN", {rectX, rectY, rectW, rectH}, false, nullptr },
		{"X-PLAYER WIN", {rectX, rectY * 3, rectW, rectH}, false, nullptr },
		{"O-PLAYER WIN", {rectX, rectY * 3, rectW, rectH}, false, nullptr }
	};

	for (Uint8 i = 0; i < m_labels.size(); ++i)
	{
		CATCH_ERROR(!(m_labels.at(i).texture = CreateTextTexture(m_labels.at(i), labelTextColor))
			, (m_labels.at(i).name + " label creating").c_str());
	}

	return true;
}

bool Game::InitPlayfield()
{
	if (m_state == GameState::PREGAMEPLAY)
	{
		const Uint8 lineSize = static_cast<Uint8>(m_mode) & static_cast<Uint8>(GameMode::SMALL_FIELD)
			? 3 : 5;
		const Uint8 fieldSize = lineSize * lineSize;
		m_playField.resize(fieldSize);

		const Uint16 bgRectX = m_width * 0.2, bgRectY = m_height * 0.2;
		const Uint16 bgRectW = m_width * 0.6, bgRectH = m_height * 0.6;
		m_bgRect = { bgRectX, bgRectY, bgRectW, bgRectH };
		m_bgColor = { 0, 153, 153, 255 };
		
		const Uint16 tileX = bgRectX, tileY = bgRectY;
		const Uint16 tileGapX = bgRectW * 0.02, tileGapY = bgRectH * 0.02;
		const Uint16 tileW = (bgRectW - tileGapX * (lineSize - 1)) / lineSize, tileH = (bgRectH - tileGapY * (lineSize - 1)) / lineSize;
		SDL_Color emptyTileColor{ 0, 0, 0, 255 };
		
		for (Uint16 i = 0, row = 0; i < fieldSize && row < lineSize; ++row)
		{
			for (Uint16 col = 0; col < lineSize; ++i, ++col)
			{
				m_playField[i].name = std::to_string(i);
				m_playField[i].rect = { tileX + (tileW + tileGapX + 1) * col, tileY + (tileH + tileGapY + 1) * row, tileW, tileH };
				m_playField[i].active = true;
				m_playField[i].color = std::make_shared<SDL_Color>(emptyTileColor);
				m_playField[i].action = []() {};
			}
		}
	}
	return true;
}
#pragma endregion

void Game::Run()
{
	/*uint64_t lastTime = 0;
	uint64_t currentTime = SDL_GetPerformanceCounter();
	double deltaTime = static_cast<double>(currentTime - lastTime) * 1000.0 / static_cast<double>(SDL_GetPerformanceFrequency());*/

	SDL_Event ev{};

	while (m_running)
	{
		/*
		currentTime = SDL_GetPerformanceCounter();
		deltaTime = static_cast<double>(currentTime - lastTime) * 1000.0 / static_cast<double>(SDL_GetPerformanceFrequency()); 
		*/
		while (SDL_PollEvent(&ev))
		{
			HandleEvents(ev);
		}

		Update();
		Render();

		//lastTime = currentTime;
	}
}

void Game::Update()
{
	switch (m_state)
	{
	case GameState::PREGAMEPLAY:
		if (!InitPlayfield())
		{
			SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failure to init play field!");
			m_state = GameState::STOP;
			break;
		}
		Render();
		m_state = GameState::GAMEPLAY;
		break;
	case GameState::GAMEPLAY:
		//TODO:
		break;
	case GameState::RESULT:
		if (!m_playField.empty())
		{
			m_playField.clear();
		}
		break;
	case GameState::STOP:
		m_running = false;
		return;
	}
}

#pragma region Render
void Game::Render()
{
	SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 0);
	SDL_RenderClear(m_renderer);

	RenderUI();
	RenderLabels();
	RenderField();

	SDL_RenderPresent(m_renderer);
}

void Game::RenderUI()
{
	switch (m_state)
	{
	case GameState::MAIN_MENU:
		for (size_t i = 0; i < m_uiButtons.size(); ++i)
		{
			m_uiButtons.at(i).active = i < 2;
		}
		break;
	case GameState::FIELD_MENU:
		for (size_t i = 0; i < m_uiButtons.size(); ++i)
		{
			m_uiButtons.at(i).active = (i >= 2 && i < 4) || i == 9;
		}
		break;
	case GameState::PLAYERS_MENU:
		for (size_t i = 0; i < m_uiButtons.size(); ++i)
		{
			m_uiButtons.at(i).active = (i >= 4 && i < 7) || i == 9;
		}
		break;
	case GameState::TEAM_MENU:
		for (size_t i = 0; i < m_uiButtons.size(); ++i)
		{
			m_uiButtons.at(i).active = i >= 7;
		}
		break;
	case GameState::PREGAMEPLAY:
		for (size_t i = 0; i < m_uiButtons.size(); ++i)
		{
			m_uiButtons.at(i).active = false;
		}
		break;
	case GameState::RESULT:
		for (size_t i = 0; i < m_uiButtons.size(); ++i)
		{
			m_uiButtons.at(i).active = i == 9;
		}
		break;
	case GameState::STOP:
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

void Game::RenderLabels()
{
	switch (m_state)
	{
	case GameState::MAIN_MENU:
	case GameState::FIELD_MENU:
	case GameState::PLAYERS_MENU:
	case GameState::TEAM_MENU:
		for (Uint8 i = 0; i < m_labels.size(); ++i)
		{
			m_labels.at(i).active = i < 2;
		}
		break;
	case GameState::PREGAMEPLAY:
		for (Uint8 i = 0; i < m_labels.size(); ++i)
		{
			m_labels.at(i).active = false;
		}
		break;
	case GameState::GAMEPLAY:
		//TODO:
		break;
	case GameState::RESULT:
		//TODO:
		break;
	case GameState::STOP:
		return;
	}

	for (auto& label: m_labels)
	{
		if (label.active)
		{
			SDL_RenderCopy(m_renderer, label.texture, nullptr, &label.rect);
		}
	}
}

void Game::RenderField()
{
	if (m_state == GameState::GAMEPLAY)
	{
		SDL_SetRenderDrawColor(m_renderer, m_bgColor.r, m_bgColor.g, m_bgColor.b, m_bgColor.a);
		SDL_RenderFillRect(m_renderer, &m_bgRect);

		for (auto& tile : m_playField)
		{
			if (tile.active)
			{
				SDL_SetRenderDrawColor(m_renderer, tile.color->r, tile.color->g, tile.color->b, tile.color->a);
				SDL_RenderFillRect(m_renderer, &tile.rect);
			}
		}
	}
}
#pragma endregion

#pragma region Handles
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
		case GameState::MAIN_MENU:		// fallthrough
		case GameState::FIELD_MENU:		// fallthrough
		case GameState::PLAYERS_MENU:	// fallthrough
		case GameState::TEAM_MENU:		// fallthrough
		case GameState::RESULT:
			if ((index = FindClickedRectObject(m_uiButtons, ev.button.x, ev.button.y)) != -1)
			{
				m_uiButtons[index].action(m_state, m_mode);
			}
			break;
		case GameState::GAMEPLAY:
			if ((index = FindClickedRectObject(m_playField, ev.button.x, ev.button.y)) != -1)
			{
				m_playField[index].action();
			}
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
#pragma endregion