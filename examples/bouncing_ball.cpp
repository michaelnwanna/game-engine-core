// SPDX-License-Identifier: MIT
//
// bouncing_ball.cpp — A deterministic simulation driven by a fixed timestep.
//
// A ball falls under gravity and bounces, with *all* physics in Q16.16
// fixed-point so the trajectory is bit-for-bit identical on any machine. The
// render loop is decoupled: variable frame times are converted into a whole
// number of fixed simulation steps.

#include <cstdio>

#include "game_engine_core/fixed.hpp"
#include "game_engine_core/timestep.hpp"

namespace {

using ge::Fixed;

struct Ball {
    Fixed y = Fixed::fromInt(10);  // start 10 units up
    Fixed vy = Fixed::fromInt(0);
};

// One fixed simulation step at dt seconds.
void step(Ball& b, Fixed dt) {
    const Fixed gravity = Fixed::fromFloat(-9.81f);
    b.vy += gravity * dt;
    b.y += b.vy * dt;
    if (b.y < Fixed::fromInt(0)) {  // hit the floor
        b.y = Fixed::fromInt(0);
        b.vy = -b.vy * Fixed::fromFloat(0.7f);  // lose energy on bounce
    }
}

}  // namespace

int main() {
    Ball ball;
    ge::FixedTimestep clock(1.0f / 60.0f);  // 60 Hz simulation
    const Fixed dt = Fixed::fromFloat(clock.step());

    // Simulate ~2 seconds using deliberately irregular frame times.
    const float frames[] = {0.016f, 0.020f, 0.033f, 0.008f, 0.050f};
    float simTime = 0.0f;
    for (int frame = 0; frame < 40; ++frame) {
        const float frameDt = frames[frame % 5];
        const int steps = clock.advance(frameDt);
        for (int s = 0; s < steps; ++s) {
            step(ball, dt);
        }
        simTime += frameDt;
        std::printf("t=%.3fs  steps=%d  height=%.3f\n",
                    static_cast<double>(simTime), steps,
                    static_cast<double>(ball.y.toFloat()));
    }
    return 0;
}
