/**
 * @file init_ops.hpp
 * @brief functions of initializing state vector
 */

#pragma once

#include <vector>

#include "../general/type.hpp"

DllExport void initialize_quantum_state(std::vector<CTYPE>& state);

DllExport void initialize_Haar_random_state(std::vector<CTYPE>& state);

DllExport void initialize_Haar_random_state_with_seed(
    std::vector<CTYPE>& state, UINT seed);