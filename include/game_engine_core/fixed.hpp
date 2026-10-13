// SPDX-License-Identifier: MIT
//
// fixed.hpp — Q16.16 fixed-point number.
//
// Floating-point results can differ across compilers, CPUs and optimisation
// levels — fatal for lockstep multiplayer, replays, and reproducible physics.
// Integer fixed-point is bit-for-bit deterministic everywhere, which is why
// many engines run their simulation in fixed-point. This is a compact Q16.16
// type (16 integer bits, 16 fractional bits) backed by a single int32.

#ifndef GE_FIXED_HPP
#define GE_FIXED_HPP

#include <cstdint>

namespace ge {

class Fixed {
public:
    static constexpr int kFracBits = 16;
    static constexpr int32_t kOne = 1 << kFracBits;

    constexpr Fixed() noexcept = default;

    static constexpr Fixed fromRaw(int32_t raw) noexcept {
        Fixed f;
        f.raw_ = raw;
        return f;
    }
    static constexpr Fixed fromInt(int32_t v) noexcept {
        return fromRaw(v * kOne);
    }
    static Fixed fromFloat(float v) noexcept {
        const float scaled = v * static_cast<float>(kOne);
        return fromRaw(static_cast<int32_t>(scaled >= 0.0f ? scaled + 0.5f
                                                           : scaled - 0.5f));
    }

    constexpr int32_t raw() const noexcept { return raw_; }
    constexpr int32_t toInt() const noexcept { return raw_ >> kFracBits; }
    float toFloat() const noexcept {
        return static_cast<float>(raw_) / static_cast<float>(kOne);
    }

    constexpr Fixed operator+(Fixed o) const noexcept {
        return fromRaw(raw_ + o.raw_);
    }
    constexpr Fixed operator-(Fixed o) const noexcept {
        return fromRaw(raw_ - o.raw_);
    }
    constexpr Fixed operator-() const noexcept { return fromRaw(-raw_); }

    // Widen to 64-bit for the intermediate product, then rescale.
    constexpr Fixed operator*(Fixed o) const noexcept {
        const int64_t p = static_cast<int64_t>(raw_) * o.raw_;
        return fromRaw(static_cast<int32_t>(p >> kFracBits));
    }
    constexpr Fixed operator/(Fixed o) const noexcept {
        const int64_t n = static_cast<int64_t>(raw_) << kFracBits;
        return fromRaw(static_cast<int32_t>(n / o.raw_));
    }

    constexpr bool operator==(Fixed o) const noexcept {
        return raw_ == o.raw_;
    }
    constexpr bool operator!=(Fixed o) const noexcept {
        return raw_ != o.raw_;
    }
    constexpr bool operator<(Fixed o) const noexcept { return raw_ < o.raw_; }
    constexpr bool operator>(Fixed o) const noexcept { return raw_ > o.raw_; }
    constexpr bool operator<=(Fixed o) const noexcept {
        return raw_ <= o.raw_;
    }
    constexpr bool operator>=(Fixed o) const noexcept {
        return raw_ >= o.raw_;
    }

    constexpr Fixed& operator+=(Fixed o) noexcept {
        raw_ += o.raw_;
        return *this;
    }

private:
    int32_t raw_ = 0;
};

}  // namespace ge

#endif  // GE_FIXED_HPP
