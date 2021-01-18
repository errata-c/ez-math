
add_executable(basic_test "basic.cpp" "complex.cpp" "hash.cpp" "solve.cpp" "rng.cpp" "trig.cpp")
target_link_libraries(basic_test PRIVATE test_framework)