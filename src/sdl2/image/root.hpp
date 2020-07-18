#pragma once

#ifdef SDL2_ENABLE_IMG

#include <type_traits>
#include <cstdint>
#include <SDL_image.h>

namespace sdl2::image
{
	enum class IMGFlag : int
	{
		JPG = IMG_INIT_JPG,
		PNG = IMG_INIT_PNG,
		TIF = IMG_INIT_TIF,
		WEBP = IMG_INIT_WEBP
	};

	constexpr inline IMGFlag operator|(IMGFlag a, IMGFlag b)noexcept
	{
		return static_cast<IMGFlag>(static_cast<int>(a) | static_cast<int>(b));
	}

	inline bool init(IMGFlag flags) noexcept
	{
		return IMG_Init(static_cast<int>(flags)) >= 0;
	}

	template <typename... Flags>
	bool init(Flags... flags) noexcept
	{
		static_assert( ( std::is_same<Flags, IMGFlags>() && ... ) );
		return sdl2::image::init( ( flags | ... ) );
	}

	inline void quit() noexcept { IMG_Quit(); }

	class [[nodiscard]] IMGRoot
	{
	public:
		IMGRoot(IMGFlag flags)noexcept
		{
			const auto success = sdl2::image::init(flags);
			SDL_assert(success);
		}

		template <typename... Flags>
		IMGRoot(Flags... flags)noexcept
		{
			const auto success = sdl2::image::init(flags...);
			SDL_assert(success);
		}

		~IMGRoot()noexcept { sdl2::image::quit(); }
	};
}

#endif
