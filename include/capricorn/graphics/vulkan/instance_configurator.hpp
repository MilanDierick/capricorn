// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#ifndef CAPRICORN_INSTANCE_CONFIGURATOR_HPP
#define CAPRICORN_INSTANCE_CONFIGURATOR_HPP

#include "capricorn/base/types.hpp"
#include "capricorn/graphics/vulkan/instance.hpp"

namespace cc::vk
{
	class instance_configurator
	{
	public:
		instance_configurator()  = default;
		~instance_configurator() = default;

		instance_configurator(const instance_configurator& other)                = delete;
		instance_configurator(instance_configurator&& other) noexcept            = delete;
		instance_configurator& operator=(const instance_configurator& other)     = delete;
		instance_configurator& operator=(instance_configurator&& other) noexcept = delete;

		instance_configurator& set_application_name(const char* name);
		instance_configurator& set_engine_name(const char* name);
		instance_configurator& set_application_version(u32 major, u32 minor, u32 patch);
		instance_configurator& set_engine_version(u32 major, u32 minor, u32 patch);
		instance_configurator& set_api_version(u32 major, u32 minor, u32 patch);

		instance_configurator& add_enabled_layer(const char* layer);
		instance_configurator& add_enabled_extension(const char* extension);
		instance_configurator& set_flags(instance_create_flags flags);

		instance_configurator& add_disabled_validation_check(VkValidationCheckEXT check);
		instance_configurator& add_enabled_validation_feature(VkValidationFeatureEnableEXT feature);
		instance_configurator& add_disabled_validation_feature(VkValidationFeatureDisableEXT feature);

		instance_configurator& set_message_severity(VkDebugUtilsMessageSeverityFlagsEXT severity);
		instance_configurator& set_message_type(VkDebugUtilsMessageTypeFlagsEXT type);
		instance_configurator& set_callback_data_flags(VkDebugUtilsMessengerCallbackDataFlagsEXT flags);
		instance_configurator& set_debug_callback(PFN_vkDebugUtilsMessengerCallbackEXT callback);
		instance_configurator& set_user_data(void* data);

		instance_configurator& set_validation_layers_enabled(b8 enabled);
		instance_configurator& set_validation_layers_requested(b8 requested);
		instance_configurator& set_debug_messenger_enabled(b8 enabled);

		instance_configurator& set_allocator(VkAllocationCallbacks* allocator);

	private:
		instance_create_info_new m_info;
	};
} // namespace cc::vk

#endif //CAPRICORN_INSTANCE_CONFIGURATOR_HPP
