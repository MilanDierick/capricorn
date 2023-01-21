// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#include "capricorn/graphics/graphics_context.hpp"

#include "capricorn/graphics/vulkan/instance_configurator.hpp"

namespace cc
{
	graphics_context::graphics_context(const graphics_context_create_info& create_info)
	    : m_window(create_info.p_window)
	{
		vk::instance_configurator instance_configurator;

		vk::instance_create_info const instance_create_info = instance_configurator
		                                                              .set_application_name("Sample application")
		                                                              .set_engine_name("Capricorn")
		                                                              .set_application_version(1, 0, 0)
		                                                              .set_engine_version(1, 0, 0)
		                                                              .set_api_version(1, 0, 0)
		                                                              .add_enabled_layer("VK_LAYER_KHRONOS_validation")
		                                                              .add_enabled_extension(VK_KHR_SURFACE_EXTENSION_NAME)
		                                                              .set_validation_layers_enabled(true)
		                                                              .get_create_info();

		m_instance = std::make_unique<vk::instance>(instance_create_info);

		VkSurfaceKHR surface = VK_NULL_HANDLE;

		if (glfwCreateWindowSurface(*m_instance->get_handle().lock(), m_window.lock().get(), nullptr, &surface) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create window surface!");
		}

		m_surface = std::make_shared<VkSurfaceKHR>(surface);

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