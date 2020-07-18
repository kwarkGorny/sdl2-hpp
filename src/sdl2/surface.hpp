#pragma once

#include <string>
#include <optional>
#include <vector>

#include <SDL_surface.h>
#include <SDL_version.h>
#ifdef SDL2_ENABLE_IMG
	#include <SDL_image.h>
#endif

namespace sdl2
{
	enum class YUVConversion
	{
		JPEG = SDL_YUV_CONVERSION_JPEG,
		BT601 = SDL_YUV_CONVERSION_BT601,
		BT709 = SDL_YUV_CONVERSION_BT709,
		AUTOMATIC = SDL_YUV_CONVERSION_AUTOMATIC
	};

	enum class SurfaceFlags : std::uint32_t
	{
		SWSURFACE = SDL_SWSURFACE,
		PREALLOC = SDL_PREALLOC,
		RLEACCEL = SDL_RLEACCEL,
		DONTFREE = SDL_DONTFREE,
		SIMD_ALIGNED = SDL_SIMD_ALIGNED
	};

	using SurfaceView = SDL_Surface*;

	class Surface
	{
	public:
		struct ColorMod
		{
			std::uint8_t r;
			std::uint8_t g;
			std::uint8_t b;
		};

		[[nodiscard]] constexpr Surface() noexcept = default;
		[[nodiscard]] constexpr explicit Surface(SDL_Surface* surface) noexcept : m_Surface{ surface } {}

		[[nodiscard]] Surface(std::uint32_t flags, int	w, int	h, int	depth, std::uint32_t rmask, std::uint32_t gmask, std::uint32_t bmask, std::uint32_t amask) noexcept
			: m_Surface{ SDL_CreateRGBSurface(flags, w, h, depth, rmask, gmask, bmask, amask) }
		{}

		[[nodiscard]] Surface(std::uint32_t flags, int w, int h, int depth, std::uint32_t format) noexcept
			: m_Surface{ SDL_CreateRGBSurfaceWithFormat(flags, w, h, depth, format) }
		{}

		[[nodiscard]] Surface(void* pixels, int w, int h, int depth, int pitch, int format) noexcept
			: m_Surface{ SDL_CreateRGBSurfaceWithFormatFrom(pixels, w, h, depth, pitch, format) }
		{}

#ifdef SDL2_ENABLE_IMG
		[[nodiscard]] Surface(SDL_RWops* src, int freesrc, const std::string& type) noexcept
			: m_Surface{ IMG_LoadTyped_RW(src, freesrc, type.c_str()) }
		{}

		[[nodiscard]] Surface(SDL_RWops* src, int freesrc) noexcept
			: m_Surface{ IMG_Load_RW(src, freesrc) }
		{}

		[[nodiscard]] Surface(const std::string& filename) noexcept
			: m_Surface{ IMG_Load(filename.c_str()) }
		{}
#else 
		[[nodiscard]] Surface(const std::string& filename) noexcept
			: m_Surface{ SDL_LoadBMP(filename.c_str()) }
		{}
#endif

		[[nodiscard]] Surface(Surface&& other) noexcept : m_Surface(std::move(other.m_Surface)) { other.m_Surface = nullptr; }
		Surface& operator=(Surface&& other) noexcept
		{
			if (m_Surface != other.m_Surface)
			{
				SDL_FreeSurface(m_Surface);
				m_Surface = other.m_Surface;
			}
			other.m_Surface = nullptr;
			return *this;
		}

		Surface(Surface&) = delete;
		Surface& operator=(Surface&) = delete;

		~Surface()noexcept
		{
			if (m_Surface)
			{
				SDL_FreeSurface(m_Surface);
			}
		}

		bool setPalette(SDL_Palette* palette)noexcept { return SDL_SetSurfacePalette(m_Surface, palette) >= 0; }

		bool lock()noexcept { return SDL_LockSurface(m_Surface) >= 0; }
		void unlock()noexcept { SDL_UnlockSurface(m_Surface); }
		bool mustLock()noexcept { return SDL_MUSTLOCK(m_Surface); }

		bool saveToFile(const std::string& filename) noexcept { return SDL_SaveBMP(m_Surface, filename.c_str()) >= 0; }

