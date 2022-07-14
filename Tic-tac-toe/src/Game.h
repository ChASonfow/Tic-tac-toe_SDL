#pragma once
#include "SDL_Handler.h"

class Game : public SDL_Handler
{
	enum class GameState
	{
		STOP = 0,
		MAIN_MENU = 1,
		FIELD_MENU = 2,
		PLAYERS_MENU = MAIN_MENU | FIELD_MENU,
		GAMEPLAY = 4,
		RESULT = GAMEPLAY | MAIN_MENU
	};

	enum class GameMode
	{
		IDLE = 0,
		SMALL_FIELD = 1,
		LARGE_FIELD = 2,
		PLAYER_VS_PLAYER = 4,
		PLAYER_VS_AI = 8,
		AI_VS_AI = 16,

		STANDART_PVP  = SMALL_FIELD | PLAYER_VS_PLAYER,
		STANDART_SOLO = SMALL_FIELD | PLAYER_VS_AI,
		STANDART_AUTO = SMALL_FIELD | AI_VS_AI,

		EXTENDED_PVP  = LARGE_FIELD | PLAYER_VS_PLAYER,
		EXTENDED_SOLO = LARGE_FIELD | PLAYER_VS_AI,
		EXTENDED_AUTO = LARGE_FIELD | AI_VS_AI
	};

	struct RectObject
	{
		~RectObject()
		{
			SDL_DestroyTexture(texture);
		}

		inline bool IsClicked(const int32_t& x, const int32_t& y) noexcept
		{
			return x >= rect.x && x <= (rect.x + rect.w) && y >= rect.y && y <= rect.y + rect.h;
		}

		std::string name;
		SDL_Rect rect;
		bool active;

		void(*action)(GameState&, GameMode&);
		SDL_Texture* texture;
	};

public:
	Game(const std::string& title, const uint16_t& width, const uint16_t& height);
	virtual ~Game();

	virtual void Run() override;

protected:
	virtual bool Init() override;
	virtual void Clear() override;

	virtual void HandleEvents(const SDL_Event& ev);
	virtual void HandleMouseEvents(const SDL_Event& ev);
	virtual void HandleKeyboardEvents(const SDL_Event& ev);

private:
	void Update(const float& dt);

	void Render();
	void Render(const float& dt);
	void RenderUI();
	void RenderUI(const float& dt);
	void RenderGame(float dt);

	SDL_Texture* GetTextTexture(const RectObject& obj, const SDL_Color& textColor);

	template<class T>
	int16_t FindClickedRectObject(T& container, const int32_t& x, const int32_t& y)
	{
		for (size_t i = 0; i < container.size(); ++i)
		{
			if (container[i].IsClicked(x, y) && container[i].active)
			{
				return i;
			}
		}
		return -1;
	}

private:
	TTF_Font* m_font;

	GameState m_state;
	GameMode m_mode;
	std::array<RectObject, 8> m_uiButtons;
	std::vector<RectObject> m_playField;
};
