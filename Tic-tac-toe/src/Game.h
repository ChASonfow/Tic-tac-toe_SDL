#pragma once
#include "SDL_Handler.h"
#include "Controller.h"
#include "RectObject.hpp"

class Game : public SDL_Handler
{
public:
	Game(const std::string& title, const Uint16& width, const Uint16& height);
	virtual ~Game();

	virtual void Run() override;

protected:
	virtual void Clear() override;

	virtual bool Init() override;
	virtual bool InitUI();
	virtual bool InitLabels();
	virtual bool InitPlayfield();

	SDL_Texture* CreateTextTexture(const LabelObject& obj, const SDL_Color& textColor);

	virtual void HandleEvents(const SDL_Event& ev);
	virtual void HandleMouseEvents(const SDL_Event& ev);
	virtual void HandleKeyboardEvents(const SDL_Event& ev);

private:
	void Update();

	void Render();
	void RenderUI();
	void RenderLabels();
	void RenderField();

	template<class T>
	int16_t FindClickedRectObject(T& container, const Sint32& x, const Sint32& y)
	{
		for (size_t i = 0; i < container.size(); ++i)
		{
			if (container.at(i).IsClicked(x, y) && container.at(i).active)
			{
				return i;
			}
		}
		return -1;
	}

private:
	TTF_Font* m_font;
	//TODO: players

	SDL_Rect m_bgRect;
	SDL_Color m_bgColor;

	GameState m_state;
	GameMode m_mode;

	std::vector<UIObject> m_uiButtons;
	std::vector<LabelObject> m_labels;
	std::vector<FieldObject> m_playField;
};
