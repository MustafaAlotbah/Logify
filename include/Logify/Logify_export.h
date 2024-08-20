/*
 * Logify Logger Library
 *
 * Author: Mustafa Alotbah
 * Contact: mustafa.alotbah@gmail.com
 * Date: 2024 August
 *
 * Description:
 * This header file defines the macro for exporting or importing symbols
 * in the Logify library based on the platform and build configuration.
 *
 * License:
 * BSD 3-Clause License
 */

#pragma once

#ifdef _WIN32
#ifdef BUILDING_LOGIFY
#define LOGIFY_API __declspec(dllexport)
#else
#define LOGIFY_API __declspec(dllimport)
#endif
#else
#if defined(__GNUC__) && __GNUC__ >= 4
#define LOGIFY_API __attribute__((visibility("default")))
#else
#define LOGIFY_API
#endif
#endif