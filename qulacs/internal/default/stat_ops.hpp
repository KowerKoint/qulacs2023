/**
 * @file stat_ops.hpp
 * @brief functions of measuring state vector
 */

#pragma once

#include <vector>

#include "../general/type.hpp"

DllExport double m0_prob(
    const std::vector<CTYPE>& state, UINT target_qubit_index);

DllExport double marginal_prob(const std::vector<CTYPE>& state,
    const std::vector<UINT>& sorted_target_qubit_index_list,
    const std::vector<UINT>& measured_value_list);

DllExport double measurement_distribution_entropy(
    const std::vector<CTYPE>& state);