/**
 * @file random.hpp
 * @brief Random Generator
 */

#pragma once

#include <array>
#include <climits>
#include <random>

#include "type.hpp"

class XorShiftEngine {
private:
    std::array<ITYPE, 4> state;

public:
    void seed(UINT seed_) {
        srand(seed_);
        state[0] = rand();
        state[1] = rand();
        state[2] = rand();
        state[3] = rand();
    }

    ITYPE operator()() {
        ITYPE t;
        t = (state[0] ^ (state[0] << 11));
        state[0] = state[1];
        state[1] = state[2];
        state[2] = state[3];
        return (state[3] = (state[3] ^ (state[3] >> 19)) ^ (t ^ (t >> 8)));
    }

    ITYPE max() { return ULONG_LONG_MAX; }
    ITYPE min() { return 0ULL; }

    XorShiftEngine(UINT seed_) { seed(seed_); }
};

/**
 * \~japanese-en 乱数を管理するクラス
 */
class Random {
private:
    std::uniform_real_distribution<double> uniform_dist;
    std::normal_distribution<double> normal_dist;
    XorShiftEngine engine;

public:
    /**
     * \~japanese-en コンストラクタ
     */
    Random(UINT seed) : uniform_dist(0, 1), normal_dist(0, 1), engine(seed) {}

    /**
     * \~japanese-en シードを設定する
     *
     * @param seed シード値
     */
    void set_seed(uint64_t seed) { engine.seed(seed); }
    /**
     * \~japanese-en \f$[0,1)\f$の一様分布から乱数を生成する
     *
     * @return 生成された乱数
     */
    double uniform() { return uniform_dist(engine); }

    /**
     * \~japanese-en 期待値0、分散1の正規分から乱数を生成する
     *
     * @return double 生成された乱数
     */
    double normal() { return normal_dist(engine); }

    /**
     * \~japanese-en 64bit整数の乱数を生成する
     *
     * @return 生成された乱数
     */
    uint64_t int64() { return engine(); }
};