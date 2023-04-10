#include "state_vector.hpp"

template <StateVectorImplementation implementation>
StateVector<implementation>::StateVector(UINT qubit_count_) : qubit_count(_qubit_count), dim(_dim), classical_register(_classical_register) {
    this->_qubit_count = qubit_count_;
    this->_dim = 1ULL << qubit_count_;
    if constexpr(implementation == StateVectorImplementation::DEFAULT) {
        this->data.data.resize(this->_dim);
        this->data.data[0] = 1.0;
    } else {
        static_assert(false, "this implementation is not supported");
    }
}