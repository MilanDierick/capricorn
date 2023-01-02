// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#ifndef CAPRICORN_APPLICATION_HPP
#define CAPRICORN_APPLICATION_HPP

#include "capricorn/base/types.hpp"
#include "capricorn/base/window.hpp"

namespace cc
{
	enum class application_state
	{
		none = 0,
		initialized,
		running,
		shutdown
	};

	class application
	{
	public:
		application();
		~application();

		application(const application& other)                = delete;
		application(application&& other) noexcept            = delete;
		application& operator=(const application& other)     = delete;
		application& operator=(application&& other) noexcept = delete;

		void initialize();
		void execute();
		void shutdown();

	private:
		std::shared_ptr<window> m_window;

		application_state m_state;
	};
} // namespace cc

#endif //CAPRICORN_APPLICATION_HPP
