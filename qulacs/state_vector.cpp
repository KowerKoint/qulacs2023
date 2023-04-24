#include "state_vector.hpp"

#include <cassert>

#include "internal/default/init_ops.hpp"
#include "internal/general/check_constraints.hpp"

template <>
struct StateVectorData<StateVectorImplementation::DEFAULT> {
    std::vector<CTYPE> data;

    // destructor and copy/move constructor/assignment is default
    StateVectorData(UINT qubit_count) : data(1ULL << qubit_count) {}
};

template <StateVectorImplementation IMPL>
StateVector<IMPL>::StateVector(UINT qubit_count_)
    : qubit_count(_qubit_count),
      dim(_dim),
      _qubit_count(qubit_count_),
      _dim(1ULL << qubit_count_),
      data(qubit_count_) {}

template <StateVectorImplementation IMPL>
void StateVector<IMPL>::set_zero_state() {
    if constexpr (IMPL == StateVectorImplementation::DEFAULT) {
        initialize_quantum_state(this->data.data);
    } else {
        assert(false);  // unknown IMPL. must be unreachable
    }
}

template <StateVectorImplementation IMPL>
void StateVector<IMPL>::set_zero_norm_state() {
    set_zero_state();
    if constexpr (IMPL == StateVectorImplementation::DEFAULT) {
        this->data.data[0] = 0.0;
    } else {
        assert(false);  // unknown IMPL. must be unreachable
    }
}

template <StateVectorImplementation IMPL>
void StateVector<IMPL>::set_computational_basis(ITYPE comp_basis) {
    check_out_of_range("comp_basis", comp_basis, 0ULL, this->_dim);
    set_zero_state();
    if constexpr (IMPL == StateVectorImplementation::DEFAULT) {
        this->data.data[0] = 0.0;
        this->data.data[comp_basis] = 1.0;
    } else {
        assert(false);  // unknown IMPL. must be unreachable
    }
}

template <StateVectorImplementation IMPL>
void StateVector<IMPL>::set_Haar_random_state(UINT seed) {
    if constexpr (IMPL == StateVectorImplementation::DEFAULT) {
        initialize_Haar_random_state(this->data.data, seed);
    } else {
        assert(false);  // unknown IMPL. must be unreachable
    }
}

template class StateVector<StateVectorImplementation::DEFAULT>;