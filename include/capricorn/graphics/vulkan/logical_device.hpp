// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#ifndef CAPRICORN_LOGICAL_DEVICE_HPP
#define CAPRICORN_LOGICAL_DEVICE_HPP

#include "capricorn/base/types.hpp"
#include "instance.hpp"

#include <vk_mem_alloc.h>
#include <vulkan/vulkan.h>

namespace cc::vk
{
	struct device_create_info
	{
		std::weak_ptr<instance> instance;
		std::weak_ptr<VkSurfaceKHR> surface;
		std::vector<const char*> required_device_extensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
		std::vector<const char*> required_validation_layers = { "VK_LAYER_KHRONOS_validation" };
	};

	class logical_device
	{
	public:
		logical_device() = default;
		~logical_device() = default;

		explicit logical_device(const device_create_info& create_info);

		logical_device(const logical_device& other) = delete;
		logical_device(logical_device&& other) noexcept = delete;
		logical_device& operator=(const logical_device& other) = delete;
		logical_device& operator=(logical_device&& other) noexcept = delete;

		operator VkDevice() const; // NOLINT(hicpp-explicit-conversions)

		static std::shared_ptr<logical_device> create(const device_create_info& create_info);

		cc_nodiscard const device_create_info get_create_info() const;

	private:
		device_create_info m_create_info;

		VkDevice m_device = VK_NULL_HANDLE;
		VkPhysicalDevice m_physical_device = VK_NULL_HANDLE;
		std::pair<VkQueue, u32> m_graphics_queue;
		std::pair<VkQueue, u32> m_present_queue;
		VmaAllocation m_allocator = VK_NULL_HANDLE;
	};
} // namespace cc::vk

#endif //CAPRICORN_LOGICAL_DEVICE_HPP
