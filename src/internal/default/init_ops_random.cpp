#include <limits.h>

#include <vector>

#ifdef _OPEMP
#include "../general/omp_util.hpp"
#endif
#include "../general/type.hpp"
#include "init_ops.hpp"

// state randomization
ITYPE xor_shift(ITYPE* state);
double random_uniform(ITYPE* state);
double random_normal(ITYPE* state);
void initialize_Haar_random_state_with_seed_single(
    std::vector<CTYPE>& state, UINT seed);
void initialize_Haar_random_state_with_seed_parallel(
    std::vector<CTYPE>&, UINT seed);

void initialize_Haar_random_state(std::vector<CTYPE>& state) {
    initialize_Haar_random_state_with_seed(state, (UINT)time(NULL));
}
void initialize_Haar_random_state_with_seed(
    std::vector<CTYPE>& state, UINT seed) {
    ITYPE dim = state.size();
#ifdef _OPENMP
    UINT threshold = 8;
    if (dim < (((ITYPE)1) << threshold)) {
        initialize_Haar_random_state_with_seed_single(state, seed);
    } else {
        initialize_Haar_random_state_with_seed_parallel(state, seed);
    }
#else
    initialize_Haar_random_state_with_seed_single(state, seed);
#endif
}

// single thread
void initialize_Haar_random_state_with_seed_single(
    std::vector<CTYPE>& state, UINT seed) {
    ITYPE dim = state.size();
    const int ignore_first = 40;
    double norm = 0.;
    ITYPE random_state[4];
    srand(seed);
    random_state[0] = rand();
    random_state[1] = rand();
    random_state[2] = rand();
    random_state[3] = rand();
    for (int i = 0; i < ignore_first; ++i) xor_shift(random_state);
    for (ITYPE index = 0; index < dim; ++index) {
        double r1, r2;
        r1 = random_normal(random_state);
        r2 = random_normal(random_state);
        state[index] = r1 + 1.i * r2;
        norm += r1 * r1 + r2 * r2;
    }
    norm = sqrt(norm);
    for (ITYPE index = 0; index < dim; ++index) {
        state[index] /= norm;
    }
}
#ifdef _OPENMP
void initialize_Haar_random_state_with_seed_parallel(
    std::vector<CTYPE>& state, UINT seed) {
    ITYPE dim = state.size();
    // multi thread
    OMPutil::get_inst().set_qulacs_num_threads(dim, 10);
    const int ignore_first = 40;
    const UINT thread_count = omp_get_max_threads();
    const ITYPE block_size = dim / thread_count;
    const ITYPE residual = dim % thread_count;

    ITYPE* random_state_list = (ITYPE*)malloc(sizeof(ITYPE) * 4 * thread_count);
    srand(seed);
    for (UINT i = 0; i < 4 * thread_count; ++i) {
        random_state_list[i] = rand();
    }

    double* norm_list = (double*)malloc(sizeof(double) * thread_count);
    for (UINT i = 0; i < thread_count; ++i) {
        norm_list[i] = 0;
    }
#pragma omp parallel
    {
        UINT thread_id = omp_get_thread_num();
        ITYPE* my_random_state = random_state_list + 4 * thread_id;
        ITYPE start_index = block_size * thread_id +
                            (residual > thread_id ? thread_id : residual);
        ITYPE end_index =
            block_size * (thread_id + 1) +
            (residual > (thread_id + 1) ? (thread_id + 1) : residual);
        ITYPE index;

        // ignore first randoms
        for (int i = 0; i < ignore_first; ++i) xor_shift(my_random_state);

        for (index = start_index; index < end_index; ++index) {
            double r1, r2;
            r1 = r2 = 1;
            r1 = random_normal(my_random_state);
            r2 = random_normal(my_random_state);
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
    free(random_state_list);
    free(norm_list);
}
#endif

ITYPE xor_shift(ITYPE* state) {
    ITYPE t;
    t = (state[0] ^ (state[0] << 11));
    state[0] = state[1];
    state[1] = state[2];
    state[2] = state[3];
    return (state[3] = (state[3] ^ (state[3] >> 19)) ^ (t ^ (t >> 8)));
}
double random_uniform(ITYPE* state) {
    return xor_shift(state) / ((double)ULONG_MAX + 1);
}
double random_normal(ITYPE* state) {
    return sqrt(-1.0 * log(1 - random_uniform(state))) *
           sin(2.0 * M_PI * random_uniform(state));
}