// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#ifndef CAPRICORN_GRAPHICS_CONTEXT_HPP
#define CAPRICORN_GRAPHICS_CONTEXT_HPP

#include "capricorn/graphics/vulkan/instance.hpp"
#include "capricorn/graphics/vulkan/logical_device.hpp"

namespace cc
{
	struct graphics_context_create_info
	{
		std::weak_ptr<GLFWwindow> p_window;
	};

	class graphics_context
	{
	public:
		graphics_context()  = default;
		~graphics_context() = default;

		explicit graphics_context(const graphics_context_create_info& create_info);

		graphics_context(const graphics_context& other)                = delete;
		graphics_context(graphics_context&& other) noexcept            = delete;
		graphics_context& operator=(const graphics_context& other)     = delete;
		graphics_context& operator=(graphics_context&& other) noexcept = delete;

		static std::shared_ptr<graphics_context> create(const graphics_context_create_info& create_info);

		cc_nodiscard std::weak_ptr<GLFWwindow> get_window() const;
		cc_nodiscard std::weak_ptr<vk::instance> get_instance() const;

	private:
		std::weak_ptr<GLFWwindow> m_window;
		std::shared_ptr<vk::instance> m_instance;
		std::shared_ptr<VkSurfaceKHR> m_surface;
		std::shared_ptr<vk::logical_device> m_logical_device;
	};
} // namespace cc

#endif //CAPRICORN_GRAPHICS_CONTEXT_HPP
