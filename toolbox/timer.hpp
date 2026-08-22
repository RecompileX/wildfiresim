#pragma once

#include <algorithm>
#include <cmath>

namespace toolbox {

class Timer {
public:
    explicit Timer(float seconds)
        : duration_(validTime(seconds) ? seconds : 0.0f) {}

    void update(float deltaTime) {
        if (validTime(deltaTime) && deltaTime > 0.0f) {
            elapsed_ += deltaTime;
        }
    }

    [[nodiscard]] bool done() const {
        return elapsed_ >= duration_;
    }

    void reset() {
        elapsed_ = 0.0f;
    }

    [[nodiscard]] float elapsed() const {
        return elapsed_;
    }

    [[nodiscard]] float remaining() const {
        return std::max(duration_ - elapsed_, 0.0f);
    }

private:
    static bool validTime(float value) {
        return std::isfinite(value) && value >= 0.0f;
    }

    float duration_ = 0.0f;
    float elapsed_ = 0.0f;
};

} // namespace toolbox
