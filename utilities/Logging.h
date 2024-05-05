#pragma once

#include <string>
#include <chrono>

#define LOG_TIMING(msg) Logging::Notification(msg)
#define LOG_INFO(msg) Logging::Notification(msg)
#define LOG_WARN(msg) Logging::Warning(msg)
#define LOG_ERROR(msg) Logging::Error(msg)


/**
* @class Logging
* @brief contains static functions for logging messages/warnings/errors
*/
class Logging
{
public:
	static void Notification(const std::string& message);
	static void Warning(const std::string& message);
	static void Error(const std::string& message);

	static inline Logging& Get() { return s_instance; };
private: // functions
	Logging() = default;
	static time_t GetCurrentTime();
	static void PrintCurrentTime();

private: // members
	static Logging s_instance;
};

