#pragma once

#include <SDL_filesystem.h>
#include <string>
#include <optional>

namespace sdl2
{
	[[nodiscard]] inline std::optional<std::string> getPrefPath(const std::string& org, const std::string& app)noexcept
	{
		char* path = SDL_GetPrefPath(org.c_str(), app.c_str());
		if (!path)
		{
			return std::nullopt;
		}
		std::string pref{ path };
		SDL_free(path);
		return pref;
	}

	[[nodiscard]] inline std::optional<std::string> getBasePath()noexcept
	{
		char* path = SDL_GetBasePath();
		if (!path)
		{
			return std::nullopt;
		}
		std::string pref{ path };
		SDL_free(path);
		return pref;
	}
}