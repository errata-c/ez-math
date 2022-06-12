find_package(fmt CONFIG REQUIRED)
find_package(Catch2 CONFIG REQUIRED)


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