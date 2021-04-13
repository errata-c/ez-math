cmake_minimum_required(VERSION 3.14)

project(EZ_MATH
VERSION 0.3.4
LANGUAGES C CXX)

include(GNUInstallDirs)
find_package(ez-cmake CONFIG REQUIRED)

option(BUILD_TESTS "Build the test executables" ON)
option(SEPARATE_DIRECTORY "Install the package into its own directory" ON)

set(PACK_NAME "ez-math")
set(CONFIG_DIR "${CMAKE_INSTALL_DATAROOTDIR}/${PACK_NAME}" CACHE STRING "The relative directory to install ${PACK_NAME} package config files.")
set(BUILD_TESTS OFF CACHE BOOL "Build the tests for ${PACK_NAME} library.")

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
	EXPORT ez-math-targets
	RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}/$<CONFIG>"
	ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}/$<CONFIG>"
	LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}/$<CONFIG>"
)

install_package(
	NAME "ez-math"
	NAMESPACE "ez::"
	DESTINATION "${CONFIG_DIR}"
	EXPORT "ez-math-targets"
	VERSION "${EZ_MATH_VERSION}"
	COMPATIBILITY "SameMajorVersion"
	PRECONFIG "preconfig.cmake"
	ARCH_INDEPENDENT
)