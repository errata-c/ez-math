#include <limits>
#include <fmt/printf.h>
#include <iostream>
#include <ctime>

#include <ez/math/complex.hpp>
#include <ez/math/poly.hpp>
#include <ez/math/constants.hpp>
#include <ez/math/trig.hpp>
#include <ez/math/prng.hpp>

int trig_test();
int hash_test();
int rng_test();
int solve_test();

int main() {
	
	if (trig_test() != 0) {
		fmt::print("trig test failed\n");
		return -1;
	}
	if (hash_test() != 0) {
		fmt::print("hash test failed\n");
		return -1;
	}
	if (rng_test() != 0) {
		fmt::print("rng test failed\n");
		return -1;
	}
	if (solve_test() != 0) {
		fmt::print("solve test failed\n");
		return -1;
	}

	fmt::print("All tests passed");
	
	return 0;
}