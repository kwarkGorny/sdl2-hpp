#pragma once

#include "events.hpp"
#include "renderer.hpp"
#include "root.hpp"
#include "window.hpp"

namespace sdl2
{
	[[nodiscard]] inline std::pair<sdl2::Window, sdl2::Renderer> create(int width, int height, WindowFlags windowFlags) noexcept
	{
		sdl2::WindowView window;
		sdl2::RendererView renderer;
		SDL_CreateWindowAndRenderer(width, height, static_cast<std::uint32_t>(windowFlags), &window, &renderer);
		return std::make_pair(sdl2::Window{ window }, sdl2::Renderer{ renderer });
	}
}