#include "Logging.h"
#include <iostream>

void Logging::PrintCurrentTime()
{
	auto now = GetCurrentTime();
	std::cout << std::ctime(&now);
}

void Logging::Notification(const std::string& message)
{
	//PrintCurrentTime();
	std::cout << "\u001b[36m[INFO]\u001b[0m " << message << std::endl;
}

void Logging::Warning(const std::string& message)
{
	//PrintCurrentTime();
	std::cout << "\u001b[33m[WARN]\u001b[0m " << message << std::endl;
}

void Logging::Error(const std::string& message)
{
	//PrintCurrentTime();
	std::cout << "\u001b[31m[ERROR]\u001b[0m " << message << std::endl;
}


time_t Logging::GetCurrentTime()
{
	return std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
}
