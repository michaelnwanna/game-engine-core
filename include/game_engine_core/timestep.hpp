// SPDX-License-Identifier: MIT
//
// timestep.hpp — Fixed-timestep accumulator (the "fix your timestep" pattern).
//
// Rendering runs as fast as the machine allows, but the simulation must advance
// in fixed, equal-size steps so physics is stable and reproducible regardless
// of frame rate. This accumulates real elapsed time and reports how many fixed
// steps to run, exposing an interpolation factor for smooth rendering between
// them — and capping work per frame to avoid the "spiral of death".

#ifndef GE_TIMESTEP_HPP
#define GE_TIMESTEP_HPP

namespace ge {

class FixedTimestep {
public:
    explicit FixedTimestep(float stepSeconds, int maxStepsPerFrame = 8) noexcept
        : step_(stepSeconds), maxSteps_(maxStepsPerFrame) {}

    /// Feed elapsed real time; returns how many fixed steps to run this frame.
    int advance(float frameSeconds) noexcept {
        accumulator_ += frameSeconds;
        int steps = 0;
        while (accumulator_ >= step_ && steps < maxSteps_) {
            accumulator_ -= step_;
            ++steps;
        }
        // Spiral-of-death guard: if we're still behind after the cap, drop the
        // backlog rather than accumulating unbounded work.
        if (accumulator_ >= step_) {
            accumulator_ = 0.0f;
        }
        return steps;
    }

    float step() const noexcept { return step_; }

    /// Fraction toward the next step, in [0, 1), for render interpolation.
    float alpha() const noexcept { return accumulator_ / step_; }

private:
    float step_;
    int maxSteps_;
    float accumulator_ = 0.0f;
};

}  // namespace ge

#endif  // GE_TIMESTEP_HPP
