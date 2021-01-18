cmake_minimum_required(VERSION 3.14)

add_executable(solvers_test "solvers.cpp")
target_link_libraries(solvers_test PRIVATE test_framework)