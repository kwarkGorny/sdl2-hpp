#pragma once

#include <SDL_mixer.h>
#include <utility>
#include <string_view>
#include <chrono>

namespace sdl2::mixer
{
	using AdditionalMixerFunction = void (*)(void* udata, std::uint8_t* stream, int len);
	using OnMusicFinish = void (*)();


	class Music
	{
	public:
		constexpr Music()noexcept = default;
		Music(const char* file)noexcept : m_Music(Mix_LoadMUS(file)) {}

		~Music()noexcept
		{
			if (m_Music)
			{
				Mix_FreeMusic(m_Music);
			}
		}

		Music(Music&) = delete;
		Music(Music&& m) noexcept : m_Music(m.m_Music) { m.m_Music = nullptr; };

		Music& operator=(Music&) = delete;
		Music& operator=(Music&& other) noexcept
		{
			if (m_Music != other.m_Music)
			{
				Mix_FreeMusic(m_Music);
				m_Music = other.m_Music;
			}
			other.m_Music = nullptr;
			return *this;
		};

		static int getDecodersCount() { return Mix_GetNumChunkDecoders(); }
		static std::string_view getDecoder(int index) { return Mix_GetMusicDecoder(index); }
		static bool hasDecoder(const std::string& name) { return Mix_HasMusicDecoder(name.c_str()); }

		static void hook(AdditionalMixerFunction callback, void* arg) { Mix_HookMusic(callback, arg); }
		static void hook(OnMusicFinish callback) { Mix_HookMusicFinished(callback); }
		static void* getData() { return Mix_GetMusicHookData(); }

		static bool setVolume(int volume) { return Mix_VolumeMusic(volume) == 1; }

		static bool halt() { return Mix_HaltMusic() == 1; }

		static bool fadeOut(std::chrono::milliseconds ms) { return Mix_FadeOutMusic(ms.count()) == 1; }

		static Mix_Fading getFading() { return Mix_FadingMusic(); }

		static void pause() { Mix_PauseMusic(); }

		static void resume() { Mix_ResumeMusic(); }

		static void rewind() { Mix_RewindMusic(); }

		static bool isPaused() { return Mix_PausedMusic(); }

		static bool setPosition(double position) { return Mix_SetMusicPosition(position) == 0; }

		static bool isPlaying() { return Mix_PlayingMusic(); }

		static bool setMusicCMD(const std::string& command) { return Mix_SetMusicCMD(command.c_str()); }

		bool play(int loops = -1) { return Mix_PlayMusic(m_Music, loops) == 1; }

		bool fadeIn(std::chrono::milliseconds ms, int loops = -1) { return Mix_FadeInMusic(m_Music, loops, ms.count()) == 1; }

		bool fadeIn(std::chrono::milliseconds ms, double position, int loops = -1) { return Mix_FadeInMusicPos(m_Music, loops, ms.count(), position) == 1; }


		Mix_MusicType getType()const { return Mix_GetMusicType(m_Music); }

		[[nodiscard]] bool isValid()const noexcept { return m_Music != nullptr; }

		[[nodiscard]] Mix_Music* get()const noexcept { return m_Music; }

	protected:
		Mix_Music* m_Music = nullptr;
	};
}
