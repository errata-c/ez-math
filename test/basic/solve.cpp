#include <limits>
#include <fmt/printf.h>
#include <iostream>
#include <ctime>

#include <ez/math/Complex.hpp>
#include <ez/math/Polynomial.hpp>
#include <ez/math/MathConstants.hpp>
#include <ez/math/Triangle.hpp>
#include <ez/math/PsuedoRandom.hpp>

#include <ez/math/Trig.hpp>

int solve_test() {
	float a = 1, b = 0, c = -1;
	float roots[2];
	int count = ez::Polynomial::solveQuadratic(a, b, c, roots);

	if (count != 2) {
		fmt::print("Incorrect number of solutions.\n");
		return -1;
	}

	if (std::abs(roots[0] + 1) > 1e-5f) {
		fmt::print("Incorrect solution value.\n");
		return -1;
	}

	if (std::abs(roots[1] - 1) > 1e-5f) {
		fmt::print("Incorrect solution value.\n");
		return -1;
	}

	return 0;
}