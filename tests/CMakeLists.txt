# SPDX-License-Identifier: MIT
add_executable(game_engine_core_tests game_engine_core_test.cpp)
target_link_libraries(game_engine_core_tests PRIVATE ge::core)
ge_set_warnings(game_engine_core_tests)

add_test(NAME game_engine_core_tests COMMAND game_engine_core_tests)
