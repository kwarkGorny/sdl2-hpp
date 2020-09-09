#pragma once

#include "window.hpp"
#include "surface.hpp"

#include <SDL_render.h>
#include <utility>
#include <optional>

namespace sdl2
{
	using TextureView = SDL_Texture*;
	using RendererView = SDL_Renderer*;

	enum class RendererFlags : std::uint32_t
	{
		SOFTWARE = SDL_RENDERER_SOFTWARE,
		ACCELERATED = SDL_RENDERER_ACCELERATED,
		PRESENTVSYNC = SDL_RENDERER_PRESENTVSYNC,
		TARGETTEXTURE = SDL_RENDERER_TARGETTEXTURE
	};
	constexpr inline RendererFlags operator|(RendererFlags a, RendererFlags b)noexcept
	{
		return static_cast<RendererFlags>(static_cast<std::uint32_t>(a) | static_cast<std::uint32_t>(b));
	}


	class Renderer
	{
	public:
		[[nodiscard]] constexpr explicit Renderer(RendererView view)noexcept
			: m_Renderer(view)
		{};

		constexpr Renderer()noexcept = default;

		explicit Renderer(sdl2::Surface& surface)noexcept
			: m_Renderer(SDL_CreateSoftwareRenderer(surface.get()))
		{}

		Renderer(WindowView window, RendererFlags flags = sdl2::RendererFlags::ACCELERATED, int index = -1)noexcept
			: m_Renderer(SDL_CreateRenderer(window, index, static_cast<std::uint32_t>(flags)))
		{}

		~Renderer()noexcept
		{
			if (m_Renderer)
			{
				SDL_DestroyRenderer(m_Renderer);
			}
		}

		Renderer(const Renderer&)noexcept = delete;
		Renderer(Renderer&& r)noexcept : m_Renderer(r.m_Renderer) { r.m_Renderer = nullptr; }

		Renderer& operator=(const Renderer&)noexcept = delete;
		Renderer& operator=(Renderer&& r)noexcept 
		{
			if (m_Renderer != r.m_Renderer)
			{
				SDL_DestroyRenderer(m_Renderer);
				m_Renderer = r.m_Renderer;
			}
			r.m_Renderer = nullptr;
			return *this;
		}

		[[nodiscard]] static int getDriversCount() { return SDL_GetNumRenderDrivers(); }

		[[nodiscard]] static std::optional<SDL_RendererInfo> getInfo(int index)
		{
			SDL_RendererInfo info;
			if (SDL_GetRenderDriverInfo(index, &info) == 0)
			{
				return info;
			}
			return std::nullopt;
		}

		[[nodiscard]] std::optional<SDL_RendererInfo> getInfo()const
		{
			SDL_RendererInfo info;
			if (SDL_GetRendererInfo(m_Renderer, &info) == 0)
			{
				return info;
			}
			return std::nullopt;
		}

		[[nodiscard]] std::optional<SDL_Point> getOutputSize()const
		{
			SDL_Point size;
			if (SDL_GetRendererOutputSize(m_Renderer, &size.x, &size.y) == 0)
			{
				return size;
			}
			return std::nullopt;
		}

		[[nodiscard]] bool isTargetSupported()const { return SDL_RenderTargetSupported(m_Renderer) == SDL_TRUE; }

		[[nodiscard]] SDL_Texture* getTarget()const { return SDL_GetRenderTarget(m_Renderer); }

		bool setTarget(SDL_Texture* target)const { return SDL_SetRenderTarget(m_Renderer, target) == 0; }

		[[nodiscard]] SDL_Point getLogicalSize()const noexcept
		{
			SDL_Point size;
			SDL_RenderGetLogicalSize(m_Renderer, &size.x, &size.y);
			return size;
		}

		bool setLogicalSize(int w, int h) noexcept { return SDL_RenderSetLogicalSize(m_Renderer, w, h) == 0; }

		bool isIntegerScale()const noexcept { return SDL_RenderGetIntegerScale(m_Renderer) == SDL_TRUE; }

		bool setIntegerScale(bool enable) noexcept { return SDL_RenderSetIntegerScale(m_Renderer, static_cast<SDL_bool>(enable)) == 0; }

		[[nodiscard]] SDL_Rect getViewport()
		{
			SDL_Rect viewport;
			SDL_RenderGetViewport(m_Renderer, &viewport);
			return viewport;
		}

		bool setViewport(const SDL_Rect& rect) { return SDL_RenderSetViewport(m_Renderer, &rect) == 0; }

		bool resetViewport() { return SDL_RenderSetViewport(m_Renderer, nullptr) == 0; }

		[[nodiscard]] SDL_Rect getClipRect()
		{
			SDL_Rect clipRect;
			SDL_RenderGetClipRect(m_Renderer, &clipRect);
			return clipRect;
		}

		bool disableClipping() { return SDL_RenderSetClipRect(m_Renderer, nullptr) == 0; }

		bool setClipRect(const SDL_Rect& rect) { return SDL_RenderSetClipRect(m_Renderer, &rect) == 0; }

		bool isClipEnabled() { return SDL_RenderIsClipEnabled(m_Renderer) == SDL_TRUE; }

		[[nodiscard]] SDL_FPoint getScale()
		{
			SDL_FPoint scale;
			SDL_RenderGetScale(m_Renderer, &scale.x, &scale.y);
			return scale;
		}

