// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#include "capricorn/graphics/vulkan/logical_device.hpp"

namespace cc::vk
{
	namespace details
	{
		struct queue_family_indices
		{
			std::optional<u8> graphics_family;
			std::optional<u8> present_family;

			cc_nodiscard b8 is_complete() const
			{
				return graphics_family.has_value() && present_family.has_value();
			}
		};

		queue_family_indices find_queue_families(const VkPhysicalDevice& physical_device, const VkSurfaceKHR& surface)
		{
			queue_family_indices indices;

			u32 queue_family_count = 0;
			vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, nullptr);

			std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
			vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, queue_families.data());

			u8 index = 0;
			for (const auto& queue_family: queue_families)
			{
				if (queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT)
					indices.graphics_family = index;

				VkBool32 present_support = false;
				vkGetPhysicalDeviceSurfaceSupportKHR(physical_device, index, surface, &present_support);

				if (present_support)
					indices.present_family = index;

				if (indices.is_complete())
					break;

				index++;
			}

			return indices;
		}

		b8 check_device_extension_support(const VkPhysicalDevice& physical_device, const std::vector<const char*>& required_device_extensions)
		{
			u32 extension_count = 0;
			vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &extension_count, nullptr);

			std::vector<VkExtensionProperties> available_extensions(extension_count);
			vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &extension_count, available_extensions.data());

			std::set<std::string> required_extensions(required_device_extensions.begin(), required_device_extensions.end());

			for (const auto& extension: available_extensions)
				required_extensions.erase(extension.extensionName);

			return required_extensions.empty();
		}

		struct swap_chain_support_details
		{
			VkSurfaceCapabilitiesKHR capabilities = {};
			std::vector<VkSurfaceFormatKHR> formats;
			std::vector<VkPresentModeKHR> present_modes;
		};

		swap_chain_support_details query_swap_chain_support(const VkPhysicalDevice& physical_device, const VkSurfaceKHR& surface)
		{
			swap_chain_support_details details;

			vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device, surface, &details.capabilities);

			u32 format_count = 0;
			vk_ensure(vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &format_count, nullptr), "failed to get physical device surface formats!");

			if (format_count != 0)
			{
				details.formats.resize(format_count);
				vk_ensure(vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &format_count, details.formats.data()), "failed to get physical device surface formats!");
			}

			u32 present_mode_count = 0;
			vk_ensure(vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &present_mode_count, nullptr), "failed to get physical device surface present modes!");

			if (present_mode_count != 0)
			{
				details.present_modes.resize(present_mode_count);
				vk_ensure(vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &present_mode_count, details.present_modes.data()), "failed to get physical device surface present modes!");
			}

			return details;
		}

		b8 is_device_suitable(const VkPhysicalDevice& physical_device, VkSurfaceKHR& surface, const std::vector<const char*>& required_device_extensions)
		{
			const auto indices = find_queue_families(physical_device, surface);

			const b8 extensions_supported = check_device_extension_support(physical_device, required_device_extensions);

			b8 swap_chain_adequate = false;
			if (extensions_supported)
			{
				swap_chain_support_details const swap_chain_support = query_swap_chain_support(physical_device, surface);
				swap_chain_adequate                                 = !swap_chain_support.formats.empty() && !swap_chain_support.present_modes.empty();
			}

			return indices.is_complete() && extensions_supported && swap_chain_adequate;
		}
	} // namespace details

	logical_device::logical_device(const device_create_info& create_info) // NOLINT(modernize-pass-by-value)
	    : m_create_info(create_info)
	{
		// Pick the physical device
		u32 device_count = 0;
		vk_ensure(vkEnumeratePhysicalDevices(m_create_info.instance.lock()->operator VkInstance(), &device_count, nullptr), "failed to enumerate physical devices!");

		if (device_count == 0)
			throw std::runtime_error("failed to find GPUs with Vulkan support!");

		std::vector<VkPhysicalDevice> devices(device_count);
		vk_ensure(vkEnumeratePhysicalDevices(m_create_info.instance.lock()->operator VkInstance(), &device_count, devices.data()), "failed to enumerate physical devices!");

		for (const auto& device: devices)
		{
			if (details::is_device_suitable(device, *m_create_info.surface.lock(), m_create_info.required_device_extensions))
			{
				m_physical_device = device;
				break;
			}
		}

		if (m_physical_device == VK_NULL_HANDLE)
		{
			throw std::runtime_error("failed to find a suitable GPU!");
		}

		// Create the logical device
		const auto indices = details::find_queue_families(m_physical_device, *m_create_info.surface.lock());

		std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
		std::set<u32> const unique_queue_families = {indices.graphics_family.value(), indices.present_family.value()};

		float const queue_priority = 1.0F;

		for (u32 const queue_family: unique_queue_families)
		{
			VkDeviceQueueCreateInfo queue_create_info = {};
			queue_create_info.sType                   = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queue_create_info.queueFamilyIndex        = queue_family;
			queue_create_info.queueCount              = 1;
			queue_create_info.pQueuePriorities        = &queue_priority;
			queue_create_infos.push_back(queue_create_info);
		}

		VkPhysicalDeviceFeatures const device_features = {};

		VkDeviceCreateInfo device_create_info      = {};
		device_create_info.sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		device_create_info.queueCreateInfoCount    = static_cast<u32>(queue_create_infos.size());
		device_create_info.pQueueCreateInfos       = queue_create_infos.data();
		device_create_info.pEnabledFeatures        = &device_features;
		device_create_info.enabledExtensionCount   = static_cast<u32>(m_create_info.required_device_extensions.size());
		device_create_info.ppEnabledExtensionNames = m_create_info.required_device_extensions.data();

		if (m_create_info.instance.lock()->validation_layers_enabled())
		{
			device_create_info.enabledLayerCount   = static_cast<u32>(m_create_info.required_validation_layers.size());
			device_create_info.ppEnabledLayerNames = m_create_info.required_validation_layers.data();
		}
		else
		{
			device_create_info.enabledLayerCount = 0;
		}

		vk_ensure(vkCreateDevice(m_physical_device, &device_create_info, nullptr, &m_device), "failed to create logical device!");

		vkGetDeviceQueue(m_device, indices.graphics_family.value(), 0, &m_graphics_queue.first);
		vkGetDeviceQueue(m_device, indices.present_family.value(), 0, &m_present_queue.first);
	}

	logical_device::operator VkDevice() const
	{
		return m_device;
	}

	std::shared_ptr<logical_device> logical_device::create(const device_create_info& create_info)
	{
		return std::make_shared<logical_device>(create_info);
	}

	const device_create_info logical_device::get_create_info() const
	{
		return m_create_info;
	}
} // namespace cc::vk