#pragma once

#include <type_traits>
#include <cstdint>
#include <SDL_ttf.h>

namespace sdl2::ttf
{
	inline bool init() noexcept
	{
		return TTF_Init() == 0;
	}

	inline bool wasInit() noexcept { return TTF_WasInit() == 0; }

	inline void quit() noexcept { TTF_Quit(); }

	class [[nodiscard]] TTFRoot
	{
	public:
		TTFRoot()noexcept
		{
			const auto success = sdl2::ttf::init();
			SDL_assert(success);
		}

		~TTFRoot()noexcept { sdl2::ttf::quit(); }
	};

	void setByteSwappedUNICODE(bool isSwapped) { TTF_ByteSwappedUNICODE(static_cast<int>(isSwapped)); }
}
