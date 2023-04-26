/**
 * @file number_util.hpp
 * @brief utility for number
 */

#pragma once

#include "type.hpp"

/**
 * Insert 0 to qubit_index-th bit of basis_index. basis_mask must be 1ULL <<
 * qubit_index.
 */
inline static ITYPE insert_zero_to_basis_index(
    ITYPE basis_index, UINT qubit_index) {
    ITYPE temp_basis = (basis_index >> qubit_index) << (qubit_index + 1);
    return temp_basis + basis_index % (1ULL << qubit_index);
}
