// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#include "capricorn/graphics/vulkan/instance.hpp"

#include "capricorn/base/log.hpp"
#include "capricorn/graphics/glfw_shared_state.hpp"
#include "capricorn/graphics/vulkan/vulkan_utils.hpp"

namespace cc::vk
{
	namespace details
	{
		VkBool32 default_debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT message_severity, VkDebugUtilsMessageTypeFlagsEXT message_type, const VkDebugUtilsMessengerCallbackDataEXT* p_callback_data, void* p_user_data)
		{
			if (message_severity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
			{
				log::warn(log_source::renderer, p_callback_data->pMessage);
			}
			else
			{
				log::info(log_source::renderer, p_callback_data->pMessage);
			}

			return VK_FALSE;
		}

		std::vector<std::string> get_available_validation_layers()
		{
			std::vector<VkLayerProperties> available_layers;
			vk_ensure(enumerate_vulkan_construct<VkLayerProperties>(available_layers, vkEnumerateInstanceLayerProperties), "Failed to enumerate available validation layers.");

			std::vector<std::string> available_layer_names;

			for (const auto& layer: available_layers)
			{
				available_layer_names.emplace_back(layer.layerName);
			}

			return available_layer_names;
		}

		std::vector<std::string> get_available_extensions()
		{
			std::vector<VkExtensionProperties> available_extensions;
			vk_ensure(enumerate_vulkan_construct<VkExtensionProperties>(available_extensions, vkEnumerateInstanceExtensionProperties, nullptr), "Failed to enumerate available extensions.");

			std::vector<std::string> available_extension_names;

			for (const auto& extension: available_extensions)
			{
				available_extension_names.emplace_back(extension.extensionName);
			}

			return available_extension_names;
		}

		VkResult create_debug_utils_messenger_ext(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* p_create_info, const VkAllocationCallbacks* p_allocator, VkDebugUtilsMessengerEXT* p_debug_messenger)
		{
			const auto func = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT"));
			if (func != nullptr)
			{
				return func(instance, p_create_info, p_allocator, p_debug_messenger);
			}

			return VK_ERROR_EXTENSION_NOT_PRESENT;
		}
	} // namespace details

	instance::instance(const instance_create_info& params)
	{
		if (params.p_application_name == nullptr)
		{
			log::error(log_source::renderer, "Application name is null.");
			throw std::runtime_error("Application name is null.");
		}

		if (params.p_engine_name == nullptr)
		{
			log::error(log_source::renderer, "Engine name is null.");
			throw std::runtime_error("Engine name is null.");
		}

		std::vector<const char*> extensions(params.enabled_extensions);
		std::vector<const char*> const layers(params.enabled_layers);
		std::vector<const char*> const glfw_extensions = glfw_shared_state::query_required_extensions();

		// Add all required GLFW extensions.
		extensions.insert(extensions.end(), std::begin(glfw_extensions), std::end(glfw_extensions));

		if (params.validation_layers_enabled)
		{
			// Add the debug utils extension.
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
			m_validation_layers_enabled = true;
		}

		// Check if the requested validation layers are available.
		if (params.validation_layers_requested)
		{
			const auto available_layers = details::get_available_validation_layers();
			for (const auto& requested_layer: params.enabled_layers)
			{
				if (std::find(available_layers.begin(), available_layers.end(), requested_layer) == available_layers.end())
				{
					log::error(log_source::renderer, "Requested validation layer {} is not available.", requested_layer);
					throw std::runtime_error("Requested validation layer is not available.");
				}
			}
		}

		// Check if the requested extensions are available.
		const auto available_extensions = details::get_available_extensions();
		for (const auto& requested_extension: extensions)
		{
			if (std::find(available_extensions.begin(), available_extensions.end(), requested_extension) == available_extensions.end())
			{
				log::error(log_source::renderer, "Requested extension {} is not available.", requested_extension);
				throw std::runtime_error("Requested extension is not available.");
			}
		}

		// Create the application info struct.
		VkApplicationInfo application_info  = {};
		application_info.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		application_info.pApplicationName   = params.p_application_name;
		application_info.applicationVersion = params.application_version;
		application_info.pEngineName        = params.p_engine_name;
		application_info.engineVersion      = params.engine_version;
		application_info.apiVersion         = VK_API_VERSION_1_0;

		// Create the instance create info struct.
		VkInstanceCreateInfo instance_create_info    = {};
		instance_create_info.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		instance_create_info.pApplicationInfo        = &application_info;
		instance_create_info.enabledExtensionCount   = static_cast<uint32_t>(extensions.size());
		instance_create_info.ppEnabledExtensionNames = extensions.data();
		instance_create_info.enabledLayerCount       = static_cast<uint32_t>(layers.size());
		instance_create_info.ppEnabledLayerNames     = layers.data();

		VkInstance instance = VK_NULL_HANDLE;

		// Create the instance.
		if (vkCreateInstance(&instance_create_info, params.p_allocator, &instance) != VK_SUCCESS)
		{
			log::error(log_source::renderer, "Failed to create vulkan instance.");
			throw std::runtime_error("Failed to create vulkan instance.");
		}

		m_instance = std::make_shared<VkInstance>(instance);

		// Create the debug messenger.
		if (params.validation_layers_requested)
		{
			VkDebugUtilsMessengerCreateInfoEXT debug_messenger_create_info = {};
			debug_messenger_create_info.sType                              = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
			debug_messenger_create_info.messageSeverity                    = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
			debug_messenger_create_info.messageType                        = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
			debug_messenger_create_info.pfnUserCallback                    = params.debug_callback;
			debug_messenger_create_info.pUserData                          = params.p_user_data;
			debug_messenger_create_info.pNext                              = nullptr;

			VkDebugUtilsMessengerEXT debug_messenger = VK_NULL_HANDLE;

			vk_ensure(details::create_debug_utils_messenger_ext(*m_instance, &debug_messenger_create_info, params.p_allocator, &debug_messenger), "Failed to create debug messenger.");

			m_debug_messenger = std::make_shared<VkDebugUtilsMessengerEXT>(debug_messenger);
		}

		log::info(log_source::renderer, "Vulkan instance created.");
	}

	instance::operator VkInstance() const noexcept
	{
		return *m_instance;
	}

	std::shared_ptr<instance> instance::create(const instance_create_info& create_info)
	{
		return std::make_shared<instance>(create_info);
	}

	std::weak_ptr<VkInstance> instance::get_handle() const noexcept
	{
		return m_instance;
	}

	b8 instance::validation_layers_enabled() const noexcept
	{
		return m_validation_layers_enabled;
	}
} // namespace cc::vk