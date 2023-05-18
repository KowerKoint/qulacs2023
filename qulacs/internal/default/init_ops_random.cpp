#include <limits.h>

#include <vector>

#ifdef _OPEMP
#include "../general/omp_util.hpp"
#endif
#include "../general/random.hpp"
#include "../general/type.hpp"
#include "init_ops.hpp"

namespace normal {
void initialize_Haar_random_state_single(std::vector<CTYPE>& state, UINT seed) {
    ITYPE dim = state.size();
    constexpr static int ignore_first = 40;
    double norm = 0.;
    Random random(seed);
    for (int i = 0; i < ignore_first; ++i) random.int64();
    for (ITYPE index = 0; index < dim; ++index) {
        double r1 = random.normal(), r2 = random.normal();
        state[index] = r1 + 1.i * r2;
        norm += r1 * r1 + r2 * r2;
    }
    norm = sqrt(norm);
    for (ITYPE index = 0; index < dim; ++index) {
        state[index] /= norm;
    }
}

#ifdef _OPENMP
void initialize_Haar_random_state_parallel(
    std::vector<CTYPE>& state, UINT seed) {
    ITYPE dim = state.size();
    // multi thread
    OMPutil::get_inst().set_qulacs_num_threads(dim, 10);
    constexpr static int ignore_first = 40;
    const UINT thread_count = omp_get_max_threads();
    const ITYPE block_size = dim / thread_count;
    const ITYPE residual = dim % thread_count;

    std::vector<Random> generator_list(thread_count);
    for (UINT i = 0; i < thread_count; ++i) {
        generator_list[i] = Random(seed ^ i);
    }

    std::vector<double> norm_list(thread_count);
#pragma omp parallel
    {
        UINT thread_id = omp_get_thread_num();
        ITYPE start_index = block_size * thread_id +
                            (residual > thread_id ? thread_id : residual);
        ITYPE end_index =
            block_size * (thread_id + 1) +
            (residual > (thread_id + 1) ? (thread_id + 1) : residual);
        ITYPE index;

        // ignore first randoms
        for (int i = 0; i < ignore_first; ++i) generator_list[i].int64();

        for (index = start_index; index < end_index; ++index) {
            double r1 = generator_list[i].normal(),
                   r2 = generator_list[i].normal();
            state[index] = r1 + 1.i * r2;
            norm_list[thread_id] += r1 * r1 + r2 * r2;
        }
    }

    double normalizer = 0.;
    for (UINT i = 0; i < thread_count; ++i) {
        normalizer += norm_list[i];
    }
    normalizer = 1. / sqrt(normalizer);
#pragma omp parallel for
    for (ITYPE index = 0; index < dim; ++index) {
        state[index] *= normalizer;
    }
    OMPutil::get_inst().reset_qulacs_num_threads();
}
#endif

void initialize_Haar_random_state(std::vector<CTYPE>& state, UINT seed) {
#ifdef _OPENMP
    initialize_Haar_random_state_parallel(state, seed);
#else
    initialize_Haar_random_state_single(state, seed);
#endif
}
}  // namespace normal