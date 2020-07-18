#pragma once

#include <type_traits>
#include <cstdint>
#include <SDL.h>

namespace sdl2
{
	enum class WindowSystemFlag : std::uint32_t
	{
		TIMER = SDL_INIT_TIMER,
		AUDIO = SDL_INIT_AUDIO,
		VIDEO = SDL_INIT_VIDEO,
		JOYSTICK = SDL_INIT_JOYSTICK,
		HAPTIC = SDL_INIT_HAPTIC,
		GAMECONTROLLER = SDL_INIT_GAMECONTROLLER,
		EVENTS = SDL_INIT_EVENTS,
		SENSOR = SDL_INIT_SENSOR,
		NOPARACHUTE = SDL_INIT_NOPARACHUTE,
		EVERYTHING = SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC | SDL_INIT_GAMECONTROLLER | SDL_INIT_SENSOR
	};

	constexpr WindowSystemFlag operator|(WindowSystemFlag a, WindowSystemFlag b)noexcept
	{
		return static_cast<WindowSystemFlag>(static_cast<std::uint32_t>(a) | static_cast<std::uint32_t>(b));
	}

	inline bool init(WindowSystemFlag flags) noexcept
	{
		return SDL_Init(static_cast<std::uint32_t>(flags)) >= 0;
	}

	template <typename... Flags>
	bool init(Flags... flags) noexcept
	{
		static_assert( ( std::is_same<Flags, WindowSystemFlag>() && ... ) );
		return sdl2::init( ( flags | ... ) );
	}

	inline bool initSubSystem(WindowSystemFlag flag)
	{
		return SDL_InitSubSystem(static_cast<std::uint32_t>(flag)) >= 0;
	}

	inline bool wasInit(WindowSystemFlag flag)
	{
		return SDL_WasInit(static_cast<std::uint32_t>(flag)) != 0;
	}

	inline void quitCloseSystem(WindowSystemFlag flag)
	{
		SDL_QuitSubSystem(static_cast<std::uint32_t>(flag));
	}

	inline void quit() noexcept { SDL_Quit(); }

	class [[nodiscard]] Root
	{
	public:
		Root(WindowSystemFlag flags)noexcept
		{
			const auto success = sdl2::init(flags);
			SDL_assert(success);
		}

		template <typename... Flags>
		Root(Flags... flags)noexcept
		{
			const auto success = sdl2::init(flags...);
			SDL_assert(success);
		}

		~Root()noexcept { sdl2::quit(); }
	};
}