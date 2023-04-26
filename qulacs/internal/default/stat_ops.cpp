#include "stat_ops.hpp"

double state_norm_squared(const std::vector<CTYPE>& state) {
    double norm = 0;
#ifdef _OPENMP
    OMPutil::get_inst().set_qulacs_num_threads(dim, 10);
#pragma omp parallel for reduction(+ : norm)
#endif
    for (ITYPE state_index = 0, loop_dim = state.size(); state_index < loop_dim;
         ++state_index) {
        norm += _cnorm(state[state_index])
    }
#ifdef _OPENMP
    OMPutil::get_inst().reset_qulacs_num_threads();
#endif
    return norm;
}