		bool setScale(SDL_FPoint scale) { return SDL_RenderSetScale(m_Renderer, scale.x, scale.y) == 0; }

		[[nodiscard]] inline SDL_Color getDrawColor()const noexcept
		{
			SDL_Color color;
			SDL_GetRenderDrawColor(get(), &color.r, &color.g, &color.b, &color.a);
			return color;
		}

		bool setDrawColor(SDL_Color color)const noexcept { return SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, color.a) == 0; }

		[[nodiscard]] std::optional<SDL_BlendMode> getBlendMode()const noexcept
		{
			SDL_BlendMode mode;
			if (SDL_GetRenderDrawBlendMode(m_Renderer, &mode) == 0)
			{
				return mode;
			}
			return std::nullopt;
		}

		bool setBlendMode(SDL_BlendMode mode)const noexcept { return SDL_SetRenderDrawBlendMode(m_Renderer, mode) == 0; }

		bool draw(int x, int y) { return SDL_RenderDrawPoint(m_Renderer, x, y) == 0; }
		bool draw(float x, float y) { return SDL_RenderDrawPointF(m_Renderer, x, y) == 0; }

		bool draw(SDL_Point start, SDL_Point end) { return SDL_RenderDrawLine(m_Renderer, start.x, start.y, end.x, end.x) == 0; }
		bool draw(SDL_FPoint start, SDL_FPoint end) { return SDL_RenderDrawLineF(m_Renderer, start.x, start.y, end.x, end.x) == 0; }

		bool drawFilled(const SDL_Rect& rect)const noexcept { return SDL_RenderFillRect(m_Renderer, &rect) == 0; }
		bool drawFilled(const SDL_FRect& rect)const noexcept { return SDL_RenderFillRectF(m_Renderer, &rect) == 0; }

		bool drawFilled(const SDL_Rect* rects, int count)const noexcept { return SDL_RenderFillRects(m_Renderer, rects, count) == 0; }
		bool drawFilled(const SDL_FRect* rects, int count)const noexcept { return SDL_RenderFillRectsF(m_Renderer, rects, count) == 0; }

		bool drawOutlined(const SDL_Rect& rect)const noexcept { return SDL_RenderDrawRect(m_Renderer, &rect) == 0; }
		bool drawOutlined(const SDL_FRect& rect)const noexcept { return SDL_RenderDrawRectF(m_Renderer, &rect) == 0; }

		bool drawOutlined(const SDL_Rect* rects, int count)const noexcept { return SDL_RenderDrawRects(m_Renderer, rects, count) == 0; }
		bool drawOutlined(const SDL_FRect* rects, int count)const noexcept { return SDL_RenderDrawRectsF(m_Renderer, rects, count) == 0; }

		bool drawPoints(const SDL_Point* points, int count) { return SDL_RenderDrawPoints(m_Renderer, points, count) == 0; }
		bool drawPoints(const SDL_FPoint* points, int count) { return SDL_RenderDrawPointsF(m_Renderer, points, count) == 0; }

		bool drawLines(const SDL_Point* line, int count) { return SDL_RenderDrawPoints(m_Renderer, line, count) == 0; }
		bool drawLines(const SDL_FPoint* line, int count) { return SDL_RenderDrawPointsF(m_Renderer, line, count) == 0; }

		bool draw(TextureView texture, const SDL_Rect& source, const SDL_Rect& destination) { return SDL_RenderCopy(m_Renderer, texture, &source, &destination) == 0; }
		bool draw(TextureView texture, const SDL_Rect& source, const SDL_FRect& destination) { return SDL_RenderCopyF(m_Renderer, texture, &source, &destination) == 0; }

		bool draw(TextureView texture, const SDL_Rect& source, const SDL_Rect& destination, const double angle, const SDL_Point& center, const SDL_RendererFlip flip) { return SDL_RenderCopyEx(m_Renderer, texture, &source, &destination, angle, &center, flip) == 0; }
		bool draw(TextureView texture, const SDL_Rect& source, const SDL_FRect& destination, const double angle, const SDL_FPoint& center, const SDL_RendererFlip flip) { return SDL_RenderCopyExF(m_Renderer, texture, &source, &destination, angle, &center, flip) == 0; }

		bool readPixels(const SDL_Rect& rect, std::uint32_t format, void* pixels, int pitch) { return SDL_RenderReadPixels(m_Renderer, &rect, format, pixels, pitch) == 0; }

		bool clear()noexcept { return SDL_RenderClear(m_Renderer) == 0; }
		void present()noexcept { SDL_RenderPresent(m_Renderer); }
		bool flush()noexcept { return SDL_RenderFlush(m_Renderer) == 0; }

		void* getMetalLayer() { return SDL_RenderGetMetalLayer(m_Renderer); }
		void* getMetalCommandEncoder() { return SDL_RenderGetMetalCommandEncoder(m_Renderer); }

		[[nodiscard]] bool isValid()const noexcept { return m_Renderer != nullptr; }

		[[nodiscard]] RendererView get()const noexcept { return m_Renderer; }

		void reset()noexcept
		{
			if (m_Renderer)
			{
				SDL_DestroyRenderer(m_Renderer);
				m_Renderer = nullptr;
			}
		}

	private:
		SDL_Renderer* m_Renderer = nullptr;
	};
}

