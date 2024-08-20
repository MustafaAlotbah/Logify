/*
 * Logify Logger Library - Internal File Stream Management
 *
 * Author: Mustafa Alotbah
 * Contact: mustafa.alotbah@gmail.com
 * Date: 2024 August
 *
 * Description:
 * This header file defines the FileStream class, an internal component of the Logify
 * library used for managing file output streams for logging. The class handles writing
 * log messages to files, rotating files based on size, and ensuring file integrity
 * across log sessions.
 *
 * Note:
 * This file is part of the internal implementation of the Logify library and is not
 * intended for direct use by library users. Changes to this file should be made with
 * a clear understanding of how it affects file-based logging within the library.
 *
 * License:
 * BSD 3-Clause License
 */

#pragma once

#include <string>
#include <memory>
#include <fstream>
#include "Logify/ColorScheme.h"


namespace Logify
{

  /**
   * @enum FileExtension
   * @brief Enumeration representing supported file extensions for log files.
   */
  enum class FileExtension
  {
	  LOG,  ///< Standard text-based log files.
	  HTML  ///< HTML formatted log files.
  };

  /**
   * @brief Constant string for the ending HTML tags used in HTML log files.
   */
  const std::string HTML_ending = "</table></body></html>";

  /**
   * @class FileStream
   * @brief Manages file output streams for logging, including file rotation and integrity checks.
   *
   * The FileStream class is responsible for writing log messages to files, rotating files when
   * they exceed a specified size, and maintaining the correct file format. It supports both
   * plain text and HTML log files.
   */
  class FileStream
  {
   public:
	  /**
	   * @brief Constructs a FileStream with the specified file name, maximum file size, and color scheme.
	   * @param filename The base name of the log file.
	   * @param maxFileSize The maximum size of the log file before rotation occurs.
	   * @param scheme The color scheme used for formatting log messages in HTML files.
	   */
	  FileStream(const std::string& filename, std::size_t maxFileSize, const ColorScheme& scheme);

	  /**
	   * @brief Destroys the FileStream, ensuring the file stream is closed properly.
	   */
	  ~FileStream();

	  /**
	   * @brief Writes a log message to the file.
	   * @param timestamp The timestamp of the log message.
	   * @param pid The process ID associated with the log message.
	   * @param tid The thread ID associated with the log message.
	   * @param level The log level of the message.
	   * @param message The actual log message to write.
	   */
	  void write(
		  const std::string& timestamp,
		  const std::string& pid,
		  const std::string& tid,
		  const std::string& level,
		  const std::string& message,
		  size_t indent
	  );

   private:
	  /**
	   * @brief Opens a new log file for writing.
	   *
	   * If the log file already exists, it appends to the file. Otherwise, it creates a new file.
	   */
	  void openFile();

	  /**
	   * @brief Generates a file path based on the current file index and extension.
	   * @return A string representing the generated file path.
	   */
	  [[nodiscard]] std::string generateFilePath() const;

	  /**
	   * @brief Rotates the log file when it exceeds the maximum allowed size.
	   *
	   * The current file is closed, a new file is created, and the file index is incremented.
	   */
	  void rotateFile();

	  /**
	   * @brief Checks if the current log file should be rotated based on its size.
	   * @return True if the file should be rotated, otherwise false.
	   */
	  [[nodiscard]] bool shouldRotate() const;

	  /**
	   * @brief Verifies the integrity of the current log file.
	   * @return True if the file is intact, otherwise false.
	   */
	  [[nodiscard]] bool isFileIntact();

	  /**
	   * @brief Validates that the log file ends with the expected end tags.
	   * @param filePath The path to the log file.
	   * @param expectedEnd The expected ending string (e.g., HTML tags).
	   * @return True if the file ends with the expected tags, otherwise false.
	   */
	  static bool validateEndTags(const std::string& filePath, const std::string& expectedEnd);

	  /**
	   * @brief Truncates the log file at the specified position.
	   * @param filePath The path to the log file.
	   * @param truncatePosition The position from which to truncate the file.
	   */
	  static void truncateFileEnd(const std::string& filePath, std::streamoff truncatePosition);

	  /**
	   * @brief Replaces occurrences of a substring within a string with another substring.
	   * @param text The original string.
	   * @param to_replace The substring to replace.
	   * @param replace_with The substring to replace with.
	   * @return A new string with the replacements made.
	   */
	  static std::string replace(
		  const std::string& text,
		  const std::string& to_replace,
		  const std::string& replace_with
	  );

	  /**
	   * @brief Extracts the file extension from a filename.
	   * @param filename The filename to extract the extension from.
	   * @return A string representing the file extension.
	   */
	  static std::string extractExtension(const std::string& filename);

	  /**
	   * @brief Determines the file extension type (LOG or HTML) from a string.
	   * @param extension The file extension string.
	   * @return The corresponding FileExtension enum value.
	   */
	  static FileExtension determineExtensionType(const std::string& extension);
   private:
	  std::string                    logFileName_;    ///< The base name of the log file.
	  std::string                    extensionName_;  ///< The extension of the log file (e.g., ".log", ".html").
	  FileExtension                  extension_;      ///< The type of the file extension.
	  std::size_t                    maxFileSize_;    ///< The maximum file size before rotation.
	  int                            fileIndex_;      ///< Index for file rotation.
	  std::unique_ptr<std::ofstream> fileStream_;     ///< Output file stream for writing log messages.
	  ColorScheme                    colorScheme_;    ///< The color scheme used for HTML log files.
  };

} // namespace Logify