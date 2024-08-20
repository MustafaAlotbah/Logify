/*
 * Logify Logger Library
 *
 * Author: Mustafa Alotbah
 * Contact: mustafa.alotbah@gmail.com
 * Date: 2024 August
 *
 * Description:
 * This header file provides the Logify API, including version information.
 * The Logify library offers a robust logging framework with various logging levels,
 * customizable output streams, and color schemes.
 *
 * Usage:
 * Include this header to access the Logify library API.
 *
 * License:
 * BSD 3-Clause License
 */

#pragma once

#include "Logify_export.h"
#include "Logger.h"
#include <string>


namespace Logify
{

  /**
   * @brief Retrieves the current version of the Logify library.
   * @return A string representing the version of the Logify library.
   */
  LOGIFY_API std::string getVersion();

} // namespace Logify
