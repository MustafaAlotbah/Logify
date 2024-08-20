/*
 * Logify Logger Library - Internal Implementation
 *
 * Author: Mustafa Alotbah
 * Contact: mustafa.alotbah@gmail.com
 * Date: 2024 August
 *
 * Description:
 * This header file contains the implementation details of the ScopedLogger class.
 * It is part of the internal implementation and is not intended to be used directly
 * by users of the Logify library.
 *
 * License:
 * BSD 3-Clause License
 */


#pragma once

#include "Logify/ScopedLogger.h"
#include <chrono>


namespace Logify
{
  /**
   * @class ScopedLogger::Impl
   * @brief Implementation details for ScopedLogger.
   *
   * ScopedLogger::Impl handles the actual logging logic and timing functionality.
   * It is designed to be used internally by the ScopedLogger class.
   */
  class ScopedLogger::Impl
  {
   public:
	  /**
	   * @brief Constructs the implementation of ScopedLogger.
	   * @param logger The Logger instance used for logging.
	   * @param scopeName The name of the scope being logged.
	   * @param level The log level for the scope entry and exit messages.
	   */
	  explicit Impl(Logger& logger, std::string scopeName, LogLevel level);
   private:
	  friend class ScopedLogger;
	  Logger& logger_;                ///< Reference to the logger instance used for logging.
	  std::string scopeName_;         ///< The name of the scope being logged.
	  LogLevel    level_;                ///< The log level for the scope entry and exit messages.

	  std::chrono::time_point<std::chrono::steady_clock> startTime_; ///< The start time of the scope.
  };
} // namespace Logify