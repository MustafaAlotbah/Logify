
#include <utility>

#include "Logify/ScopedLogger.h"
#include "ScopedLoggerImpl.h"
#include "LoggerImpl.h"


Logify::ScopedLogger::Impl::Impl(Logger& logger, std::string scopeName, LogLevel level)
	:
	logger_(logger),
	scopeName_(std::move(scopeName)),
	level_(level),
	startTime_(std::chrono::steady_clock::now())
{}

Logify::ScopedLogger::ScopedLogger(Logify::Logger& logger, const std::string& scopeName, Logify::LogLevel level)
	: pImpl_(std::make_unique<Impl>(logger, scopeName, level))
{
	logger.log(level, scopeName);
	logger.pImpl_->indent();
}

Logify::ScopedLogger::~ScopedLogger()
{
	// decrease the indentation
	pImpl_->logger_.pImpl_->deindent();

	// Record the end time of the scope.
	auto endTime = std::chrono::steady_clock::now();

	// Calculate the duration of the scope in milliseconds.
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - pImpl_->startTime_).count();

	// Log the exit of the scope, along with its duration.
	pImpl_->logger_.log(pImpl_->level_, "~" + pImpl_->scopeName_ + " - Duration: " + std::to_string(duration) + " ms");
}
