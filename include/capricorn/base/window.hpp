// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#ifndef CAPRICORN_WINDOW_HPP
#define CAPRICORN_WINDOW_HPP

#include "capricorn/base/types.hpp"
#include "capricorn/graphics/graphics_context.hpp"

namespace cc
{
	class window
	{
	public:
		window()  = default;
		~window() = default;

		window(const char* title, u32 width, u32 height);

		window(const window& other)                = delete;
		window(window&& other) noexcept            = delete;
		window& operator=(const window& other)     = delete;
		window& operator=(window&& other) noexcept = delete;

		void tick();

		cc_nodiscard b8 should_close() const;

		cc_nodiscard std::weak_ptr<GLFWwindow> get_native_window() const;

	private:
		std::shared_ptr<GLFWwindow> m_window;
		std::shared_ptr<graphics_context> m_graphics_context;

		const char* m_p_title = nullptr;
		u32 m_width = 0;
		u32 m_height = 0;
	};
} // namespace cc

#endif //CAPRICORN_WINDOW_HPP
