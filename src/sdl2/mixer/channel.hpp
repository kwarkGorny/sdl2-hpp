#pragma once

#include <SDL_mixer.h>
#include <cstdint>
#include <chrono>


namespace sdl2::mixer
{
	using OnChannelFinished = void (*)(int);

	using ChunkView = Mix_Chunk*;

	class Channel
	{
	public:

		[[nodiscard]] constexpr explicit Channel(int channelId)noexcept : m_ChannelId(channelId) {}

		[[nodiscard]] constexpr Channel(Channel&)noexcept = default;
		[[nodiscard]] constexpr Channel(Channel&&)noexcept = default;

		[[nodiscard]] constexpr Channel& operator=(Channel&)noexcept = default;
		[[nodiscard]] constexpr Channel& operator=(Channel&&)noexcept = default;

		bool registerEffect(Mix_EffectFunc_t f, Mix_EffectDone_t d, void* arg)const noexcept { return Mix_RegisterEffect(m_ChannelId, f, d, arg) != 0; }

		bool unregisterEffect(Mix_EffectFunc_t f)const noexcept { return Mix_UnregisterEffect(m_ChannelId, f) != 0; }

		bool unregisterAllEffects()const noexcept { return Mix_UnregisterAllEffects(m_ChannelId) != 0; }

		bool setPanning(std::uint8_t left, std::uint8_t right)const noexcept { return Mix_SetPanning(m_ChannelId, left, right) != 0; }

		bool setPosition(std::int16_t angle, std::uint8_t distance)const noexcept { return Mix_SetPosition(m_ChannelId, angle, distance) != 0; }

		bool setDistance(std::uint8_t distance)const noexcept { return Mix_SetDistance(m_ChannelId, distance) != 0; }

		bool reverseStereo(bool flip)const noexcept { return Mix_SetReverseStereo(m_ChannelId, flip) != 0; }

		bool group(int tag)const noexcept { return Mix_GroupChannel(m_ChannelId, tag) == 1; }

		bool halt() { return Mix_HaltChannel(m_ChannelId) == 1; }

		bool expire(std::chrono::milliseconds ms) { return Mix_ExpireChannel(m_ChannelId, ms.count()) == 1; }

		bool unexpire() { return Mix_ExpireChannel(m_ChannelId, -1) == 1; }

		bool fadeOut(std::chrono::milliseconds ms) { return Mix_ExpireChannel(m_ChannelId, ms.count()) == 1; }

		bool setVolume(int volume) { return Mix_Volume(m_ChannelId, volume); }

		[[nodiscard]] bool isPlaying() { return Mix_Playing(m_ChannelId); }

		[[nodiscard]] constexpr int get()const noexcept { return m_ChannelId; }

		[[nodiscard]] ChunkView getChunk()const noexcept { return Mix_GetChunk(m_ChannelId); }

		[[nodiscard]] Mix_Fading getFading() { return Mix_FadingChannel(m_ChannelId); }

		void pause() { Mix_Pause(m_ChannelId); }

		void resume() { Mix_Resume(m_ChannelId); }

		[[nodiscard]] bool isPaused() { return Mix_Paused(m_ChannelId); }

		static bool reserveChannels(int num)noexcept { return Mix_ReserveChannels(num); }

		static void onFinish(OnChannelFinished cb)noexcept { Mix_ChannelFinished(cb); }

		[[nodiscard]] static constexpr sdl2::mixer::Channel Any() { return sdl2::mixer::Channel(-1); }

	protected:
		int m_ChannelId;
	};
}