// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#include "capricorn/graphics/glfw_shared_state.hpp"

#include "capricorn/base/types.hpp"

namespace cc
{
	namespace details
	{

	}

	std::optional<std::vector<const char*>> glfw_shared_state::m_required_extensions = std::nullopt;

	std::vector<const char*> glfw_shared_state::query_required_extensions()
	{
		if (m_required_extensions.has_value())
		{
			return m_required_extensions.value();
		}

		uint32_t glfw_extension_count = 0;
		const char** glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);

		m_required_extensions = std::vector<const char*>(glfw_extensions, glfw_extensions + glfw_extension_count);

		return m_required_extensions.value();
	}
} // namespace cc