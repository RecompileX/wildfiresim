#pragma once

#include <algorithm>
#include <chrono>
#include <cmath>
#include <random>
#include <stdexcept>
#include <utility>

namespace toolbox {
namespace detail {

inline std::mt19937& randomEngine() {
    thread_local std::mt19937 engine([] {
        try {
            std::random_device rd;
            std::seed_seq seed{
                rd(), rd(), rd(), rd(),
                rd(), rd(), rd(), rd()
            };
            return std::mt19937(seed);
        } catch (...) {
            const auto seed = static_cast<std::mt19937::result_type>(
                std::chrono::steady_clock::now().time_since_epoch().count()
            );
            return std::mt19937(seed);
        }
    }());

    return engine;
}

} // namespace detail

inline int random(int min, int max) {
    if (min > max) {
        std::swap(min, max);
    }

    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(detail::randomEngine());
}

inline float random(float min, float max) {
    if (!std::isfinite(min) || !std::isfinite(max)) {
        throw std::invalid_argument("toolbox::random float bounds must be finite");
    }

    if (min > max) {
        std::swap(min, max);
    }

    std::uniform_real_distribution<float> distribution(min, max);
    return distribution(detail::randomEngine());
}

inline bool chance(float percent) {
    if (std::isnan(percent) || percent <= 0.0f) {
        return false;
    }

    if (percent >= 100.0f) {
        return true;
    }

    percent = std::clamp(percent, 0.0f, 100.0f);
    return random(0.0f, 100.0f) < percent;
}

} // namespace toolbox
