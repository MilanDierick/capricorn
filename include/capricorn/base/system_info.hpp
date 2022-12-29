// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#ifndef CAPRICORN_SYSTEM_INFO_HPP
#define CAPRICORN_SYSTEM_INFO_HPP

#include "capricorn/base/types.hpp"

#include <vulkan/vulkan.h>

namespace cc
{
	struct system_info
	{
		system_info()  = delete;
		~system_info() = delete;

		system_info(const system_info& other)                = delete;
		system_info(system_info&& other) noexcept            = delete;
		system_info& operator=(const system_info& other)     = delete;
		system_info& operator=(system_info&& other) noexcept = delete;

		static b8 query_glfw();
		static b8 query_vulkan(VkInstance instance);

		static u32 glfw_extension_count;
		static const char** glfw_extensions;
		static u32 vk_available_extension_count;
		static std::vector<VkExtensionProperties> vk_available_extensions;
		static u32 vk_layer_count;
		static std::vector<VkLayerProperties> vk_layers;
		static u32 vk_physical_device_count;
		static std::vector<VkPhysicalDevice> vk_physical_devices;
	};

} // namespace cc

#endif //CAPRICORN_SYSTEM_INFO_HPP
