/*
 * Logify Logger Library
 *
 * Author: Mustafa Alotbah
 * Contact: mustafa.alotbah@gmail.com
 * Date: 2024 August
 *
 * Description:
 * This header file declares the Logger class for the Logify logging library.
 * The Logger class is designed to provide an easy-to-use logging interface
 * with customizable log levels, output streams, time formats, and color schemes.
 *
 * Usage:
 * The Logger class can be instantiated and used to log messages with various
 * severity levels. It supports output to multiple streams, including files
 * with rotation based on file size.
 *
 * License:
 * BSD 3-Clause License
 */

#pragma once

#include "Logify/Logify_export.h"
#include "Logify/ColorScheme.h"
#include <string>
#include <memory>


namespace Logify
{

  /**
   * @enum LogLevel
   * @brief Enumeration representing the severity levels for logging.
   */
  enum class LogLevel
  {
	  TRACE = 0,
	  DEBUG = 1,
	  INFO  = 2,
	  WARN  = 3,
	  ERROR = 4,
	  FATAL = 5
  };

  /**
   * @class Logger
   * @brief A customizable logging class for managing log messages and output streams.
   */
  class Logger
  {
   public:
	  /**
	   * @brief Constructs a Logger with a specified logging level.
	   * @param level The initial logging level. Default is LogLevel::INFO.
	   */
	  LOGIFY_API explicit Logger(LogLevel level = LogLevel::INFO);

	  /**
	   * @brief Destroys the Logger instance.
	   */
	  LOGIFY_API virtual ~Logger();

	  /**
	   * @brief Sets the logging level.
	   * @param level The logging level to set.
	   * @return A reference to the Logger object.
	   */
	  LOGIFY_API Logger& setLogLevel(LogLevel level);

	  /**
	   * @brief Adds an output stream to the logger.
	   * @param out The output stream to add.
	   * @return A reference to the Logger object.
	   */
	  LOGIFY_API Logger& addOutputStream(std::ostream& out);

	  /**
	   * @brief Removes an output stream from the logger.
	   * @param out The output stream to remove.
	   * @return A reference to the Logger object.
	   */
	  LOGIFY_API Logger& removeOutputStream(std::ostream& out);

	  /**
	   * @brief Sets the time format for log messages.
	   * @param format The time format string.
	   * @return A reference to the Logger object.
	   */
	  LOGIFY_API Logger& setTimeFormat(const std::string& format);

	  /**
	   * @brief Activates or Deactivates the indentation inside scopes (see ScopedLogger)
	   * @param active true for activating indentation, false otherwise.
	   * @return A reference to the Logger object.
	   */
	  LOGIFY_API Logger& setIndentation(bool active);

	  /**
	   * @brief Adds a file stream to the logger with optional size limits and color scheme.
	   * @param filename The name of the file to log to.
	   * @param maxFileSize The maximum size of the log file before rotation (default is 10MB).
	   * @param scheme The color scheme to apply (default is DefaultDarkScheme).
	   * @return A reference to the Logger object.
	   */
	  LOGIFY_API Logger& addFileStream(
		  const std::string& filename,
		  std::size_t maxFileSize = 10 * 1024 * 1024,
		  const ColorScheme& scheme = DefaultDarkScheme
	  );

	  /**
	   * @brief Logs a message with a specified log level.
	   * @param level The severity level of the log message.
	   * @param message The message to log.
	   */
	  LOGIFY_API void log(LogLevel level, const std::string& message);

	  /**
	   * @brief Logs a TRACE level message.
	   * @param message The message to log.
	   */
	  LOGIFY_API void trace(const std::string& message);

	  /**
	   * @brief Logs a DEBUG level message.
	   * @param message The message to log.
	   */
	  LOGIFY_API void debug(const std::string& message);

	  /**
	   * @brief Logs an INFO level message.
	   * @param message The message to log.
	   */
	  LOGIFY_API void info(const std::string& message);

	  /**
	   * @brief Logs a WARN level message.
	   * @param message The message to log.
	   */
	  LOGIFY_API void warn(const std::string& message);

	  /**
	   * @brief Logs an ERROR level message.
	   * @param message The message to log.
	   */
	  LOGIFY_API void error(const std::string& message);

	  /**
	   * @brief Logs a FATAL level message.
	   * @param message The message to log.
	   */
	  LOGIFY_API void fatal(const std::string& message);

   private:
	  friend class ScopedLogger;
	  class Impl;                    ///< Forward declaration of the implementation class.
	  std::unique_ptr<Impl> pImpl_;  ///< Pointer to the implementation class.
  };

} // namespace Logify