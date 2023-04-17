#include "omp_util.hpp"

void OMPutil::set_qulacs_num_threads(ITYPE dim, UINT para_threshold) {
    UINT threshold = para_threshold;
    if (qulacs_force_threshold > 0) threshold = qulacs_force_threshold;
    if (dim < (((ITYPE)1) << threshold)) {
        omp_set_num_threads(1);
    } else {
        omp_set_num_threads(qulacs_num_thread_max);
    }
}

void OMPutil::reset_qulacs_num_threads() {
    omp_set_num_threads(qulacs_num_default_thread_max);
}