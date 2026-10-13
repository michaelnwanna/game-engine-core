# SPDX-License-Identifier: MIT
cmake_minimum_required(VERSION 3.20)

project(game-engine-core
  VERSION 0.1.0
  DESCRIPTION "Deterministic real-time core: fixed-timestep loop + fixed-point math"
  LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "Build type" FORCE)
endif()

option(GE_BUILD_TESTING "Build tests" ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(CompilerWarnings)

# Header-only core library.
add_library(game_engine_core INTERFACE)
add_library(ge::core ALIAS game_engine_core)
target_include_directories(game_engine_core INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
target_compile_features(game_engine_core INTERFACE cxx_std_17)

# Example: a deterministic bouncing-ball simulation.
add_executable(bouncing_ball examples/bouncing_ball.cpp)
target_link_libraries(bouncing_ball PRIVATE ge::core)
ge_set_warnings(bouncing_ball)

if(GE_BUILD_TESTING)
  enable_testing()
  add_subdirectory(tests)
endif()
