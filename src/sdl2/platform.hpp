#pragma once

#include <SDL_platform.h>
#include <SDL_power.h>

#include <string_view>
#include <optional>
#include <chrono>

namespace sdl2::platform
{
	[[nodiscard]] inline std::string_view name() { return { SDL_GetPlatform() }; }
}

namespace sdl2::power
{
	enum class PowerState
	{
		UNKNOWN = SDL_POWERSTATE_UNKNOWN,
		ON_BATTERY = SDL_POWERSTATE_ON_BATTERY,
		NO_BATTERY = SDL_POWERSTATE_NO_BATTERY,
		CHARGING = SDL_POWERSTATE_CHARGING,
		CHARGED = SDL_POWERSTATE_CHARGED
	};

	struct PowerInfo
	{
		PowerState state;
		std::optional<std::chrono::seconds> timeLeft;
		std::optional<int> percentage;
	};

	[[nodiscard]] inline PowerInfo info()
	{
		int secs, pct;
		const auto state = SDL_GetPowerInfo(&secs, &pct);
		return PowerInfo{
			static_cast<PowerState>(state),
			secs >= 0 ? std::chrono::seconds(secs) : std::optional<std::chrono::seconds>{ std::nullopt },
			pct >= 0 ? pct : std::optional<int>{ std::nullopt }
		};
	}
}