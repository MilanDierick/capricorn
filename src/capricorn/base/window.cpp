// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#include "capricorn/base/window.hpp"

namespace cc
{
	window::window(const char* title, const u32 width, const u32 height)
	    : m_p_title(title),
	      m_width(width),
	      m_height(height)
	{
		ensure(glfwInit(), "Failed to initialize GLFW!");
		ensure(glfwVulkanSupported(), "Vulkan is not supported!");

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		m_window = glfwCreateWindow(static_cast<i32>(m_width), static_cast<i32>(m_height), m_p_title, nullptr, nullptr);
		ensure(m_window, "Failed to create GLFW window!");
	}

	void window::tick()
	{
		glfwPollEvents();
	}

	b8 window::should_close() const
	{
		return glfwWindowShouldClose(m_window);
	}

	GLFWwindow* window::get_native_window() const
	{
		return m_window;
	}
} // namespace cc