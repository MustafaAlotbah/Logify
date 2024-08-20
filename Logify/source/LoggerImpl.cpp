
#include "LoggerImpl.h"
#include <chrono>
#include <iomanip>
#include <sstream>
#include <utility>


Logify::Logger::Impl::Impl(Logify::LogLevel level, std::string format)
	:
	currentLogLevel_(level),
	timeFormat_(std::move(format)),
	indent_(0),
	useIndent_(false)
{}

bool Logify::Logger::Impl::shouldLog(Logify::LogLevel level) const
{
	return level >= currentLogLevel_;
}

std::string Logify::Logger::Impl::levelToString(Logify::LogLevel level)
{
	// Map each log level to its corresponding string name. TODO Customizable
	switch (level)
	{
		case LogLevel::TRACE:
			return "TRACE";
		case LogLevel::DEBUG:
			return "DEBUG";
		case LogLevel::INFO:
			return "INFO ";
		case LogLevel::WARN:
			return "WARN ";
		case LogLevel::ERROR:
			return "ERROR";
		case LogLevel::FATAL:
			return "FATAL";
		default:
			return "?";
	}
}

std::string Logify::Logger::Impl::getColorCode(Logify::LogLevel level)
{
	// Map each log level to its corresponding ANSI color code.
	switch (level)
	{
		case LogLevel::TRACE:
		case LogLevel::DEBUG:
			return ConsoleColors::Grey;
		case LogLevel::INFO:
			return ConsoleColors::White;
		case LogLevel::WARN:
			return ConsoleColors::Yellow;
		case LogLevel::ERROR:
		case LogLevel::FATAL:
			return ConsoleColors::Red;
		default:
			return ConsoleColors::White;
	}
}

std::string Logify::Logger::Impl::getCurrentTime() const
{
	// Get the current system time and convert to time_t for formatting.
	auto now   = std::chrono::system_clock::now();
	auto timeT = std::chrono::system_clock::to_time_t(now);

	// Convert to local time (based on system's locale).
	auto localTime = std::localtime(&timeT);

	// Format the time according to the provided format string.
	std::ostringstream oss;
	oss << std::put_time(localTime, timeFormat_.c_str());
	return oss.str();
}

void Logify::Logger::Impl::writeToFileStreams(
	const std::string& timestamp,
	std::uint32_t pid,
	std::thread::id tid,
	Logify::LogLevel level,
	const std::string& message
)
{
	// Convert the thread ID to a string.
	std::ostringstream oss;
	oss << tid;
	std::string tidStr = oss.str();

	size_t indent = useIndent_ ? indent_ : 0;

	// Iterate over each file stream and write the log message to it.
	for (const auto& fileStream : fileStreams_)
	{
		fileStream->write(timestamp, std::to_string(pid), tidStr, levelToString(level), message, indent);
	}
}
void Logify::Logger::Impl::indent()
{
	indent_++;
}

void Logify::Logger::Impl::deindent()
{
	if (indent_ == 0) return;
	indent_--;
}
