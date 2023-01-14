

FetchContent_Declare(
	Catch2
	GIT_REPOSITORY "https://github.com/catchorg/Catch2.git"
	GIT_TAG "v3.2.1"
	FIND_PACKAGE_ARGS CONFIG REQUIRED
)
FetchContent_Declare(
	fmt
	GIT_REPOSITORY "https://github.com/fmtlib/fmt.git"
	GIT_TAG "9.1.0"
	FIND_PACKAGE_ARGS CONFIG REQUIRED
)

FetchContent_MakeAvailable(Catch2 fmt)


add_executable(ez_math_tests 
	"complex.cpp" 
	"solve.cpp" 
	"trig.cpp"
	"color.cpp"
)
target_link_libraries(ez_math_tests PRIVATE 
	ez::math 
	fmt::fmt
	Catch2::Catch2WithMain
)