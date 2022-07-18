#pragma once

#include <SDL.h>
#include <SDL_ttf.h>

#include <string>
#include <array>
#include <vector>
#include <map>
#include <algorithm>
#include <memory>
#include <concepts>

#define CATCH_ERROR(condition, function_name) if(condition) { \
			SDL_LogError(SDL_LOG_CATEGORY_ERROR, "%s Error: %s", function_name, SDL_GetError()); \
			return false; \
		}

template<typename T>
concept Iterable = requires(T t)
{
	t.begin();
	t.end();
};