#pragma once

#include "../surface.hpp"
#include <SDL_ttf.h>
#include <utility>
#include <string_view>
#include <optional>

namespace sdl2::ttf
{
	enum class FontStyle
	{
		NORMAL = TTF_STYLE_NORMAL,
		BOLD = TTF_STYLE_BOLD,
		ITALOC = TTF_STYLE_ITALIC,
		UNDERLINE = TTF_STYLE_UNDERLINE,
		STRIKETHROUGH = TTF_STYLE_STRIKETHROUGH
	};

	constexpr inline FontStyle operator|(FontStyle a, FontStyle b)noexcept
	{
		return static_cast<FontStyle>(static_cast<int>(a) | static_cast<int>(b));
	}

	enum class FontHinting
	{
		NORMAL = TTF_HINTING_NORMAL,
		LIGHT = TTF_HINTING_LIGHT,
		MONO = TTF_HINTING_MONO,
		NONE = TTF_HINTING_NONE
	};

	using FontView = TTF_Font*;

	class Font
	{
	public:
		[[nodiscard]] constexpr Font()noexcept = default;

		[[nodiscard]] explicit Font(const char* file, int ptsize)noexcept
			: m_Font(TTF_OpenFont(file, ptsize))
		{}

		[[nodiscard]] explicit Font(const char* file, int ptsize, long index)noexcept
			: m_Font(TTF_OpenFontIndex(file, ptsize, index))
		{}

		~Font()noexcept
		{
			if (m_Font)
			{
				TTF_CloseFont(m_Font);
			}
		}

		Font(const Font&)noexcept = delete;
		[[nodiscard]] Font(Font&& f)noexcept : m_Font(f.m_Font) { f.m_Font = nullptr; }

		Font& operator=(Font&)noexcept = delete;
		[[nodiscard]] Font& operator=(Font&& other)noexcept
		{
			if (m_Font != other.m_Font)
			{
				TTF_CloseFont(m_Font);
				m_Font = other.m_Font;
			}
			other.m_Font = nullptr;
			return *this;
		};

		[[nodiscard]] FontStyle getStyle() { return static_cast<FontStyle>(TTF_GetFontStyle(m_Font)); }
		void setStyle(FontStyle style) { TTF_SetFontStyle(m_Font, static_cast<int>(style)); }

		[[nodiscard]] int getOutline() { return TTF_GetFontOutline(m_Font); }
		void setOutline(int outlineSize) { TTF_SetFontOutline(m_Font, outlineSize); }

		[[nodiscard]] FontHinting getHinting() { return static_cast<FontHinting>(TTF_GetFontHinting(m_Font)); }
		void setHinting(FontHinting style) { TTF_SetFontHinting(m_Font, static_cast<int>(style)); }

		[[nodiscard]] int getHeight() { return TTF_FontHeight(m_Font); }

		[[nodiscard]] int getAscent() { return TTF_FontAscent(m_Font); }

		[[nodiscard]] int getDescent() { return TTF_FontDescent(m_Font); }

		[[nodiscard]] int getLineSkip() { return TTF_FontLineSkip(m_Font); }

		[[nodiscard]] bool hasKerning() { return TTF_GetFontKerning(m_Font) == 1; }
		void setKerning(bool allowed) { TTF_SetFontKerning(m_Font, static_cast<int>(allowed)); }

		[[nodiscard]] long getFaces() { return TTF_FontFaces(m_Font); }

		[[nodiscard]] bool isFaceFixedWidth() { return TTF_FontFaceIsFixedWidth(m_Font) > 0; }

		[[nodiscard]] std::string_view getFaceFamilyName() { return TTF_FontFaceFamilyName(m_Font); }
		[[nodiscard]] std::string_view getFaceStyleName() { return TTF_FontFaceStyleName(m_Font); }

		[[nodiscard]] bool isGlyphProvided(std::uint16_t ch) { return TTF_GlyphIsProvided(m_Font, ch) > 0; }
		[[nodiscard]] int getGlyphIndex(std::uint16_t ch) { return TTF_GlyphIsProvided(m_Font, ch); }

		bool queryGlyphMetrics(std::uint16_t ch, int& minx, int& maxx, int& miny, int& maxy, int& advance) { return TTF_GlyphMetrics(m_Font, ch, &minx, &maxx, &miny, &maxy, &advance) == 0; }

		std::optional<SDL_Point> getSize(const std::string& text) 
		{
			SDL_Point p;
			if (TTF_SizeText(m_Font, text.c_str(), &p.x, &p.y) == 0)
			{
				return p;
			}
			return std::nullopt;
		}

		bool querySize(const std::string& text, SDL_Point& size) { return TTF_SizeText(m_Font, text.c_str(), &size.x, &size.y) == 0; }

