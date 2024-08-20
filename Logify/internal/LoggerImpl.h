/*
 * Logify Logger Library - Internal Implementation
 *
 * Author: Mustafa Alotbah
 * Contact: mustafa.alotbah@gmail.com
 * Date: 2024 August
 *
 * Description:
 * This header file contains the internal implementation details of the Logger class
 * in the Logify library. It defines the Logger::Impl class, which is responsible for
 * the actual logging logic, including managing log levels, output streams, file streams,
 * and formatting of log messages.
 *
 * Note:
 * This file is part of the internal implementation of the Logify library and is not intended
 * for direct use by library users. Modifications to this file should be done with caution,
 * as they can affect the core functionality of the Logger class.
 *
 * License:
 * BSD 3-Clause License
 */

#pragma once


#include "Logify/Logger.h"
#include "FileStream.h"
#include <vector>
#include <mutex>
#include <thread>


namespace Logify
{

  /**
   * @brief ANSI color codes for console output.
   *
   * These constants are used to colorize log messages when output to a console
   * that supports ANSI color codes.
   */
  namespace ConsoleColors
  {
	static constexpr const char* Grey   = "\033[90m";  ///< Grey color for low-importance messages.
	static constexpr const char* White  = "\033[97m";  ///< White color for standard messages.
	static constexpr const char* Yellow = "\033[93m";  ///< Yellow color for warning messages.
	static constexpr const char* Red    = "\033[91m";  ///< Red color for error messages.
	static constexpr const char* Reset  = "\033[0m";   ///< Reset color formatting to default.
  } // namespace ConsoleColors

  /**
   * @class Logger::Impl
   * @brief The internal implementation class for Logger.
   *
   * This class encapsulates the actual functionality of the Logger, handling log levels,
   * message formatting, output to streams, and file management. It is designed to be
   * used internally by the Logger class and is not intended for direct use by library users.
   */
  class Logger::Impl
  {
   public:
	  /**
	   * @brief Constructs the Logger::Impl with the specified log level and time format.
	   * @param level The initial log level.
	   * @param format The time format for log messages. Default is "%d.%m.%Y %H:%M:%S".
	   */
	  explicit Impl(LogLevel level, std::string format = "%d.%m.%Y %H:%M:%S");

	  /**
	   * @brief Determines if a message of a given log level should be logged.
	   * @param level The log level to check.
	   * @return True if the message should be logged, otherwise false.
	   */
	  [[nodiscard]] bool shouldLog(LogLevel level) const;

	  /**
	   * @brief Converts a LogLevel to its string representation.
	   * @param level The log level to convert.
	   * @return A string representing the log level.
	   */
	  [[nodiscard]] static std::string levelToString(LogLevel level);

	  /**
	   * @brief Retrieves the appropriate color code for a given log level.
	   * @param level The log level.
	   * @return A string containing the ANSI color code for the log level.
	   */
	  [[nodiscard]] static std::string getColorCode(LogLevel level);

	  /**
	   * @brief Gets the current time formatted according to the timeFormat_.
	   * @return A string representing the current time.
	   */
	  [[nodiscard]] std::string getCurrentTime() const;

	  /**
	   * @brief Writes log messages to all registered file streams.
	   * @param timestamp The formatted timestamp of the log message.
	   * @param pid The process ID.
	   * @param tid The thread ID.
	   * @param level The log level of the message.
	   * @param message The log message.
	   */
	  void writeToFileStreams(
		  const std::string& timestamp,
		  std::uint32_t pid,
		  std::thread::id tid,
		  Logify::LogLevel level,
		  const std::string& message
	  );

	  /**
	   * @brief Retrieves the current process ID.
	   * @return The process ID as an unsigned 32-bit integer.
	   */
	  static std::uint32_t getPID();

	  /**
	   * @brief Increase the indentation.
	   */
	  void indent();

	  /**
	   * @brief Decrease the indentation.
	   */
	  void deindent();

   private:
	  friend class Logger; ///< Allows Logger class to directly access the private members of Impl.
	  LogLevel                                 currentLogLevel_;  ///< The current logging level of the Logger.
	  std::vector<std::ostream*>               outputStreams_;    ///< Vector of output streams for logging.
	  std::string                              timeFormat_;       ///< Format string for timestamps in log messages.
	  std::mutex                               mutex_;            ///< Mutex for thread-safe access to Logger methods.
	  std::vector<std::unique_ptr<FileStream>> fileStreams_;      ///< Vector of file streams for logging to files.
	  size_t                                   indent_;
	  bool                                     useIndent_;
  };


} // namespace Logify