#include "logger.h"

#include <spdlog/sinks/stdout_color_sinks.h>

namespace evo
{
std::shared_ptr<spdlog::logger> Logger::core_logger_s;

void Logger::init()
{
	spdlog::set_pattern("%^[%T](%s:%#) %n: %v%$");
	core_logger_s = spdlog::stdout_color_mt("EVO");
	core_logger_s->set_level(spdlog::level::trace);

	EVO_INFO("Logger initialized successfully");
}

}
