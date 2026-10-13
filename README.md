# game-engine-core

> The deterministic heart of a real-time engine: a **fixed-timestep simulation
> loop** and a **fixed-point math** type — so physics and gameplay are stable and
> bit-for-bit reproducible across machines, frame rates, and replays.

[![CI](https://github.com/michaelnwanna/game-engine-core/actions/workflows/ci.yml/badge.svg)](../../actions/workflows/ci.yml)
![C++17](https://img.shields.io/badge/C%2B%2B-17-blue)
![License: MIT](https://img.shields.io/badge/License-MIT-green)

Part of a low-level C++ portfolio focused on performance and determinism,
alongside [`mcu-baremetal`](https://github.com/michaelnwanna/mcu-baremetal) and
[`microtensor`](https://github.com/michaelnwanna/microtensor).

## Why determinism

Floating-point results can differ across compilers, CPUs, and optimisation
levels — which breaks lockstep multiplayer, replays, and reproducible physics.
This core solves the two foundations:

- **`Fixed` (Q16.16)** — a fixed-point number backed by a single `int32`. All
  arithmetic is integer, so it's **bit-for-bit identical everywhere**. 64-bit
  intermediates keep multiply/divide exact.
- **`FixedTimestep`** — the canonical *"fix your timestep"* accumulator: convert
  variable, jittery frame times into a whole number of equal simulation steps,
  expose an interpolation `alpha()` for smooth rendering, and cap work per frame
  to avoid the *spiral of death*.

## Quick look

```cpp
#include "game_engine_core/fixed.hpp"
#include "game_engine_core/timestep.hpp"

ge::FixedTimestep clock(1.0f / 60.0f);   // 60 Hz simulation
const ge::Fixed dt = ge::Fixed::fromFloat(clock.step());

// Per rendered frame:
int steps = clock.advance(frameSeconds); // however many fixed steps are due
for (int i = 0; i < steps; ++i) simulate(dt);
render(clock.alpha());                    // interpolate between sim states
```

See [`examples/bouncing_ball.cpp`](examples/bouncing_ball.cpp) — a ball bouncing
under gravity, fully in fixed-point, driven by deliberately irregular frame
times.

## Build & test

```bash
cmake -S . -B build -G Ninja
cmake --build build
ctest --test-dir build --output-on-failure
./build/bouncing_ball
```

Requires CMake ≥ 3.20 and a C++17 compiler. Tests are dependency-free.

## Roadmap

- A cache-friendly **entity/component (ECS)** core with archetype storage.
- 2D/3D **fixed-point vector & matrix** math built on `Fixed`.
- A deterministic **event/command buffer** for lockstep networking and replays.
- A spatial hash / broadphase for collision.

## License

MIT — see [`LICENSE`](LICENSE).
