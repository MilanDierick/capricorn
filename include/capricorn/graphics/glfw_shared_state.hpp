// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#ifndef CAPRICORN_GLFW_SHARED_STATE_HPP
#define CAPRICORN_GLFW_SHARED_STATE_HPP

#include "capricorn/base/initialization_state.hpp"

#include <optional>

namespace cc
{
	class glfw_shared_state
	{
	public:
		glfw_shared_state()  = delete;
		~glfw_shared_state() = delete;

		glfw_shared_state(const glfw_shared_state& other)                = delete;
		glfw_shared_state(glfw_shared_state&& other) noexcept            = delete;
		glfw_shared_state& operator=(const glfw_shared_state& other)     = delete;
		glfw_shared_state& operator=(glfw_shared_state&& other) noexcept = delete;

		static std::vector<const char*> query_required_extensions();

	private:
		static std::optional<std::vector<const char*>> m_required_extensions;
	};
} // namespace cc

#endif //CAPRICORN_GLFW_SHARED_STATE_HPP
