#pragma once

#include "channel.hpp"
#include <cstdint>
#include <chrono>


namespace sdl2::mixer
{
	class ChannelGroup
	{
	public:

		[[nodiscard]] constexpr explicit ChannelGroup(int tag)noexcept : m_Tag(tag) {}

		[[nodiscard]] constexpr ChannelGroup(ChannelGroup&)noexcept = default;
		[[nodiscard]] constexpr ChannelGroup(ChannelGroup&&)noexcept = default;

		[[nodiscard]] constexpr ChannelGroup& operator=(ChannelGroup&)noexcept = default;
		[[nodiscard]] constexpr ChannelGroup& operator=(ChannelGroup&&)noexcept = default;

		int group(sdl2::mixer::Channel fromId, sdl2::mixer::Channel toId) noexcept { return Mix_GroupChannels(fromId.get(), toId.get(), m_Tag); }

		void halt() noexcept { Mix_HaltGroup(m_Tag); }

		[[nodiscard]] bool isAvailable() noexcept { return Mix_GroupAvailable(m_Tag) != -1; }

		[[nodiscard]] int firstAvaible() noexcept { return Mix_GroupAvailable(m_Tag); }

		[[nodiscard]] int count() noexcept { return Mix_GroupCount(m_Tag); }

		[[nodiscard]] int findOldestSample(int tag) noexcept { return Mix_GroupOldest(m_Tag); }

		[[nodiscard]] int findYoungestSample(int tag) noexcept { return Mix_GroupNewer(m_Tag); }

		int fadeOut(std::chrono::milliseconds ms) noexcept { return Mix_FadeOutGroup(m_Tag, ms.count()); }

	protected:
		int m_Tag;
	};
}