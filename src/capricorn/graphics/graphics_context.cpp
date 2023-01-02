// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#include "capricorn/graphics/graphics_context.hpp"

namespace cc
{
	graphics_context::graphics_context(const graphics_context_create_info& create_info)
	    : m_window(create_info.p_window)
	{
		vk::instance_create_info const instance_create_info = create_info.instance_create_info;

		m_instance = vk::instance::create(instance_create_info);
		m_surface  = m_instance->create_surface(m_window);

		vk::device_create_info const device_create_info = {
		        m_instance,
		        m_surface,
		};

		m_logical_device = vk::logical_device::create(device_create_info);
	}

	std::shared_ptr<graphics_context> graphics_context::create(const graphics_context_create_info& create_info)
	{
		return std::make_shared<graphics_context>(create_info);
	}

	std::weak_ptr<GLFWwindow> graphics_context::get_window() const
	{
		return m_window;
	}

	std::weak_ptr<vk::instance> graphics_context::get_instance() const
	{
		return m_instance;
	}
} // namespace cc