		bool setRLE(bool enable)noexcept { return SDL_SetSurfaceRLE(m_Surface, static_cast<int>(enable)) >= 0; }

		bool setColorKey(bool enable, std::uint32_t key) const noexcept { return SDL_SetColorKey(m_Surface, static_cast<int>(enable), key) >= 0; }

		bool hasColorKey() const { return SDL_HasColorKey(m_Surface) == SDL_TRUE; }

		[[nodiscard]] std::optional<std::uint32_t> getColorKey() const noexcept
		{
			std::uint32_t v;
			if (SDL_GetColorKey(m_Surface, &v) >= 0)
			{
				return v;
			}
			return std::nullopt;
		}

		bool setColorMod(std::uint8_t r, std::uint8_t g, std::uint8_t b) const noexcept { return SDL_SetSurfaceColorMod(m_Surface, r, g, b) >= 0; }

		bool setColorMod(const sdl2::Surface::ColorMod& colorMod) const noexcept { return SDL_SetSurfaceColorMod(m_Surface, colorMod.r, colorMod.g, colorMod.b) >= 0; }

		[[nodiscard]] std::optional<sdl2::Surface::ColorMod> getColorMod() const
		{
			sdl2::Surface::ColorMod mod;
			if (SDL_GetSurfaceColorMod(m_Surface, &mod.r, &mod.g, &mod.b) >= 0)
			{
				return mod;
			}
			return std::nullopt;
		}

		bool setAlphaMod(std::uint8_t a) const { return SDL_SetSurfaceAlphaMod(m_Surface, a) >= 0; }

		[[nodiscard]] std::optional<std::uint8_t> getAlphaMod() const noexcept
		{
			std::uint8_t a;
			if (SDL_GetSurfaceAlphaMod(m_Surface, &a) >= 0)
			{
				return a;
			}
			return std::nullopt;
		}

		void setBlendMode(SDL_BlendMode blendMode)const noexcept { SDL_SetSurfaceBlendMode(m_Surface, blendMode); }

		[[nodiscard]] std::optional<SDL_BlendMode> getBlendMode()const noexcept
		{
			SDL_BlendMode blendMode;
			if (SDL_GetSurfaceBlendMode(m_Surface, &blendMode) >= 0)
			{
				return blendMode;
			}
			return std::nullopt;
		}

		void setClipRect(const SDL_Rect& rect)const noexcept { SDL_SetClipRect(m_Surface, &rect); }

		[[nodiscard]] SDL_Rect getClipRect()const noexcept
		{
			SDL_Rect rect;
			SDL_GetClipRect(m_Surface, &rect);
			return rect;
		}

		[[nodiscard]] Surface duplicate()noexcept { return Surface{ SDL_DuplicateSurface(m_Surface) }; }

		[[nodiscard]] Surface convert(const SDL_PixelFormat& pixelFormat, std::uint32_t flags = static_cast<std::uint32_t>(SurfaceFlags::SWSURFACE))const noexcept
		{
			return Surface{ SDL_ConvertSurface(m_Surface, &pixelFormat, flags) };
		}

		[[nodiscard]] Surface convert(std::uint32_t format, std::uint32_t flags = static_cast<std::uint32_t>(SurfaceFlags::SWSURFACE))const noexcept
		{
			return Surface{ SDL_ConvertSurfaceFormat(m_Surface, format, flags) };
		}

		bool convert(int width, int height, std::uint32_t src_format, const void* src, int src_pitch, std::uint32_t dst_format, void* dst, int dst_pitch)const noexcept
		{
			return SDL_ConvertPixels(width, height, src_format, src, src_pitch, dst_format, dst, dst_pitch) >= 0;
		}

		bool fill(const SDL_Rect& rect, std::uint32_t color)noexcept
		{
			return SDL_FillRect(m_Surface, &rect, color) >= 0;
		}

		bool fill(const SDL_Rect* rects, int count, std::uint32_t color)noexcept
		{
			return SDL_FillRects(m_Surface, rects, count, color) >= 0;
		}

