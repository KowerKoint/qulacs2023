#include <vector>
#ifdef _OPENMP
#include "../general/omp_util.hpp"
#endif

#include "../general/type.hpp"

namespace qulacs_internal {
void initialize_quantum_state(std::vector<CTYPE>& state) {
    ITYPE index;
#ifdef _OPENMP
    OMPutil::get_inst().set_qulacs_num_threads(state.size(), 15);
#endif

    initialize_quantum_state_parallel(state);

#ifdef _OPENMP
    OMPutil::get_inst().reset_qulacs_num_threads();
#endif
}
}  // namespace qulacs_internal

void initialize_quantum_state_parallel(std::vector<CTYPE>& state) {
    ITYPE index, dim = state.size();
#ifdef _OPENMP
#pragma omp parallel for
#endif
    for (index = 0; index < dim; ++index) {
        state[index] = 0;
    }
    state[0] = 1.0;
}