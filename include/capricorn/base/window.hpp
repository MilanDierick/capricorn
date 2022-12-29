// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#ifndef CAPRICORN_WINDOW_HPP
#define CAPRICORN_WINDOW_HPP

#include "capricorn/base/types.hpp"

#include <GLFW/glfw3.h>

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

		b8 should_close() const;

		[[nodiscard]] GLFWwindow* get_native_window() const;

	private:
		GLFWwindow* m_window;

		const char* m_p_title;
		u32 m_width;
		u32 m_height;
	};
} // namespace cc

#endif //CAPRICORN_WINDOW_HPP
