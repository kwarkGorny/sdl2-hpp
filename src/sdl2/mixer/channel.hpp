#pragma once

#include <SDL_mixer.h>
#include <cstdint>
#include <chrono>


namespace sdl2::mixer
{
	using OnChannelFinished = void (*)(int channel);

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

		bool setPanning(std::uint8_t left, std::uint8_t right)const noexcept { return Mix_SetPanning(m_ChannelId, left, right); }

		bool setPosition(std::int16_t angle, std::uint8_t distance)const noexcept { return Mix_SetPosition(m_ChannelId, angle, distance); }

		bool setDistance(std::uint8_t distance)const noexcept { return Mix_SetDistance(m_ChannelId, distance); }

		bool reverseStereo(bool flip)const noexcept { return Mix_SetReverseStereo(m_ChannelId, flip); }

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

		bool isPaused() { return Mix_Paused(m_ChannelId); }

		static bool reserveChannels(int num)noexcept { return Mix_ReserveChannels(num); }

		static bool group(int fromId, int toId, int tag) noexcept { return Mix_GroupChannels(fromId, toId, tag) == 1; }

		static bool haltGroup(int tag) noexcept { return Mix_HaltGroup(tag) == 1; }

		static bool groupAvailable(int tag) noexcept { return Mix_GroupAvailable(tag) != -1; }

		static int groupCount(int tag) noexcept { return Mix_GroupCount(tag); }

		static int findOldestSampleInGroup(int tag) noexcept { return Mix_GroupOldest(tag); }

		static int findYoungestSampleInGroup(int tag) noexcept { return Mix_GroupNewer(tag); }

		static bool fadeOutGroup(int tag, std::chrono::milliseconds ms) noexcept { return Mix_FadeOutGroup(tag, ms.count()); }

		static void onFinish(OnChannelFinished cb)noexcept { Mix_ChannelFinished(cb); }

		static bool setVolume(int volume) { return Mix_Volume(-1, volume) == 1; }

		[[nodiscard]] static constexpr sdl2::mixer::Channel Any() { return sdl2::mixer::Channel(-1); }

	protected:
		int m_ChannelId;
	};
}