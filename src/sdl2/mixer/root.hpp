#pragma once

#include <type_traits>
#include <cstdint>
#include <SDL_mixer.h>
#include <SDL_assert.h>

namespace sdl2::mixer
{
	enum class MixerFlag : int
	{
		FLAC = MIX_INIT_FLAC,
		MOD = MIX_INIT_MOD,
		MP3 = MIX_INIT_MP3,
		OGG = MIX_INIT_OGG,
		MID = MIX_INIT_MID,
		OPUS = MIX_INIT_OPUS,
		EVERYTHING = MIX_INIT_FLAC | MIX_INIT_MOD | MIX_INIT_MP3 | MIX_INIT_OGG | MIX_INIT_MID | MIX_INIT_OPUS
	};

	constexpr inline MixerFlag operator|(MixerFlag a, MixerFlag b)noexcept
	{
		return static_cast<MixerFlag>(static_cast<int>(a) | static_cast<int>(b));
	}

	inline bool init(MixerFlag flags) noexcept
	{
		return Mix_Init(static_cast<int>(flags)) >= 0;
	}

	template <typename... Flags>
	bool init(Flags... flags) noexcept
	{
		static_assert( ( std::is_same<Flags, MixerFlag>() && ... ) );
		return sdl2::image::init( ( flags | ... ) );
	}

	inline void quit() noexcept { Mix_Quit(); }

	class [[nodiscard]] MixerRoot
	{
	public:
		MixerRoot(MixerFlag flags)noexcept
		{
			const auto success = sdl2::mixer::init(flags);
			SDL_assert(success);
		}

		template <typename... Flags>
		MixerRoot(Flags... flags)noexcept
		{
			const auto success = sdl2::mixer::init(flags...);
			SDL_assert(success);
		}

		~MixerRoot()noexcept { sdl2::mixer::quit(); }
	};
}
