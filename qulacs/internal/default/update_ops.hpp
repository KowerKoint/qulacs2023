/**
 * @file update_ops.hpp
 * @brief functions of updating state vector
 */

#pragma once

#include <vector>

#include "../general/type.hpp"

DllExport void normalize(const std::vector<CTYPE>& state, double norm);