// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#ifndef CAPRICORN_TYPES_HPP
#define CAPRICORN_TYPES_HPP

#include "capricorn/base/log.hpp"

#include <cstdint>
#include <vulkan/vulkan.h>

using i8  = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

using u8  = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using f32 = float;
using f64 = double;

using b8 = bool;

#define cc_unused [[maybe_unused]]
#define cc_nodiscard [[nodiscard]]

namespace cc
{
	constexpr void ensure(bool condition, const char* message)
	{
		if (!condition)
		{
			log::critical(log_source::none, message);
			throw;
		}
	}

	namespace vk
	{
		constexpr void vk_ensure(VkResult result, const char* message)
		{
			if (result != VK_SUCCESS)
			{
				log::critical(log_source::none, message);
				throw;
			}
		}

		template<typename T>
		constexpr std::vector<T> query_construct(VkInstance instance, const char* p_name)
		{
			u32 count = 0;
			vkEnumerateInstanceExtensionProperties(p_name, &count, nullptr);
			std::vector<T> extensions(count);
			vkEnumerateInstanceExtensionProperties(p_name, &count, extensions.data());
			return extensions;
		}
	} // namespace vk
} // namespace cc

#endif //CAPRICORN_TYPES_HPP
