#include "SDL_Handler.h"

SDL_Handler::SDL_Handler(const std::string & title, const uint16_t & width, const uint16_t & height)
	: m_title(title), m_width(width), m_height(height), m_window(nullptr), m_renderer(nullptr), m_running(false)
{
	m_running = SDL_Handler::Init();
}

SDL_Handler::~SDL_Handler()
{
	SDL_Handler::Clear();
}

bool SDL_Handler::Init()
{
	SDL_DisplayMode dm{};
	SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
	
	CATCH_ERROR(SDL_Init(SDL_INIT_EVERYTHING), "SDL_Init");
	CATCH_ERROR((m_width == 0 || m_height == 0) && SDL_GetDesktopDisplayMode(0, &dm), "SDL_GetDesktopDisplayMode");
	CATCH_ERROR(!(m_window = SDL_CreateWindow(m_title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED
		, m_width, m_height, SDL_WINDOW_SHOWN)), "SDL_CreateWindow");
	CATCH_ERROR(!(m_renderer = SDL_CreateRenderer(m_window, -1
		, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)), "SDL_CreateRenderer");

	return true;
}

void SDL_Handler::Run()
{
	SDL_Event ev{};

	while (m_running)
	{
		while (SDL_PollEvent(&ev))
		{
			switch (ev.type)
			{
			case SDL_QUIT:
				m_running = false;
				break;
			default:
				break;
			}
		}
	}
}

void SDL_Handler::Clear()
{
	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}
