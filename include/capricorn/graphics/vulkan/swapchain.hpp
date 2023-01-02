// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#ifndef CAPRICORN_SWAPCHAIN_HPP
#define CAPRICORN_SWAPCHAIN_HPP

#include "instance.hpp"
#include "logical_device.hpp"
namespace cc::vk
{
	template<class T>
	struct object_create_info
	{
	};

	template<typename T>
	class vk_object
	{
	public:
		vk_object()          = default;
		virtual ~vk_object() = default;

		vk_object(const vk_object& other)                = delete;
		vk_object(vk_object&& other) noexcept            = delete;
		vk_object& operator=(const vk_object& other)     = delete;
		vk_object& operator=(vk_object&& other) noexcept = delete;

		virtual void create(const object_create_info<T>& info) = 0;
		virtual void destroy()                                 = 0;

		[[nodiscard]] virtual T get() const = 0;

	private:
		T m_object;
	};

	struct swapchain_create_info
	{
		std::weak_ptr<instance> p_instance;
		std::weak_ptr<logical_device> p_device;
		std::weak_ptr<VkSurfaceKHR> p_surface;
	};

	class swapchain
	{
	public:
		swapchain()  = default;
		~swapchain() = default;

		explicit swapchain(const swapchain_create_info& create_info);

		swapchain(const swapchain& other)                = delete;
		swapchain(swapchain&& other) noexcept            = delete;
		swapchain& operator=(const swapchain& other)     = delete;
		swapchain& operator=(swapchain&& other) noexcept = delete;

		static std::shared_ptr<swapchain> create(std::weak_ptr<instance> instance, std::weak_ptr<logical_device> device, std::weak_ptr<VkSurfaceKHR> surface);

		[[nodiscard]] const std::vector<VkImage>& get_images() const;
		[[nodiscard]] const std::vector<VkImageView>& get_image_views() const;
		[[nodiscard]] const VkExtent2D& get_extent() const;
		[[nodiscard]] const VkFormat& get_format() const;
		[[nodiscard]] const VkSwapchainKHR& get_swapchain() const;

	private:
		VkSwapchainKHR m_swapchain = VK_NULL_HANDLE;
		std::vector<VkImage> m_swapchain_images;
		std::vector<VkImageView> m_swapchain_image_views;
		VkExtent2D m_swapchain_extent     = {};
		VkFormat m_swapchain_image_format = VK_FORMAT_UNDEFINED;
	};
} // namespace cc::vk

#endif //CAPRICORN_SWAPCHAIN_HPP
