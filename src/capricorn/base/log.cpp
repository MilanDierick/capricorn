// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#include "capricorn/base/log.hpp"

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace cc
{
	std::shared_ptr<spdlog::logger> log::s_logger = nullptr;

	constexpr std::string log_source_to_string(log_source source)
	{
		switch (source)
		{
			case log_source::none:
				return "[NONE] ";
			case log_source::application:
				return "[APPLICATION] ";
			case log_source::renderer:
				return "[RENDERER] ";
			default:
				return "[UNKNOWN] ";
		}
	}

	void log::initialize()
	{
		std::vector<spdlog::sink_ptr> log_sinks{};
		log_sinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
		log_sinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("Capricorn.log", true));

		log_sinks[0]->set_pattern("[%T] [%^%l%$] %v");
		log_sinks[1]->set_pattern("[%T] [%l] %v");

		s_logger = std::make_shared<spdlog::logger>("Capricorn", begin(log_sinks), end(log_sinks));
		register_logger(s_logger);
		s_logger->set_level(spdlog::level::trace);
		s_logger->flush_on(spdlog::level::trace);

		log::info(log_source::none, "Capricorn logging initialized and ready to start logging!");
	}
} // namespace cc