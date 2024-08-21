/*
 * Logify Logger Library
 *
 * Author: Mustafa Alotbah
 * Contact: mustafa.alotbah@gmail.com
 * Date: 2024 August
 *
 * Description:
 * ScopedLogger is a utility class that logs the entry and exit points of a scope.
 * It automatically logs when the scope begins and when it ends, along with the duration
 * of the scope in milliseconds. This is particularly useful for timing and profiling
 * code execution within specific blocks.
 *
 * Usage:
 * Simply create an instance of ScopedLogger at the beginning of a scope.
 * The logger will automatically log the scope's entry and exit, along with the duration.
 * Example:
 *
 * ```cpp
 * {
 *     Logify::ScopedLogger logger(mainLogger, "MyFunction");
 *     // Your code here
 * }
 * ```
 *
 * License:
 * BSD 3-Clause License
 */

#pragma once

#include "Logify/Logify_export.h"
#include "Logify/Logger.h"
#include <string>
#include <memory>

// Macro to select the correct function signature macro
#ifdef _MSC_VER
#define LOGIFY_FUNC_SIGNATURE __FUNCSIG__
#else
#define LOGIFY_FUNC_SIGNATURE __PRETTY_FUNCTION__
#endif

#define LOGGER_NAME logger

// Macro to create a unique ScopedLogger with the default log level (INFO)
#define LOGIFY_SCOPED_LOGGER() \
    Logify::ScopedLogger LOGIFY_CONCATENATE(scopedLogger_, __COUNTER__)(LOGGER_NAME, LOGIFY_FUNC_SIGNATURE, Logify::LogLevel::INFO)

// Macro to create a unique ScopedLogger with a specified log level
#define LOGIFY_SCOPED_LOGGER_LEVEL(level) \
    Logify::ScopedLogger LOGIFY_CONCATENATE(scopedLogger_, __COUNTER__)(LOGGER_NAME, LOGIFY_FUNC_SIGNATURE, level)

// Macro to create a unique ScopedLogger with a specified scope name
#define LOGIFY_SCOPED_LOGGER_NAMED(name) \
    Logify::ScopedLogger LOGIFY_CONCATENATE(scopedLogger_, __COUNTER__)(LOGGER_NAME, name, Logify::LogLevel::INFO)

// Helper macros for concatenating names
#define LOGIFY_CONCATENATE_IMPL(s1, s2) s1##s2
#define LOGIFY_CONCATENATE(s1, s2) LOGIFY_CONCATENATE_IMPL(s1, s2)


namespace Logify
{

  /**
   * @class ScopedLogger
   * @brief A RAII-based logger that automatically logs the duration of a scope.
   *
   * ScopedLogger is designed to log when a scope is entered and exited, along with
   * the duration the scope was active. It uses RAII (Resource Acquisition Is Initialization)
   * to ensure the scope's end is logged when the object goes out of scope.
   */
  class ScopedLogger
  {
   public:
	  /**
	   * @brief Constructs a ScopedLogger.
	   * @param logger The Logger instance used for logging.
	   * @param scopeName The name of the scope being logged.
	   * @param level The log level for the scope entry and exit messages. Default is LogLevel::INFO.
	   */
	  LOGIFY_API explicit ScopedLogger(Logger& logger, const std::string& scopeName, LogLevel level = LogLevel::INFO);

	  /**
	   * @brief Destroys the ScopedLogger, logging the exit of the scope and its duration.
	   */
	  LOGIFY_API ~ScopedLogger();
   private:
	  class Impl;                      ///< Forward declaration of the implementation class.
	  std::unique_ptr<Impl> pImpl_;    ///< Pointer to the implementation class.
  };

} // namespace Logify