// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#include "capricorn/graphics/vulkan/instance_configurator.hpp"

namespace cc::vk
{
	instance_create_info& instance_configurator::get_create_info() noexcept
	{
		return m_info;
	}

	instance_configurator& instance_configurator::set_application_name(const char* name)
	{
		m_info.p_application_name = name;
		return *this;
	}

	instance_configurator& instance_configurator::set_engine_name(const char* name)
	{
		m_info.p_engine_name = name;
		return *this;
	}

	instance_configurator& instance_configurator::set_application_version(u32 major, u32 minor, u32 patch)
	{
		m_info.application_version = VK_MAKE_VERSION(major, minor, patch);
		return *this;
	}

	instance_configurator& instance_configurator::set_engine_version(u32 major, u32 minor, u32 patch)
	{
		m_info.engine_version = VK_MAKE_VERSION(major, minor, patch);
		return *this;
	}

	instance_configurator& instance_configurator::set_api_version(u32 major, u32 minor, u32 patch)
	{
		m_info.api_version = VK_MAKE_VERSION(major, minor, patch);
		return *this;
	}

	instance_configurator& instance_configurator::add_enabled_layer(const char* layer)
	{
		m_info.enabled_layers.push_back(layer);
		m_info.validation_layers_requested = true;
		return *this;
	}

	instance_configurator& instance_configurator::add_enabled_extension(const char* extension)
	{
		m_info.enabled_extensions.push_back(extension);
		return *this;
	}

	instance_configurator& instance_configurator::set_flags(instance_create_flags flags)
	{
		m_info.flags = flags;
		return *this;
	}

	instance_configurator& instance_configurator::add_disabled_validation_check(VkValidationCheckEXT check)
	{
		m_info.disabled_validation_checks.push_back(check);
		return *this;
	}

	instance_configurator& instance_configurator::add_enabled_validation_feature(VkValidationFeatureEnableEXT feature)
	{
		m_info.enabled_validation_features.push_back(feature);
		return *this;
	}

	instance_configurator& instance_configurator::add_disabled_validation_feature(VkValidationFeatureDisableEXT feature)
	{
		m_info.disabled_validation_features.push_back(feature);
		return *this;
	}

	instance_configurator& instance_configurator::set_message_severity(VkDebugUtilsMessageSeverityFlagsEXT severity)
	{
		m_info.message_severity = severity;
		return *this;
	}

	instance_configurator& instance_configurator::set_message_type(VkDebugUtilsMessageTypeFlagsEXT type)
	{
		m_info.message_type = type;
		return *this;
	}

	instance_configurator& instance_configurator::set_callback_data_flags(VkDebugUtilsMessengerCallbackDataFlagsEXT flags)
	{
		m_info.callback_data_flags = flags;
		return *this;
	}

	instance_configurator& instance_configurator::set_debug_callback(PFN_vkDebugUtilsMessengerCallbackEXT callback)
	{
		m_info.debug_callback = callback;
		return *this;
	}

	instance_configurator& instance_configurator::set_user_data(void* data)
	{
		m_info.p_user_data = data;
		return *this;
	}

	instance_configurator& instance_configurator::set_validation_layers_enabled(b8 enabled)
	{
		m_info.validation_layers_enabled = enabled;
		return *this;
	}

	instance_configurator& instance_configurator::set_validation_layers_requested(b8 requested)
	{
		m_info.validation_layers_requested = requested;
		return *this;
	}

	instance_configurator& instance_configurator::set_debug_messenger_enabled(b8 enabled)
	{
		m_info.debug_messenger_enabled = enabled;
		return *this;
	}

	instance_configurator& instance_configurator::set_allocator(VkAllocationCallbacks* allocator)
	{
		m_info.p_allocator = allocator;
		return *this;
	}
} // namespace cc::vk