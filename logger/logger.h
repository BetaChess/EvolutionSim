#ifndef LOGGER_H
#define LOGGER_H

// Set the logger to log all, so aito can decide what to log itself.
#define SPDLOG_ACTIVE_LEVEL 1


#include <memory>

#include <spdlog/spdlog.h>
#undef near
#undef far



namespace evo
{

class Logger
{
private:
	static std::shared_ptr<spdlog::logger> core_logger_s;

public:
	static void init();

	inline static std::shared_ptr<spdlog::logger>& get_core_logger() { return core_logger_s; };
};

}

#define EVO_TRACE(...)	SPDLOG_LOGGER_TRACE(evo::Logger::get_core_logger(), __VA_ARGS__)
#define EVO_INFO(...)	SPDLOG_LOGGER_INFO(evo::Logger::get_core_logger(), __VA_ARGS__)
#define EVO_WARN(...)	SPDLOG_LOGGER_WARN(evo::Logger::get_core_logger(), __VA_ARGS__)
#define EVO_ERROR(...)	SPDLOG_LOGGER_ERROR(evo::Logger::get_core_logger(), __VA_ARGS__)
#define EVO_FATAL(...)	SPDLOG_LOGGER_FATAL(evo::Logger::get_core_logger(), __VA_ARGS__)



#endif // LOGGER_H

