#include "state_vector.hpp"

#include <cassert>

#include "internal/default/init_ops.hpp"
#include "internal/default/stat_ops.hpp"
#include "internal/default/update_ops.hpp"
#include "internal/general/check_constraints.hpp"
#include "internal/general/random.hpp"

#ifdef _USE_MPI
#include "internal/mpi/mpi_util.hpp"
#endif

constexpr StateVectorImplementation DEFAULT =
    StateVectorImplementation::DEFAULT;
constexpr StateVectorImplementation MPI = StateVectorImplementation::MPI;

template <>
struct StateVectorData<DEFAULT> {
    std::vector<CTYPE> data;

    StateVectorData(UINT qubit_count) : data(1ULL << qubit_count) {}
};

template <>
struct StateVectorData<MPI> {
    std::vector<CTYPE> data;
    UINT inner_qc;
    UINT outer_qc;

    StateVectorData(UINT qubit_count) {
#ifdef _USE_MPI
        MPIutil& mpiutil = MPIutil::getinst();
        UINT mpirank = mpiutil.get_rank();
        UINT mpisize = mpiutil.get_size();
        UINT lognodes = (UINT)std::log2(mpisize);
        inner_qc = qubit_count - lognodes;
        outer_qc = lognodes;
#else
        assert(false);  // MPI is not available
#endif
    }
};

template <StateVectorImplementation IMPL>
StateVector<IMPL>::StateVector(UINT qubit_count_)
    : _qubit_count(qubit_count_),
      _dim(1ULL << qubit_count_),
      _data(qubit_count_) {}

template <StateVectorImplementation IMPL>
void StateVector<IMPL>::set_zero_state() {
    if constexpr (IMPL == DEFAULT) {
        normal::initialize_quantum_state(this->_data.data);
    } else {
        assert(false);  // unknown IMPL. must be unreachable
    }
}

template <StateVectorImplementation IMPL>
void StateVector<IMPL>::set_zero_norm_state() {
    set_zero_state();
    if constexpr (IMPL == DEFAULT) {
        this->_data.data[0] = 0.0;
    } else {
        assert(false);  // unknown IMPL. must be unreachable
    }
}

template <StateVectorImplementation IMPL>
void StateVector<IMPL>::set_computational_basis(ITYPE comp_basis) {
    check_out_of_range("comp_basis", comp_basis, 0ULL, this->_dim);
    set_zero_state();
    if constexpr (IMPL == DEFAULT) {
        this->_data.data[0] = 0.0;
        this->_data.data[comp_basis] = 1.0;
    } else {
        assert(false);  // unknown IMPL. must be unreachable
    }
}

template <StateVectorImplementation IMPL>
void StateVector<IMPL>::set_Haar_random_state(UINT seed) {
    if constexpr (IMPL == DEFAULT) {
        normal::initialize_Haar_random_state(this->_data.data, seed);
    } else {
        assert(false);  // unknown IMPL. must be unreachable
    }
}

template <StateVectorImplementation IMPL>
double StateVector<IMPL>::get_zero_probability(UINT target_qubit_index) const {
    check_out_of_range(
        "target_qubit_index", target_qubit_index, 0U, this->_qubit_count);
    if constexpr (IMPL == DEFAULT) {
        return normal::m0_prob(this->_data.data, target_qubit_index);
    } else {
        assert(false);  // unknown IMPL. must be unreachable
    }
}

template <StateVectorImplementation IMPL>
double StateVector<IMPL>::get_marginal_probability(
    const std::vector<UINT>& measured_values) const {
    check_equal(
        "measured_values", (UINT)measured_values.size(), this->_qubit_count);
    std::vector<UINT> target_index;
    std::vector<UINT> target_value;
    for (UINT i = 0; i < measured_values.size(); ++i) {
        UINT measured_value = measured_values[i];
        if (measured_value == 0 || measured_value == 1) {
            target_index.push_back(i);
            target_value.push_back(measured_value);
        }
    }
    if constexpr (IMPL == DEFAULT) {
        return normal::marginal_prob(
            this->_data.data, target_index, target_value);
    } else {
        assert(false);  // unknown IMPL. must be unreachable
    }
}

template <StateVectorImplementation IMPL>
double StateVector<IMPL>::get_entropy() const {
    if constexpr (IMPL == DEFAULT) {
        return normal::measurement_distribution_entropy(this->_data.data);
    } else {
        assert(false);  // unknown IMPL. must be unreachable
    }
}

template <StateVectorImplementation IMPL>
double StateVector<IMPL>::get_squared_norm() const {
    if constexpr (IMPL == DEFAULT) {
        return normal::state_norm_squared(this->_data.data);
    } else {
        assert(false);  // unknown IMPL. must be unreachable
    }
}

template <StateVectorImplementation IMPL>
void StateVector<IMPL>::normalize(double squared_norm) {
    if constexpr (IMPL == DEFAULT) {
        normal::normalize(this->_data.data, squared_norm);
    } else {
        assert(false);  // unknown IMPL. must be unreachable
    }
}

template <StateVectorImplementation IMPL>
void StateVector<IMPL>::load(const std::vector<CTYPE>& state) {
    check_equal("state.size()", (ITYPE)state.size(), this->_dim);
    if constexpr (IMPL == DEFAULT) {
        this->_data.data = state;
    } else {
        assert(false);  // unknown IMPL. must be unreachable
    }
}

template <StateVectorImplementation IMPL>
void StateVector<IMPL>::load(std::vector<CTYPE>&& state) {
    check_equal("state.size()", (ITYPE)state.size(), this->_dim);
    if constexpr (IMPL == DEFAULT) {
        this->_data.data = state;
    } else {
        assert(false);  // unknown IMPL. must be unreachable
    }
}

template <StateVectorImplementation IMPL>
std::vector<CTYPE> StateVector<IMPL>::duplicate_data() const {
    if constexpr (IMPL == DEFAULT) {
        return this->_data.data;
    } else {
        assert(false);  // unknown IMPL. must be unreachable
    }
}

template <StateVectorImplementation IMPL>
std::vector<ITYPE> StateVector<IMPL>::sampling(
    UINT sampling_count, UINT seed) const {
    if constexpr (IMPL == DEFAULT) {
        std::vector<double> stacked_prob;
        std::vector<ITYPE> result;
        double sum = 0.;
        stacked_prob.push_back(0.);
        for (UINT i = 0; i < this->dim; ++i) {
            sum += std::norm(this->_data.data[i]);
            stacked_prob.push_back(sum);
        }

        Random random(seed);
        for (UINT count = 0; count < sampling_count; ++count) {
            double r = random.uniform();
            auto ite =
                std::lower_bound(stacked_prob.begin(), stacked_prob.end(), r);
            auto index = std::distance(stacked_prob.begin(), ite) - 1;
            result.push_back(index);
        }
        return result;
    } else {
        assert(false);  // unknown IMPL. must be unreachable
    }
}

template class StateVector<DEFAULT>;