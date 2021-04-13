
find_package(fmt CONFIG REQUIRED)

add_library(test_framework INTERFACE)
target_link_libraries(test_framework INTERFACE ez::math fmt::fmt-header-only)

add_subdirectory("basic")