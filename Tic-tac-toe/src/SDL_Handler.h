#pragma once
#include "pch.h"

#define CATCH_ERROR(condition, function_name) if(condition) { \
			SDL_LogError(SDL_LOG_CATEGORY_ERROR, "%s Error: %s", function_name, SDL_GetError()); \
			return false; \
		}

class SDL_Handler
{
public:
	SDL_Handler(const std::string& title, const uint16_t& width, const uint16_t& height);
	virtual ~SDL_Handler();

	virtual void Run();
	
protected:
	virtual bool Init();
	virtual void Clear();

protected:
	SDL_Window* m_window;
	SDL_Renderer* m_renderer;

	std::string m_title;
	uint16_t m_width;
	uint16_t m_height;
	bool m_running;
};

