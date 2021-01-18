cmake_minimum_required(VERSION 3.14)

project(EZ_MATH
VERSION 0.3.4
LANGUAGES C CXX)

include(GNUInstallDirs)
find_package(ez-cmake CONFIG REQUIRED)

set(PACK_NAME "ez-math")
set(CONFIG_DIR "${CMAKE_INSTALL_DATAROOTDIR}/${PACK_NAME}" CACHE STRING "The relative directory to install ${PACK_NAME} package config files.")
set(BUILD_TESTS OFF CACHE BOOL "Build the tests for ${PACK_NAME} library.")

find_package(glm CONFIG REQUIRED)

add_library(ez-math INTERFACE)

target_include_directories(ez-math 
	INTERFACE
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
	"$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

target_compile_features(ez-math INTERFACE cxx_std_17)
target_compile_definitions(ez-math INTERFACE $<$<PLATFORM_ID:Windows>:NOMINMAX>)
target_compile_options(ez-math INTERFACE "$<BUILD_INTERFACE:$<$<CXX_COMPILER_ID:MSVC>:/permissive->>")
target_link_libraries(ez-math INTERFACE glm)

if(${BUILD_TESTS})
	add_subdirectory("test")
endif()

set(CMAKE_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}/ez-math)

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
	DESTINATION "${CONFIG_DIR}"
	EXPORT "ez-math-targets"
	VERSION "${EZ_MATH_VERSION}"
	COMPATIBILITY "SameMajorVersion"
	PRECONFIG "preconfig.cmake"
	ARCH_INDEPENDENT
)