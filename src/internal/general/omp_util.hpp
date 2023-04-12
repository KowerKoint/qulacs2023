#pragma once

#include "constant.hpp"
#include "omp.h"
#include "type.hpp"

class OMPutil {
private:
    UINT qulacs_num_default_thread_max = 1;
    UINT qulacs_num_thread_max = 0;
    UINT qulacs_force_threshold = 0;

    OMPutil() {
        qulacs_num_thread_max = omp_get_max_threads();
        if (const char* tmp = std::getenv("QULACS_NUM_THREADS")) {
            const UINT tmp_val = strtol(tmp, nullptr, 0);
            if (0 < tmp_val && tmp_val <= MAX_NUM_THREADS)
                qulacs_num_thread_max = tmp_val;
        }

        qulacs_force_threshold = 0;
        if (const char* tmp = std::getenv("QULACS_PARALLEL_NQUBIT_THRESHOLD")) {
            const UINT tmp_val = strtol(tmp, nullptr, 0);
            if (0 < tmp_val && tmp_val <= PARALLEL_NQUBIT_THRESHOLD)
                qulacs_force_threshold = tmp_val;
        }

        qulacs_num_default_thread_max = omp_get_max_threads();
    }
    ~OMPutil() = default;

public:
    OMPutil(const OMPutil&) = delete;
    OMPutil& operator=(const OMPutil&) = delete;
    OMPutil(OMPutil&&) = delete;
    OMPutil& operator=(OMPutil&&) = delete;

    static OMPutil& get_inst() {
        static OMPutil instance;
        return instance;
    }
    void set_qulacs_num_threads(ITYPE dim, UINT para_threshold);
    void reset_qulacs_num_threads();
};