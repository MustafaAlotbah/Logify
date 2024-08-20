/*
 * Logify Logger Library
 *
 * Author: Mustafa Alotbah
 * Contact: mustafa.alotbah@gmail.com
 * Date: 2024 August
 *
 * Description:
 * This header file declares the ColorScheme struct used in the Logify
 * logging library. The ColorScheme struct defines color settings for
 * various log message components, enabling customizable log output styles.
 *
 * License:
 * BSD 3-Clause License
 */

#pragma once


#include <string>


namespace Logify
{

  /**
   * @struct ColorScheme
   * @brief A structure representing a color scheme for log message components.
   */
  struct ColorScheme
  {
	  std::string background;        ///< Background color of the log output.
	  std::string timestampColor;    ///< Color of the timestamp in the log output.
	  std::string pidTidColor;       ///< Color of the process ID and thread ID in the log output.
	  std::string defaultColor;      ///< Default color for log messages.
	  std::string debugColor;        ///< Color for DEBUG level log messages.
	  std::string infoColor;         ///< Color for INFO level log messages.
	  std::string warnColor;         ///< Color for WARN level log messages.
	  std::string errorColor;        ///< Color for ERROR level log messages.
  };

  /**
   * @brief Default dark color scheme used for log outputs.
   */
  constexpr ColorScheme DefaultDarkScheme = {
	  "#1e1e1e",
	  "#6c6c6c",
	  "#9cdcfe",
	  "#d4d4d4",
	  "#569cd6",
	  "#4ec9b0",
	  "#dcdcaa",
	  "#f44747"
  };

} // namespace Logify