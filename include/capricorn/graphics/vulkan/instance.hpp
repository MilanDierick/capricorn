// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#ifndef CAPRICORN_INSTANCE_HPP
#define CAPRICORN_INSTANCE_HPP

#include "capricorn/base/types.hpp"

#include <vulkan/vulkan.h>

namespace cc::vk
{
	namespace details
	{
		VKAPI_ATTR VkBool32 VKAPI_CALL default_debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
		                                                      VkDebugUtilsMessageTypeFlagsEXT message_type,
		                                                      const VkDebugUtilsMessengerCallbackDataEXT* p_callback_data,
		                                                      void* p_user_data);
	}

	enum class instance_create_flags
	{
		instance_create_enumerate_portability_bit_khr = 0x00000001,
	};

	struct instance_create_info_new
	{
		// Application information.
		const char* p_application_name;
		const char* p_engine_name;
		u32 application_version;
		u32 engine_version;
		u32 api_version;

		// Vulkan instance information.
		std::vector<const char*> enabled_layers;
		std::vector<const char*> enabled_extensions;
		instance_create_flags flags = static_cast<instance_create_flags>(0);

		// Validation features.
		std::vector<VkValidationCheckEXT> disabled_validation_checks;
		std::vector<VkValidationFeatureEnableEXT> enabled_validation_features;
		std::vector<VkValidationFeatureDisableEXT> disabled_validation_features;

		// Debug messenger information.
		VkDebugUtilsMessageSeverityFlagsEXT message_severity          = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		VkDebugUtilsMessageTypeFlagsEXT message_type                  = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		VkDebugUtilsMessengerCallbackDataFlagsEXT callback_data_flags = 0;
		PFN_vkDebugUtilsMessengerCallbackEXT debug_callback           = details::default_debug_callback;
		void* p_user_data                                             = nullptr;

		b8 validation_layers_enabled   = false;
		b8 validation_layers_requested = false;
		b8 debug_messenger_enabled     = false;

		// Custom allocator.
		VkAllocationCallbacks* p_allocator = nullptr;
	};

	class instance
	{
	public:
		instance()  = default;
		~instance() = default;

		explicit instance(const instance_create_info_new& params);

		instance(const instance& other)                = delete;
		instance(instance&& other) noexcept            = delete;
		instance& operator=(const instance& other)     = delete;
		instance& operator=(instance&& other) noexcept = delete;

		static std::shared_ptr<instance> create(const instance_create_info_new& create_info);

		cc_nodiscard std::weak_ptr<VkInstance> get_handle() const noexcept;

	private:
		std::shared_ptr<VkInstance> m_instance                      = nullptr;
		std::shared_ptr<VkDebugUtilsMessengerEXT> m_debug_messenger = nullptr;
		std::shared_ptr<VkAllocationCallbacks> m_allocator          = nullptr;
		PFN_vkGetInstanceProcAddr m_get_instance_proc_addr          = nullptr;
		PFN_vkGetDeviceProcAddr m_get_device_proc_addr              = nullptr;

		b8 properties2_ext_supported = false;
		u32 instance_version         = VK_API_VERSION_1_0;
		u32 api_version              = VK_API_VERSION_1_0;
	};
} // namespace cc::vk

#endif //CAPRICORN_INSTANCE_HPP
