cmake_minimum_required(VERSION 3.21)

project(EZ_MATH
	VERSION 0.4.0
	LANGUAGES C CXX
)

include(FetchContent)
include(GNUInstallDirs)

FetchContent_Declare(
	ez-cmake
	GIT_REPOSITORY "https://github.com/errata-c/ez-cmake.git"
	GIT_TAG "main"
)
FetchContent_MakeAvailable(ez-cmake)

set(EZ_MATH_CONFIG_DIR "share/ez-math" CACHE STRING "The relative directory to install package config files.")


find_package(glm CONFIG REQUIRED)
find_package(ez-meta CONFIG REQUIRED)

add_library(ez-math INTERFACE)

target_include_directories(ez-math INTERFACE
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
	"$<INSTALL_INTERFACE:include>"
)

target_compile_features(ez-math INTERFACE cxx_std_17)
target_compile_definitions(ez-math INTERFACE "$<$<PLATFORM_ID:Windows>:NOMINMAX>")
target_compile_options(ez-math INTERFACE "$<BUILD_INTERFACE:$<$<CXX_COMPILER_ID:MSVC>:/permissive->>")
target_link_libraries(ez-math INTERFACE glm::glm ez::meta)
set_target_properties(ez-math PROPERTIES EXPORT_NAME "math")

# Just for internal compatibility, so that subprojects can use the namespaced version
add_library(ez::math ALIAS ez-math)

if(PROJECT_IS_TOP_LEVEL)
	include(CTest)
	if(BUILD_TESTING)
		add_subdirectory("test")
	endif()

	install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/"
		TYPE INCLUDE
		FILES_MATCHING
		PATTERN "*.h" PATTERN "*.hpp"
	)

	install(TARGETS ez-math
		EXPORT "ez-math-export"
	)

	install_package(
		NAME "ez-math"
		NAMESPACE "ez::"
		DESTINATION "${EZ_MATH_CONFIG_DIR}"
		EXPORT "ez-math-export"
		VERSION "${EZ_MATH_VERSION}"
		COMPATIBILITY "SameMajorVersion"
		PRECONFIG "cmake/preconfig.cmake"
		ARCH_INDEPENDENT
	)
endif()