		std::optional<SDL_Point> getSizeUTF8(const std::string& text)
		{
			SDL_Point p;
			if (TTF_SizeUTF8(m_Font, text.c_str(), &p.x, &p.y) == 0)
			{
				return p;
			}
			return std::nullopt;
		}

		bool querySizeUTF8(const std::string& text, SDL_Point& size) { return TTF_SizeUTF8(m_Font, text.c_str(), &size.x, &size.y) == 0; }

		std::optional<SDL_Point> getSizeUNICODE(const std::uint16_t* text)
		{
			SDL_Point p;
			if (TTF_SizeUNICODE(m_Font, text, &p.x, &p.y) == 0)
			{
				return p;
			}
			return std::nullopt;
		}

		bool querySizeUNICODE(const std::uint16_t* text, SDL_Point& size) { return TTF_SizeUNICODE(m_Font, text, &size.x, &size.y) == 0; }
		
		[[nodiscard]] sdl2::Surface rendeSolid(std::uint16_t ch, SDL_Color fg) { return sdl2::Surface{ TTF_RenderGlyph_Solid(m_Font, ch, fg) }; }
		[[nodiscard]] sdl2::Surface renderSolid(const std::string& text, SDL_Color fg) { return sdl2::Surface{ TTF_RenderText_Solid(m_Font, text.c_str(), fg) }; }
		[[nodiscard]] sdl2::Surface renderUTF8Solid(const std::string& text, SDL_Color fg) { return sdl2::Surface{ TTF_RenderUTF8_Solid(m_Font, text.c_str(), fg) }; }
		[[nodiscard]] sdl2::Surface renderSolid(const std::uint16_t* text, SDL_Color fg) { return sdl2::Surface{ TTF_RenderUNICODE_Solid(m_Font, text, fg) }; }

		[[nodiscard]] sdl2::Surface renderShaded(std::uint16_t ch, SDL_Color fg, SDL_Color bg) { return sdl2::Surface{ TTF_RenderGlyph_Shaded(m_Font, ch, fg, bg) }; }
		[[nodiscard]] sdl2::Surface renderShaded(const std::string& text, SDL_Color fg, SDL_Color bg) { return sdl2::Surface{ TTF_RenderText_Shaded(m_Font, text.c_str(), fg, bg) }; }
		[[nodiscard]] sdl2::Surface renderUTF8Shaded(const std::string& text, SDL_Color fg, SDL_Color bg) { return sdl2::Surface{ TTF_RenderUTF8_Shaded(m_Font, text.c_str(), fg, bg) }; }
		[[nodiscard]] sdl2::Surface renderShaded(const std::uint16_t* text, SDL_Color fg, SDL_Color bg) { return sdl2::Surface{ TTF_RenderUNICODE_Shaded(m_Font, text, fg, bg) }; }

		[[nodiscard]] sdl2::Surface renderBlended(std::uint16_t ch, SDL_Color fg) { return sdl2::Surface{ TTF_RenderGlyph_Blended(m_Font, ch, fg) }; }
		[[nodiscard]] sdl2::Surface renderBlended(const std::string& text, SDL_Color fg) { return sdl2::Surface{ TTF_RenderText_Blended(m_Font, text.c_str(), fg) }; }
		[[nodiscard]] sdl2::Surface renderUTF8Blended(const std::string& text, SDL_Color fg) { return sdl2::Surface{ TTF_RenderUTF8_Blended(m_Font, text.c_str(), fg) }; }
		[[nodiscard]] sdl2::Surface renderBlended(const std::uint16_t* text, SDL_Color fg) { return sdl2::Surface{ TTF_RenderUNICODE_Blended(m_Font, text, fg) }; }

		[[nodiscard]] sdl2::Surface renderBlended(const std::string& text, SDL_Color fg, std::uint32_t wrapLength) { return sdl2::Surface{ TTF_RenderText_Blended_Wrapped(m_Font, text.c_str(), fg, wrapLength) }; }
		[[nodiscard]] sdl2::Surface renderUTF8Blended(const std::string& text, SDL_Color fg, std::uint32_t wrapLength) { return sdl2::Surface{ TTF_RenderUTF8_Blended_Wrapped(m_Font, text.c_str(), fg, wrapLength) }; }
		[[nodiscard]] sdl2::Surface renderBlended(const std::uint16_t* text, SDL_Color fg, std::uint32_t wrapLength) { return sdl2::Surface{ TTF_RenderUNICODE_Blended_Wrapped(m_Font, text, fg, wrapLength) }; }

		[[nodiscard]] int getKerningSizeGlyphs(std::uint16_t previousCh, std::uint16_t ch) { return TTF_GetFontKerningSizeGlyphs(m_Font, previousCh, ch); }

		[[nodiscard]] bool isValid()const noexcept { return m_Font != nullptr; }
		[[nodiscard]] FontView get()const noexcept { return m_Font; }

	private:
		TTF_Font* m_Font = nullptr;
	};

}
