// SPDX-License-Identifier: MIT
//
// Dependency-free unit tests for game-engine-core.

#include <cstdio>

#include "game_engine_core/fixed.hpp"
#include "game_engine_core/timestep.hpp"

namespace {

int g_failures = 0;
int g_checks = 0;

#define CHECK(cond)                                                            \
    do {                                                                       \
        ++g_checks;                                                            \
        if (!(cond)) {                                                         \
            std::printf("FAIL %s:%d: CHECK(%s)\n", __FILE__, __LINE__, #cond); \
            ++g_failures;                                                      \
        }                                                                      \
    } while (0)

float absf(float x) {
    return x < 0.0f ? -x : x;
}

using ge::Fixed;

void test_fixed_basic_arithmetic() {
    const Fixed a = Fixed::fromInt(3);
    const Fixed b = Fixed::fromInt(4);
    CHECK((a + b).toInt() == 7);
    CHECK((b - a).toInt() == 1);
    CHECK((a * b).toInt() == 12);
    CHECK((b / a).toFloat() > 1.33f);
    CHECK((b / a).toFloat() < 1.34f);
}

void test_fixed_float_roundtrip() {
    const float values[] = {0.0f, 1.5f, -2.25f, 9.81f, -0.001f};
    for (float v : values) {
        const Fixed f = Fixed::fromFloat(v);
        // Round-trip error is bounded by one quantization step (1/65536).
        CHECK(absf(f.toFloat() - v) < (1.0f / 32768.0f));
    }
}

void test_fixed_is_deterministic() {
    // The same integer ops must yield identical raw bits every run/platform.
    const Fixed a = Fixed::fromFloat(1.1f);
    const Fixed b = Fixed::fromFloat(2.2f);
    const Fixed c = a * b + a;
    CHECK(c.raw() == (Fixed::fromFloat(1.1f) * Fixed::fromFloat(2.2f) +
                      Fixed::fromFloat(1.1f))
                         .raw());
}

void test_timestep_step_count() {
    ge::FixedTimestep clock(0.10f);    // 100 ms steps
    CHECK(clock.advance(0.05f) == 0);  // not enough yet
    CHECK(clock.advance(0.06f) == 1);  // 0.11s accumulated -> 1 step
    // 0.01s remained; +0.20s = 0.21s -> 2 steps.
    CHECK(clock.advance(0.20f) == 2);
}

void test_timestep_alpha_in_range() {
    ge::FixedTimestep clock(0.10f);
    clock.advance(0.05f);
    const float a = clock.alpha();
    CHECK(a >= 0.0f && a < 1.0f);
}

void test_timestep_caps_work() {
    ge::FixedTimestep clock(0.01f, /*maxStepsPerFrame=*/4);
    // A huge frame would demand 100 steps; the cap must limit it.
    const int steps = clock.advance(1.0f);
    CHECK(steps == 4);
}

}  // namespace

int main() {
    test_fixed_basic_arithmetic();
    test_fixed_float_roundtrip();
    test_fixed_is_deterministic();
    test_timestep_step_count();
    test_timestep_alpha_in_range();
    test_timestep_caps_work();

    std::printf("%d checks, %d failures\n", g_checks, g_failures);
    return g_failures == 0 ? 0 : 1;
}
