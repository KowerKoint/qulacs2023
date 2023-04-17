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
enum class StateVectorImplementation { DEFAULT };

/**
 * @brief StateVector Data Structure
 */
template <StateVectorImplementation implementation>
struct StateVectorData {};

template <>
struct StateVectorData<StateVectorImplementation::DEFAULT> {
    std::vector<CTYPE> data;

    const CTYPE& operator[](ITYPE index) const { return data[index]; }
    CTYPE& operator[](ITYPE index) { return data[index]; }
};

/**
 * @brief StateVector expression of quantum state
 * \~japanese-en 量子状態の状態ベクトルによる表現
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
     * \~japanese-en 量子ビット数
     */
    const UINT& qubit_count;

    /**
     * @brief dimension of state vector
     * \~japanese-en 量子状態の次元
     */
    const ITYPE& dim;

    /**
     * @brief classical register
     * \~japanese-en 古典ビットのレジスタ
     */
    const std::vector<UINT>& classical_register;

    /**
     * @brief data
     * \~japanese-en 状態ベクトルのデータ
     */
    StateVectorData<implementation> data;

    /**
     * @brief constructor
     * \~japanese-en コンストラクタ
     * @param qubit_count num of qubits
     */
    StateVector(UINT qubit_count_);

    /**
     * @brief destructor
     * \~japanese-en デストラクタ
     */
    ~StateVector();

    /**
     * @brief intialize state to computational basis "0"
     * \~japanese-en 量子状態を計算基底の0状態に初期化する
     */
    void set_zero_state();
    /**
     * @brief initialize state to norm 0 state
     * \~japanese-en 量子状態をノルム0の状態にする
     */
    void set_zero_norm_state();
    /**
     * @brief initialize state to computational basis <code>comp_basis></code>
     * \~japanese-en 量子状態を<code>comp_basis</code>の計算基底状態に初期化する
     */
    void set_computational_basis(ITYPE comp_basis);
    /**
     * @brief initialize state to Haar random state
     * \~japanese-en 量子状態をHaar
     * randomにサンプリングされた量子状態に初期化する
     */
    void set_Haar_random_state();
};