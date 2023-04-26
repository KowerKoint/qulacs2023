#include "../general/number_util.hpp"
#include "../general/omp_util.hpp"
#include "stat_ops.hpp"

double m0_prob(const std::vector<CTYPE>& state, UINT target_qubit_index) {
    double sum = 0;
#ifdef _OPENMP
    OMPutil::get_inst().set_qulacs_num_threads(dim, 10);
#pragma omp parallel for reduction(+ : sum)
#endif
    for (ITYPE state_index = 0, loop_dim = state.size() >> 1;
         state_index < loop_dim; ++state_index) {
        ITYPE basis_index =
            insert_zero_to_basis_index(state_index, target_qubit_index);
        sum += std::norm(state[basis_index]);
    }
#ifdef _OPENMP
    OMPutil::get_inst().reset_qulacs_num_threads();
#endif
    return sum;
}

double marginal_prob(const std::vector<CTYPE>& state,
    const std::vector<UINT>& sorted_target_qubit_index_list,
    const std::vector<UINT>& measured_value_list) {
    double sum = 0;
#ifdef _OPENMP
    OMPutil::get_inst().set_qulacs_num_threads(dim, 10);
#pragma omp parallel for reduction(+ : sum)
#endif
    for (ITYPE state_index = 0,
               loop_dim = state.size() >> sorted_target_qubit_index_list.size();
         state_index++) {
        ITYPE basis_index = state_index;
        for (UINT cursor = 0; cursor < sorted_target_qubit_index_list.size();
             ++cursor) {
            UINT target_qubit_index = sorted_target_qubit_index_list[cursor];
            UINT measured_value = measured_value_list[cursor];
            basis_index =
                insert_zero_to_basis_index(basis_index, target_qubit_index) |
                (ITYPE)measured_value << target_qubit_index;
        }
        sum += std::norm(state[basis_index]);
    }
#ifdef _OPENMP
    OMPutil::get_inst().reset_qulacs_num_threads();
#endif
    return sum;
}

double measurement_distribution_entropy(const std::vector<CTYPE>& state) {
    double ent = 0;
#ifdef _OPENMP
    OMPutil::get_inst().set_qulacs_num_threads(dim, 10);
#pragma omp parallel for reduction(+ : ent)
#endif
    for (ITYPE state_index = 0, loop_dim = state.size(); state_index < loop_dim;
         ++state_index) {
        double prob = std::norm(state[state_index]);
        if (prob > 0) {
            ent -= prob * std::log(prob);
        }
    }
#ifdef _OPENMP
    OMPutil::get_inst().reset_qulacs_num_threads();
#endif
    return ent;
}