		bool fill(const std::vector<SDL_Rect>& rects, std::uint32_t color)noexcept
		{
			return SDL_FillRects(m_Surface, rects.data(), static_cast<int>(rects.size()), color) >= 0;
		}

		static bool blit(Surface& source, const SDL_Rect& sourceRect, Surface& destination, SDL_Rect& destinationRect)noexcept
		{
			return SDL_BlitSurface(source.get(), &sourceRect, destination.get(), &destinationRect) >= 0;
		}

		static bool upperBlit(Surface& source, const SDL_Rect& sourceRect, Surface& destination, SDL_Rect& destinationRect)noexcept
		{
			return SDL_UpperBlit(source.get(), &sourceRect, destination.get(), &destinationRect) >= 0;
		}

		static bool lowerBlit(Surface& source, SDL_Rect& sourceRect, Surface& destination, SDL_Rect& destinationRect)noexcept
		{
			return SDL_LowerBlit(source.get(), &sourceRect, destination.get(), &destinationRect) >= 0;
		}

		static bool stretch(Surface& source, const SDL_Rect& sourceRect, Surface& destination, const SDL_Rect& destinationRect)noexcept
		{
			return SDL_SoftStretch(source.get(), &sourceRect, destination.get(), &destinationRect) >= 0;
		}

		static bool blitScaled(Surface& source, const SDL_Rect& sourceRect, Surface& destination, SDL_Rect& destinationRect)noexcept
		{
			return SDL_BlitScaled(source.get(), &sourceRect, destination.get(), &destinationRect) >= 0;
		}

		static bool upperBlitScaled(Surface& source, const SDL_Rect& sourceRect, Surface& destination, SDL_Rect& destinationRect)noexcept
		{
			return SDL_UpperBlitScaled(source.get(), &sourceRect, destination.get(), &destinationRect) >= 0;
		}

		static bool lowerBlitScaled(Surface& source, SDL_Rect& sourceRect, Surface& destination, SDL_Rect& destinationRect)noexcept
		{
			return SDL_LowerBlitScaled(source.get(), &sourceRect, destination.get(), &destinationRect) >= 0;
		}

		static void setYUVConversionMode(YUVConversion mode)noexcept { SDL_SetYUVConversionMode(static_cast<SDL_YUV_CONVERSION_MODE>(mode)); }

		[[nodiscard]] static YUVConversion getYUVConversionMode()noexcept { return static_cast<YUVConversion>(SDL_GetYUVConversionMode()); }

		[[nodiscard]] static YUVConversion getYUVConversionMode(int width, int height)noexcept { return static_cast<YUVConversion>(SDL_GetYUVConversionModeForResolution(width, height)); }

		[[nodiscard]] std::uint32_t getFlags()const noexcept { return m_Surface->flags; }

		[[nodiscard]] const SDL_PixelFormat* getPixelFormat()const noexcept { return m_Surface->format; }

		[[nodiscard]] int getWidth()const noexcept { return m_Surface->w; }

		[[nodiscard]] int getHeight()const noexcept { return m_Surface->h; }

		[[nodiscard]] int getPitch()const noexcept { return m_Surface->pitch; }

		[[nodiscard]] const void* getPixels()const noexcept { return m_Surface->pixels; }
		[[nodiscard]] void* getPixels()noexcept { return m_Surface->pixels; }

		[[nodiscard]] const void* getUserData()const noexcept { return m_Surface->userdata; }
		[[nodiscard]] void* getUserData()noexcept { return m_Surface->userdata; }

		[[nodiscard]] const void* getLockData()const noexcept { return m_Surface->lock_data; }

		[[nodiscard]] inline bool isValid()const noexcept { return m_Surface != nullptr; }

		[[nodiscard]] const SurfaceView get()const noexcept { return m_Surface; }
		[[nodiscard]] SurfaceView get()noexcept { return m_Surface; }

#ifdef SDL2_ENABLE_IMG
		bool savePNG(const std::string& file) { return IMG_SavePNG(m_Surface, file.c_str()); }
		bool saveJPG(const std::string& file, int quality) { return IMG_SaveJPG(m_Surface, file.c_str(), quality); }
#endif

	protected:
		SDL_Surface* m_Surface = nullptr;
	};
}