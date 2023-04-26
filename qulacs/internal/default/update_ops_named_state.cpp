#include "update_ops.hpp"

void normalize(std::vector<CTYPE>& state, double norm) {
    const double normalize_factor = 1.0 / sqrt(norm);
#ifdef _OPENMP
    OMPutil::get_inst().set_qulacs_num_threads(dim, 13);
#pragma omp parallel for
#endif
    for (ITYPE state_index = 0, loop_dim = state.size(); state_index < loop_dim;
         ++state_index) {
        state[state_index] *= normalize_factor;
    }
#ifdef _OPENMP
    OMPutil::get_inst().reset_qulacs_num_threads();
#endif
}
