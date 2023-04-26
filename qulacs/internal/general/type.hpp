/**
 * @file type.hpp
 * @brief Type definitions
 */

#pragma once

#include <complex>

//! unsigned integer
using UINT = unsigned int;

//! complex value
using CTYPE = std::complex<double>;
using namespace std::complex_literals;

//! dimension index
#ifdef _MSC_VER
// In MSVC, OpenMP only supports signed index
using ITYPE = signed long long;
#else
using ITYPE = unsigned long long;
#endif

//! define export command
#if defined(__MINGW32__) || defined(_MSC_VER)
#define DllExport __declspec(dllexport)
#else
#define DllExport __attribute__((visibility("default")))
#endif