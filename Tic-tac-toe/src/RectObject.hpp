#pragma once
#include "Controller.hpp"

struct RectObject
{
	inline bool IsClicked(const int32_t& x, const int32_t& y) const noexcept
	{
		return x >= rect.x && x <= (rect.x + rect.w) && y >= rect.y && y <= rect.y + rect.h;
	}

	std::string name;
	SDL_Rect rect;
	bool active;
};

struct LabelObject : RectObject
{
	~LabelObject()
	{
		if (texture)
		{
			SDL_DestroyTexture(texture);
		}
	}

	SDL_Texture* texture;
};

struct UIObject : LabelObject
{
	void(*action)(GameState&, GameMode&);
};

struct FieldObject : RectObject
{
	std::shared_ptr<SDL_Color> color;
	PlayTeam markedBy;
};