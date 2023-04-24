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
template <StateVectorImplementation IMPL>
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
template <StateVectorImplementation IMPL>
class StateVector {
private:
    UINT _qubit_count;
    ITYPE _dim;

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
     * @brief data
     * \~japanese-en 状態ベクトルのデータ
     */
    StateVectorData<IMPL> data;

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
     * @brief copy constructor
     * \~japanese-en コピーコンストラクタ
     * @param other コピー元
     */
    StateVector(const StateVector<IMPL>& other);

    /**
     * @brief copy assignment
     * \~japanese-en コピー代入演算子
     * @param other コピー元
     */
    StateVector operator=(const StateVector<IMPL>& other);

    /**
     * @brief move constructor
     * \~japanese-en ムーブコンストラクタ
     * @param other ムーブ元
     */
    StateVector(StateVector<IMPL>&& other) noexcept;

    /**
     * @brief move assignment
     * \~japanese-en ムーブ代入演算子
     * @param other ムーブ元
     */
    StateVector operator=(StateVector<IMPL>&& other) noexcept;

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

    /**
     * @brief calculate probability of observing zero on specified qubit
     * \~japanese-en
     * <code>target_qubit_index</code>の添え字の量子ビットを測定した時、0が観測される確率を計算する。
     *
     * 量子状態は変更しない。
     * @param target_qubit_index
     * @return 0が観測される確率
     */
    double get_zero_probability(UINT target_qubit_index) const;

    /**
     * @brief calculate probability of observing specified computational basis
     * \~japanese-en 複数の量子ビットを測定した時の周辺確率を計算する
     *
     * @param measured_values
     * 量子ビット数と同じ長さの0,1,2の配列。0,1はその値が観測され、2は測定をしないことを表す。
     * @return 計算された周辺確率
     */
    double get_marginal_probability(std::vector<UINT> measured_values) const;

    /**
     * @brief calculate entropy of probability distribution
     * \~japanese-en
     * 計算基底で測定した時得られる確率分布のエントロピーを計算する。
     *
     * @return エントロピー
     */
    double get_entropy() const;

    /**
     * @brief calculate norm
     * \~japanese-en 量子状態のノルムを計算する
     *
     * 量子状態のノルムは非ユニタリなゲートを作用した時に小さくなる。
     * @return ノルム
     */
    double get_squared_norm() const;

    /**
     * @brief normalize
     * \~japanese-en 量子状態を正規化する
     *
     * @param norm 自身のノルム
     */
    void normalize(double squared_norm);

    /**
     * @brief copy std::vector to this
     * \~japanese-en <code>state</code>の量子状態を自身へコピーする。
     * @param state コピー元
     */
    void load(const std::vector<CTYPE>& state);

    /**
     * @brief get copied state vector
     * \~japanese-en 量子状態のコピーををstd::vector<CTYPE>として得る
     * @return 量子状態のコピー
     */
    std::vector<CTYPE> duplicate_data() const;

    /**
     * @brief do sampling of measured computational basis
     * \~japanese-en 量子状態を測定した際の計算基底のサンプリングを行う
     *
     * @param[in] sampling_count サンプリングを行う回数
     * @param[in] random_seed サンプリングで乱数を振るシード値
     * @return サンプルされた値のリスト
     */
    std::vector<ITYPE> sampling(UINT sampling_count, UINT random_seed);

    /**
     * @brief do sampling of measured computational basis
     * \~japanese-en 量子状態を測定した際の計算基底のサンプリングを行う
     *
     * @param[in] sampling_count サンプリングを行う回数
     * @return サンプルされた値のリスト
     */
    std::vector<ITYPE> sampling(UINT sampling_count);

    /**
     * @brief export to property tree
     * \~japanese-en property treeに変換
     *
     * @return ptree
     */
    boost::property_tree::ptree to_ptree() const = 0;
};

namespace state {
/**
 * @brief calculate inner product of two StateVector
 * \~japanese-en 量子状態間の内積を計算する
 *
 * @param[in] state_bra 内積のブラ側の量子状態
 * @param[in] state_ket 内積のケット側の量子状態
 * @return 内積の値
 */
template <StateVectorImplementation IMPL>
CTYPE DllExport inner_product(
    const StateVector<IMPL>& state_bra, const StateVector<IMPL>& state_ket);

/**
 * @brief calculate tensor product of two StateVector
 * \~japanese-en 量子状態間のテンソル積を計算する
 *
 * @param[in] state_left 上位ビット側の量子状態
 * @param[in] state_right 下位ビット側の量子状態
 * @return テンソル積
 */
template <StateVectorImplementation IMPL>
DllExport StateVector<IMPL> tensor_product(
    const StateVector<IMPL>& state_left, const StateVector<IMPL>& state_right);

/**
 * @brief permutate qubit index
 * \~japanese-en 量子ビットの順番を入れ替えた量子状態を返す
 *
 * @param[in] state 変換前の量子状態
 * @param[in] qubit_order 量子ビット入れ替えの順列
 * @return 変換後の量子状態
 */
template <StateVectorImplementation IMPL>
DllExport StateVector<IMPL> permutate_qubit(
    const StateVector<IMPL>& state, const std::vector<UINT>& qubit_order);

/**
 * @brief permutate qubit index
 * \~japanese-en 特定の量子ビットへの射影を行う
 *
 * @param[in] state 変換前の量子状態
 * @param[in] target 射影対象の量子ビットインデックス
 * @param[in] projection 射影先(0または1)
 * @return 変換後の量子状態
 */
template <StateVectorImplementation IMPL>
DllExport StateVector<IMPL> drop_qubit(const StateVector<IMPL>& state,
    const std::vector<UINT>& target, const std::vector<UINT>& projection);

/**
 * @brief create superposition of states of coef1|state1>+coef2|state2>
 * \japanese-en 2量子状態の係数付き重ね合わせ状態を作成する
 *
 * @param[in] coef1 state1の係数
 * @param[in] state1 1つ目の量子状態
 * @param[in] coef2 state2の係数
 * @param[in] state2 2つ目の量子状態
 */
template <StateVectorImplementation IMPL>
DllExport StateVector<IMPL> make_superposition(CTYPE coef1,
    const StateVector<IMPL>& state1, CTYPE coef2,
    const StateVector<IMPL>& state2);
}  // namespace state
