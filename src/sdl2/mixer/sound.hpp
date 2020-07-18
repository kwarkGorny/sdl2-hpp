#pragma once

#include "channel.hpp"

#include <SDL_mixer.h>
#include <utility>
#include <chrono>
#include <string>

namespace sdl2::mixer
{
	class Sound
	{
	public:
		[[nodiscard]] constexpr Sound()noexcept = default;
		[[nodiscard]] Sound(const std::string& file)noexcept
			: m_Sound(Mix_LoadWAV(file.c_str()))
		{}

		~Sound()noexcept
		{
			if (m_Sound)
			{
				Mix_FreeChunk(m_Sound);
			}
		}

		Sound(Sound&) = delete;
		[[nodiscard]] Sound(Sound&& s) noexcept : m_Sound(s.m_Sound) { s.m_Sound = nullptr; };

		Sound& operator=(Sound&) = delete;
		Sound& operator=(Sound&& other) noexcept
		{
			if (m_Sound != other.m_Sound)
			{
				Mix_FreeChunk(m_Sound);
				m_Sound = other.m_Sound;
			}
			other.m_Sound = nullptr;
			return *this;
		}

		sdl2::mixer::Channel play(int loops = 1, sdl2::mixer::Channel wantedChannel = sdl2::mixer::Channel::Any()) 
		{
			return sdl2::mixer::Channel{ Mix_PlayChannel(wantedChannel.get(), m_Sound, loops) }; 
		}

		sdl2::mixer::Channel play(std::chrono::milliseconds ticks, int loops = 1, sdl2::mixer::Channel wantedChannel = sdl2::mixer::Channel::Any()) 
		{
			return sdl2::mixer::Channel{ Mix_PlayChannelTimed(wantedChannel.get(), m_Sound, loops, ticks.count()) }; 
		}

		sdl2::mixer::Channel fadeIn(std::chrono::milliseconds ms, int loops = 1, sdl2::mixer::Channel wantedChannel = sdl2::mixer::Channel::Any())
		{
			return sdl2::mixer::Channel{ Mix_FadeInChannel(wantedChannel.get(), m_Sound, loops, ms.count()) };
		}

		sdl2::mixer::Channel fadeIn(std::chrono::milliseconds ms, std::chrono::milliseconds ticks, int loops = 1, sdl2::mixer::Channel wantedChannel = sdl2::mixer::Channel::Any())
		{ 
			return sdl2::mixer::Channel{ Mix_FadeInChannelTimed(wantedChannel.get(), m_Sound, loops, ms.count(), ticks.count()) };
		}

		bool setVolume(int volume) { return Mix_VolumeChunk(m_Sound, volume) == 1; }

		[[nodiscard]] bool isValid()const noexcept { return m_Sound != nullptr; }

		[[nodiscard]] Mix_Chunk* get()const noexcept { return m_Sound; }

	protected:
		Mix_Chunk* m_Sound = nullptr;

	};
}
