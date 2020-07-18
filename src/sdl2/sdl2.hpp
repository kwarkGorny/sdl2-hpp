#pragma once

#include "events.hpp"
#include "renderer.hpp"
#include "root.hpp"
#include "window.hpp"

namespace sdl2
{
	[[nodiscard]] inline std::pair<sdl2::Window, sdl2::Renderer> create(int width, int height, std::uint32_t windowFlags) noexcept
	{
		sdl2::WindowView window;
		sdl2::RendererView renderer;
		SDL_CreateWindowAndRenderer(width, height, windowFlags, &window, &renderer);
		return std::make_pair(sdl2::Window{ window }, sdl2::Renderer{ renderer });
	}
}