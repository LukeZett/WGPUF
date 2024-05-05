#pragma once


#include <chrono>
#include "Logging.h"

/**
* @brief Simple scope-based Timer, outputing to logging class
*/
class Timer
{
	std::chrono::high_resolution_clock::time_point m_start;
	std::string m_timingName;

public:
	Timer(const std::string& timedEventName)
	{
		m_timingName = timedEventName;
		m_start = std::chrono::high_resolution_clock::now();
	}

	inline void Stop()
	{
		auto end = std::chrono::high_resolution_clock::now();
		auto duration = end - m_start;

		LOG_TIMING(m_timingName + " took " + std::to_string(duration.count() / 1000000.0) + " ms");
	}

	~Timer()
	{
		Stop();
	}
};
