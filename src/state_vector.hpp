/**
 * @file state_vector.hpp
 * @brief StateVector class definition
 */

#pragma once
#include <vector>

#include "internal/general/type.hpp"

/**
 * @brief type of StateVector Implementation
 */
enum class StateVectorImplementation {
    DEFAULT
};

/**
 * @brief StateVector Data Structure
 */
template <StateVectorImplementation implementation>
struct StateVectorData{
};

template <>
struct StateVectorData<StateVectorImplementation::DEFAULT> {
    std::vector<CTYPE> data;
};

/**
 * @brief StateVector expression of quantum state
 */
template <StateVectorImplementation implementation>
class StateVector {
private:
    UINT _qubit_count;
    ITYPE _dim;
    std::vector<UINT> _classical_register;

public:
    /**
     * @brief num of qubits
    */
    const UINT& qubit_count;

    /**
     * @brief dimension of state vector
    */
    const ITYPE& dim;

    /**
     * @brief classical register
    */
    const std::vector<UINT>& classical_register;
    
    /**
     * @brief data
    */
    StateVectorData<implementation> data;
    
    /**
     * @brief constructor
     * @param qubit_count num of qubits
     */
    StateVector(UINT qubit_count_) : qubit_count(_qubit_count), dim(_dim), classical_register(_classical_register) {
        this->_qubit_count = qubit_count_;
        this->_dim = 1ULL << _qubit_count;
        this->_classical_register.resize(_qubit_count);
        
        if constexpr (implementation == StateVectorImplementation::DEFAULT) {
            this->data.data.resize(this->_dim);
        }
    }
};