// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#ifndef CAPRICORN_LOG_HPP
#define CAPRICORN_LOG_HPP

#include <spdlog/fmt/ostr.h>
#include <spdlog/spdlog.h>

namespace cc
{
	enum class log_source
	{
		none = 0,
		application,
		renderer,
	};

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

	class log final
	{
	public:
		static void initialize();

		template<typename T>
		constexpr static void trace(log_source source, const T& message);

		template<typename... Args>
		constexpr static void trace(log_source source, spdlog::format_string_t<Args...> fmt, Args&&... args);

		template<typename T>
		constexpr static void info(log_source source, const T& message);

		template<typename... Args>
		constexpr static void info(log_source source, spdlog::format_string_t<Args...> fmt, Args&&... args);

		template<typename T>
		constexpr static void warning(log_source source, const T& message);

		template<typename... Args>
		constexpr static void warning(log_source source, spdlog::format_string_t<Args...> fmt, Args&&... args);

		template<typename T>
		constexpr static void error(log_source source, const T& message);

		template<typename... Args>
		constexpr static void error(log_source source, spdlog::format_string_t<Args...> fmt, Args&&... args);

		template<typename T>
		constexpr static void critical(log_source source, const T& message);

		template<typename... Args>
		constexpr static void critical(log_source source, spdlog::format_string_t<Args...> fmt, Args&&... args);

	private:
		static std::shared_ptr<spdlog::logger> s_logger;
	};

	template<typename T>
	constexpr void log::trace(log_source source, const T& message)
	{
		s_logger->trace(log_source_to_string(source) + message);
	}

	template<typename... Args>
	constexpr void log::trace(log_source source, spdlog::format_string_t<Args...> fmt, Args&&... args)
	{
		auto formatted_string = fmt::format(fmt, std::forward<Args>(args)...);
		s_logger->trace(log_source_to_string(source) + formatted_string);	}

	template<typename T>
	constexpr void log::info(log_source source, const T& message)
	{
		s_logger->info(log_source_to_string(source) + message);
	}

	template<typename... Args>
	constexpr void log::info(log_source source, spdlog::format_string_t<Args...> fmt, Args&&... args)
	{
		auto formatted_string = fmt::format(fmt, std::forward<Args>(args)...);
		s_logger->info(log_source_to_string(source) + formatted_string);	}

	template<typename T>
	constexpr void log::warning(log_source source, const T& message)
	{
		s_logger->warn(log_source_to_string(source) + message);
	}

	template<typename... Args>
	constexpr void log::warning(log_source source, spdlog::format_string_t<Args...> fmt, Args&&... args)
	{
		auto formatted_string = fmt::format(fmt, std::forward<Args>(args)...);
		s_logger->warn(log_source_to_string(source) + formatted_string);	}

	template<typename T>
	constexpr void log::error(log_source source, const T& message)
	{
		s_logger->error(log_source_to_string(source) + message);
	}

	template<typename... Args>
	constexpr void log::error(log_source source, spdlog::format_string_t<Args...> fmt, Args&&... args)
	{
		auto formatted_string = fmt::format(fmt, std::forward<Args>(args)...);
		s_logger->error(log_source_to_string(source) + formatted_string);
	}

	template<typename T>
	constexpr void log::critical(log_source source, const T& message)
	{
		s_logger->critical(log_source_to_string(source) + message);
	}

	template<typename... Args>
	constexpr void log::critical(log_source source, spdlog::format_string_t<Args...> fmt, Args&&... args)
	{
		auto formatted_string = fmt::format(fmt, std::forward<Args>(args)...);
		s_logger->critical(log_source_to_string(source) + formatted_string);	}
} // namespace cc

#endif //CAPRICORN_LOG_HPP
