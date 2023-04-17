#pragma once

#include <stdexcept>

#include "type.hpp"

template <typename T>
void check_out_of_range(
    const std::string& arg_name, T arg, T lower_bound, T upper_bound) {
    if (arg < lower_bound || arg >= upper_bound) {
        std::stringstream ss;
        ss << arg_name << " is out of range: " << arg << " is not in ["
           << lower_bound << ", " << upper_bound << ").";
        throw std::out_of_range(ss.str());
    }
}