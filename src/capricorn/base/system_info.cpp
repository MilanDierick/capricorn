// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#include "capricorn/base/system_info.hpp"

#include <GLFW/glfw3.h>

namespace cc
{
	u32 system_info::glfw_extension_count                                   = 0;
	const char** system_info::glfw_extensions                               = nullptr;
	u32 system_info::vk_available_extension_count                           = 0;
	std::vector<VkExtensionProperties> system_info::vk_available_extensions = {};
	u32 system_info::vk_layer_count                                         = 0;
	std::vector<VkLayerProperties> system_info::vk_layers                   = {};
	u32 system_info::vk_physical_device_count                               = 0;
	std::vector<VkPhysicalDevice> system_info::vk_physical_devices          = {};

	u32 query_glfw_extension_count()
	{
		u32 count = 0;
		glfwGetRequiredInstanceExtensions(&count);
		return count;
	}

	const char** query_glfw_extensions()
	{
		return glfwGetRequiredInstanceExtensions(&system_info::glfw_extension_count);
	}

	u32 query_vk_extension_count()
	{
		u32 extension_count = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);
		return extension_count;
	}

	std::vector<VkExtensionProperties> query_vk_extensions()
	{
		u32 extension_count = query_vk_extension_count();
		std::vector<VkExtensionProperties> extensions(extension_count);
		vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, extensions.data());
		return extensions;
	}

	u32 query_vk_layer_count()
	{
		u32 layer_count = 0;
		vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
		return layer_count;
	}

	std::vector<VkLayerProperties> query_vk_layers()
	{
		u32 layer_count = query_vk_layer_count();
		std::vector<VkLayerProperties> layers(layer_count);
		vkEnumerateInstanceLayerProperties(&layer_count, layers.data());
		return layers;
	}

	u32 query_vk_physical_device_count(VkInstance instance)
	{
		u32 physical_device_count = 0;
		vkEnumeratePhysicalDevices(instance, &physical_device_count, nullptr);
		return physical_device_count;
	}

	std::vector<VkPhysicalDevice> query_vk_physical_devices(VkInstance instance)
	{
		u32 physical_device_count = query_vk_physical_device_count(instance);
		std::vector<VkPhysicalDevice> physical_devices(physical_device_count);
		vkEnumeratePhysicalDevices(instance, &physical_device_count, physical_devices.data());
		return physical_devices;
	}

	b8 system_info::query_glfw()
	{
		glfw_extension_count         = query_glfw_extension_count();
		glfw_extensions              = query_glfw_extensions();
		vk_available_extension_count = query_vk_extension_count();
		vk_available_extensions      = query_vk_extensions();
		vk_layer_count               = query_vk_layer_count();
		vk_layers                    = query_vk_layers();

		return true;
	}

	b8 system_info::query_vulkan(VkInstance instance)
	{
		vk_physical_device_count = query_vk_physical_device_count(instance);
		vk_physical_devices      = query_vk_physical_devices(instance);

		return true;
	}
} // namespace cc