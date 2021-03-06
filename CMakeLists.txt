cmake_minimum_required(VERSION 3.14)

project(EZ_MATH
VERSION 0.4.0
LANGUAGES C CXX)

include(GNUInstallDirs)
include("cmake/ez-cmake/install_package.cmake")

option(BUILD_TESTS "Build the test executables" ON)
option(SEPARATE_DIRECTORY "Install the package into its own directory" ON)
set(CONFIG_DIR "${CMAKE_INSTALL_DATAROOTDIR}/ez-math" CACHE STRING "The relative directory to install package config files.")



find_package(glm CONFIG REQUIRED)
find_package(ez-meta CONFIG REQUIRED)

add_library(ez-math INTERFACE)

target_include_directories(ez-math INTERFACE
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
	"$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

target_compile_features(ez-math INTERFACE cxx_std_17)
target_compile_definitions(ez-math INTERFACE "$<$<PLATFORM_ID:Windows>:NOMINMAX>")
target_compile_options(ez-math INTERFACE "$<BUILD_INTERFACE:$<$<CXX_COMPILER_ID:MSVC>:/permissive->>")
target_link_libraries(ez-math INTERFACE glm::glm ez::meta)
set_target_properties(ez-math PROPERTIES EXPORT_NAME "math")

# Just for internal compatibility, so that subprojects can use the namespaced version
add_library(ez::math ALIAS ez-math)

if(BUILD_TESTS)
	add_subdirectory("test")
endif()

if(SEPARATE_DIRECTORY)
	set(CMAKE_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}/ez-math)
endif()

install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/"
	TYPE INCLUDE
	FILES_MATCHING
	PATTERN "*.h" PATTERN "*.hpp"
)

install(TARGETS ez-math
	EXPORT "ez-math-export"
	RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}/$<CONFIG>"
	ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}/$<CONFIG>"
	LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}/$<CONFIG>"
)

install_package(
	NAME "ez-math"
	NAMESPACE "ez::"
	DESTINATION "${CONFIG_DIR}"
	EXPORT "ez-math-export"
	VERSION "${EZ_MATH_VERSION}"
	COMPATIBILITY "SameMajorVersion"
	PRECONFIG "cmake/preconfig.cmake"
	ARCH_INDEPENDENT
)