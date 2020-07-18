#pragma once

#include "surface.hpp"
#include "renderer.hpp"
#ifdef SDL2_ENABLE_IMG
	#include <SDL_image.h>
#endif
#include <utility>

namespace sdl2
{
	class Texture
	{
	public:
		struct Attributes
		{
			std::uint32_t format;
			int access;
			int w;
			int h;
		};

		constexpr Texture()noexcept = default;

		Texture(RendererView renderer, SurfaceView surface)noexcept
			: m_Texture(SDL_CreateTextureFromSurface(renderer, surface))
		{}

		Texture(RendererView renderer, sdl2::Surface& surface)noexcept
			: m_Texture(SDL_CreateTextureFromSurface(renderer, surface.get()))
		{}

		Texture(RendererView renderer, std::uint32_t format, int access, int w, int h)
			: m_Texture(SDL_CreateTexture(renderer, format, access, w, h))
		{}

#ifdef SDL2_ENABLE_IMG
		Texture(RendererView renderer, const std::string& file)noexcept
			: m_Texture(IMG_LoadTexture(renderer, file.c_str()))
		{}
#endif
		~Texture()noexcept
		{
			if (m_Texture)
			{
				SDL_DestroyTexture(m_Texture);
			}
		}

		Texture(Texture&) = delete;
		Texture(Texture&& t) noexcept : m_Texture(t.m_Texture) { t.m_Texture = nullptr; };

		Texture& operator=(Texture&) = delete;
		Texture& operator=(Texture&& t) noexcept 
		{
			if (m_Texture != t.m_Texture)
			{
				SDL_DestroyTexture(m_Texture);
				m_Texture = t.m_Texture;
			}
			t.m_Texture = nullptr;
			return *this;
		};

		[[nodiscard]] Attributes getAttributes() const
		{
			Attributes attributes;
			SDL_QueryTexture(m_Texture, &attributes.format, &attributes.access, &attributes.w, &attributes.h);
			return attributes;
		}

		[[nodiscard]] std::uint32_t getFormat() const
		{
			std::uint32_t format;
			SDL_QueryTexture(m_Texture, &format, nullptr, nullptr, nullptr);
			return format;
		}

		bool update(const SDL_Rect& rect, const void* pixels, int pitch)
		{
			return SDL_UpdateTexture(m_Texture, &rect, pixels, pitch) == 0;
		}

		bool update(const void* pixels, int pitch)
		{
			return SDL_UpdateTexture(m_Texture, nullptr, pixels, pitch) == 0;
		}

		bool update(const SDL_Rect& rect, const std::uint8_t* Yplane, int Ypitch, const std::uint8_t* Uplane, int Upitch, const std::uint8_t* Vplane, int Vpitch)
		{
			return SDL_UpdateYUVTexture(m_Texture, &rect, Yplane, Ypitch, Uplane, Upitch, Vplane, Vpitch) == 0;
		}

		bool update(const std::uint8_t* Yplane, int Ypitch, const std::uint8_t* Uplane, int Upitch, const std::uint8_t* Vplane, int Vpitch)
		{
			return SDL_UpdateYUVTexture(m_Texture, nullptr, Yplane, Ypitch, Uplane, Upitch, Vplane, Vpitch) == 0;
		}

		bool lock(const SDL_Rect& rect, void** pixels, int* pitch)
		{
			return SDL_LockTexture(m_Texture, &rect, pixels, pitch);
		}

		bool lock(void** pixels, int* pitch)
		{
			return SDL_LockTexture(m_Texture, nullptr, pixels, pitch);
		}

		SurfaceView lock(const SDL_Rect& rect)
		{
			SDL_Surface* surface;
			return SDL_LockTextureToSurface(m_Texture, &rect, &surface) == 0 ? surface : nullptr;
		}

		SurfaceView lock()
		{
			SDL_Surface* surface;
			return SDL_LockTextureToSurface(m_Texture, nullptr, &surface) == 0 ? surface : nullptr;
		}

		void unlock()
		{
			SDL_UnlockTexture(m_Texture);
		}

		[[nodiscard]] int getAccess() const
		{
			int access;
			SDL_QueryTexture(m_Texture, nullptr, &access, nullptr, nullptr);
			return access;
		}

		[[nodiscard]] SDL_Point getSize() const
		{
			SDL_Point size;
			SDL_QueryTexture(m_Texture, nullptr, nullptr, &size.x, &size.y);
			return size;
		}

		[[nodiscard]] SDL_BlendMode getBlendMode()const noexcept
		{
			SDL_BlendMode blendMode;
			SDL_GetTextureBlendMode(m_Texture, &blendMode);
			return blendMode;
		}

		void setBlendMode(SDL_BlendMode blendMode)const noexcept { SDL_SetTextureBlendMode(m_Texture, blendMode); }

		[[nodiscard]] SDL_ScaleMode getScaleMode()const noexcept
		{
			SDL_ScaleMode scaleMode;
			SDL_GetTextureScaleMode(m_Texture, &scaleMode);
			return scaleMode;
		}

		void setScaleMode(SDL_ScaleMode scaleMode)const noexcept { SDL_SetTextureScaleMode(m_Texture, scaleMode); }

		[[nodiscard]] bool isValid()const noexcept { return m_Texture != nullptr; }
		[[nodiscard]] SDL_Texture* get()const noexcept { return m_Texture; }

		[[nodiscard]] SDL_Color getColor()const noexcept
		{
			SDL_Color c;
			SDL_GetTextureColorMod(m_Texture, &c.r, &c.g, &c.b);
			SDL_GetTextureAlphaMod(m_Texture, &c.a); 
			return c;
		}
		
		void setColor(const SDL_Color& c)const noexcept 
		{
			SDL_SetTextureColorMod(m_Texture, c.r, c.g, c.b);
			SDL_SetTextureAlphaMod(m_Texture, c.a); 
		}

		[[nodiscard]] std::optional<SDL_FPoint> glBind() 
		{
			SDL_FPoint size;
			if (SDL_GL_BindTexture(m_Texture, &size.x, &size.y) == 0)
			{
				return size;
			}
			return std::nullopt;
		}

		bool glUnbind() { return SDL_GL_UnbindTexture(m_Texture) == 0; }

	private:
		SDL_Texture* m_Texture = nullptr;
	};
}
