#pragma once

#include "window.hpp"

#include <SDL_messagebox.h>
#include <string>
#include <optional>

namespace sdl
{
	inline bool showMessageBox(std::uint32_t flags, const std::string& title, const std::string& message)
	{
		return SDL_ShowSimpleMessageBox(flags, title.c_str(), message.c_str(), nullptr) == 0;
	}

	inline bool showMessageBox(std::uint32_t flags, const std::string & title, const std::string& message, const sdl2::Window& parent)
	{
		return SDL_ShowSimpleMessageBox(flags, title.c_str(), message.c_str(), parent.get()) == 0;
	}

	inline std::optional<int> showMessageBox(const SDL_MessageBoxData& messageboxdata)
	{
		int buttonId;
		if (SDL_ShowMessageBox(&messageboxdata, &buttonId) != 0)
		{
			return std::nullopt;
		}
		return buttonId;
	}
}