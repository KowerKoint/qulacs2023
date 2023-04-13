#include "state_vector.hpp"

#include "internal/default/init_ops.hpp"
#include "internal/general/check_constraints.hpp"

template <StateVectorImplementation implementation>
StateVector<implementation>::StateVector(UINT qubit_count_)
    : qubit_count(_qubit_count),
      dim(_dim),
      classical_register(_classical_register) {
    this->_qubit_count = qubit_count_;
    this->_dim = 1ULL << qubit_count_;
    if constexpr (implementation == StateVectorImplementation::DEFAULT) {
        this->data.data.resize(this->_dim);
        this->data.data[0] = 1.0;
    } else {
        static_assert(false, "this implementation is not supported");
    }
}

template <StateVectorImplementation implementation>
StateVector<implementation>::~StateVector() {}

template <StateVectorImplementation implementation>
void StateVector<implementation>::set_zero_state() {
    if constexpr (implementation == StateVectorImplementation::DEFAULT) {
        initialize_quantum_state(this->data.data);
    } else {
        static_assert(false, "this implementation is not supported");
    }
}

template <StateVectorImplementation implementation>
void StateVector<implementation>::set_zero_norm_state() {
    set_zero_state();
    this->data[0] = 0.0;
}

template <StateVectorImplementation implementation>
void StateVector<implementation>::set_computational_basis(ITYPE comp_basis) {
    check_computational_basis(comp_basis, 0ULL, this->_dim);
    set_zero_state();
    this->data[0] = 0.0;
    this->data[comp_basis] = 1.0;
}

template <StateVectorImplementation implementation>
void StateVector<implementation>::set_Haar_random_state() {
    if constexpr (implementation == StateVectorImplementation::DEFAULT) {
        initialize_Haar_random_state(this->data.data);
    } else {
        static_assert(false, "this implementation is not supported");
    }
}