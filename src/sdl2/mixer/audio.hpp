#pragma once

#include <SDL_mixer.h>
#include <utility>
#include <cstdint>
#include <string>
#include <string_view>

namespace sdl2::mixer
{
	using OnMixingPerformed = void (*)(void* udata, std::uint8_t* stream, int len);

	inline bool openAudio(int frequency, std::uint16_t format, int channels, int chunkSize) { return Mix_OpenAudio(frequency, format, channels, chunkSize) != 0; }

	inline bool openDevice(int frequency, std::uint16_t format, int channels, int chunkSize, const char* device, int allowedchanges) { return Mix_OpenAudioDevice(frequency, format, channels, chunkSize, device, allowedchanges) != 0; }

	inline int allocateChannels(int channelsNumber) { return Mix_AllocateChannels(channelsNumber); }

	inline bool querySpec(int& frequency, std::uint16_t& format, int& channels) { return Mix_QuerySpec(&frequency, &format, &channels) != 0; }

	inline void setPostMix(OnMixingPerformed callback, void* arg) { Mix_SetPostMix(callback, arg); }

	inline bool setSynchroValue(int value) { return Mix_SetSynchroValue(value) == 1; }

	[[nodiscard]] inline int getSynchroValue() { return Mix_GetSynchroValue(); }

	inline bool setSoundFonts(const std::string& paths) { return Mix_SetSoundFonts(paths.c_str()) == 1; }

	inline std::string_view getSoundFonts() { return Mix_GetSoundFonts(); }

	using OnSoundFont = int (*)(const char*, void*);

	inline bool eachSoundFont(OnSoundFont cb, void* data) { return Mix_EachSoundFont(cb, data)== 1; };

	inline void closeAudio() { Mix_CloseAudio(); }

}
