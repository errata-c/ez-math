
find_package(Catch2 CONFIG REQUIRED)

add_executable(basic_test "basic.cpp" "complex.cpp" "solve.cpp" "trig.cpp")
target_link_libraries(basic_test PRIVATE test_framework Catch2::Catch2)