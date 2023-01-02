// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#ifndef CAPRICORN_VULKAN_UTILS_HPP
#define CAPRICORN_VULKAN_UTILS_HPP

#include "capricorn/base/types.hpp"

namespace cc::vk
{
	/**
	 * @brief Enumerates the results of a Vulkan API call that returns a list of data.
	 *
	 * @details The given function should take a count and data pointer as the last two
	 * arguments. When called with a nullptr for the data pointer, the function
	 * should return the number of results in the count argument. When called with
	 * a non-nullptr data pointer, the function should return the results in the
	 * data pointer and return VK_SUCCESS.
	 *
	 * @tparam Result The type of the result.
	 * @tparam Func The type of the function to call.
	 * @tparam Args The types of the arguments to pass to the function.
	 * @param[out] results The vector to store the enumerated results in.
	 * @param[in] func The function to call to retrieve the results.
	 * @param[in] params The parameters to pass to the function, excluding the
	 *                   count and data pointer.
	 * @return The result of the second call to the function, or VK_ERROR_INITIALIZATION_FAILED
	 *         if the first call to the function returned a failure result.
	 */
	template<typename Result, typename Func, typename ... Params>
	VkResult enumerate_vulkan_construct(std::vector<Result>& results, Func&& func, Params&& ... params)
	{
		u32 count = 0;
		VkResult result = func(params..., &count, nullptr);

		if (result != VK_SUCCESS)
		{
			return result;
		}

		results.resize(count);
		return func(params..., &count, results.data());
	}
} // namespace cc::vk

#endif //CAPRICORN_VULKAN_UTILS_HPP
