// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#include "capricorn/base/application.hpp"

#include "capricorn/base/log.hpp"

namespace cc
{
	application::application()
	    : m_window(),
	      m_state(application_state::none)
	{
	}

	application::~application()
	{
	}

	void application::initialize()
	{
		log::initialize();

		log::info(log_source::application, "Initializing Capricorn Engine...");

		m_window = std::make_shared<window>("Capricorn Engine", 1280, 720);

		m_state = application_state::initialized;
	}

	void application::execute()
	{
		log::info(log_source::application, "Executing Capricorn Engine...");

		m_state = application_state::running;

		while (m_state == application_state::running)
		{
			if (m_window->should_close())
			{
				m_state = application_state::shutdown;
				continue;
			}

			m_window->tick();
		}
	}

	void application::shutdown()
	{
		log::info(log_source::application, "Shutting down Capricorn Engine...");
	}
} // namespace cc