
#include "Logify/Logger.h"
#include "LoggerImpl.h"

#include <algorithm>
#include <iostream>
#include <sstream>


Logify::Logger::Logger(Logify::LogLevel level) : pImpl_(std::make_unique<Impl>(level))
{}

// Destructor: Defaulted because pImpl_ is a smart pointer, and it will automatically clean up.
Logify::Logger::~Logger() = default;

Logify::Logger& Logify::Logger::setLogLevel(Logify::LogLevel level)
{
	// Update the current log level in the Logger implementation.
	pImpl_->currentLogLevel_ = level;
	return *this;
}

Logify::Logger& Logify::Logger::addOutputStream(std::ostream& out)
{
	// Add the provided output stream to the list of output streams in the Logger implementation.
	pImpl_->outputStreams_.push_back(&out);
	return *this;
}

Logify::Logger& Logify::Logger::removeOutputStream(std::ostream& out)
{
	// Remove the provided output stream from the list of output streams.
	pImpl_->outputStreams_.erase(
		std::remove(pImpl_->outputStreams_.begin(), pImpl_->outputStreams_.end(), &out),
		pImpl_->outputStreams_.end());
	return *this;
}

Logify::Logger& Logify::Logger::setTimeFormat(const std::string& format)
{
	// Set the time format string in the Logger implementation.
	pImpl_->timeFormat_ = format;
	return *this;
}

Logify::Logger& Logify::Logger::addFileStream(
	const std::string& filename,
	std::size_t maxFileSize,
	const ColorScheme& scheme
)
{
	// Create a new FileStream object with the given filename, max file size, and color scheme.
	// Add the FileStream to the list of file streams managed by the Logger implementation.
	pImpl_->fileStreams_.emplace_back(std::make_unique<FileStream>(filename, maxFileSize, scheme));
	return *this;
}

void Logify::Logger::log(Logify::LogLevel level, const std::string& message)
{
	// Check if the current log level allows this message to be logged.
	if (!pImpl_->shouldLog(level)) return;

	// Get the appropriate color code for the log level.
	const std::string& colorCode = pImpl_->getColorCode(level);

	// Get the current timestamp formatted according to the set time format.
	const std::string& timestamp = pImpl_->getCurrentTime();

	// Retrieve the current process ID.
	std::uint32_t pid = pImpl_->getPID();

	// Get the current thread ID.
	std::thread::id tid = std::this_thread::get_id();

	// Construct the log message string using the timestamp, PID, TID, log level, and message content.
	std::ostringstream oss;
	oss << "[" << timestamp << "][ID:" << pid << "/" << tid << "][" << pImpl_->levelToString(level) << "]: " << message
		<< std::endl;

	// Lock the mutex to ensure thread-safe access to the output streams.
	std::lock_guard<std::mutex> lock(pImpl_->mutex_);

	// Iterate over each output stream in the Logger implementation.
	for (auto& stream : pImpl_->outputStreams_)
	{
		// Check if the stream is either std::cout or std::cerr (console output).
		if (stream == &std::cout || stream == &std::cerr)
		{
			// If the stream is a console, prepend the color code and append the reset code after the message.
			*stream << colorCode << oss.str() << ConsoleColors::Reset << std::flush;
		}
		else
		{
			// For non-console ostreams, simply write the message without color codes.
			*stream << oss.str() << std::flush;
		}
	}

	// Write the log message to all file streams managed by the Logger implementation.
	pImpl_->writeToFileStreams(timestamp, pid, tid, level, message);
}

void Logify::Logger::trace(const std::string& message)
{
	log(LogLevel::TRACE, message);
}

void Logify::Logger::debug(const std::string& message)
{
	log(LogLevel::DEBUG, message);
}

void Logify::Logger::info(const std::string& message)
{
	log(LogLevel::INFO, message);
}

void Logify::Logger::warn(const std::string& message)
{
	log(LogLevel::WARN, message);
}

void Logify::Logger::error(const std::string& message)
{
	log(LogLevel::ERROR, message);
}

void Logify::Logger::fatal(const std::string& message)
{
	log(LogLevel::FATAL, message);
}

Logify::Logger& Logify::Logger::setIndentation(bool active)
{
	pImpl_->useIndent_ = active;
	return *this;
}
