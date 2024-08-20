
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