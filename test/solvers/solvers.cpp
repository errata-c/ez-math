#include <limits>
#include <fmt/printf.h>
#include <iostream>
#include <ctime>
#include <array>

#include <ez/math/Polynomial.hpp>
#include <ez/math/MathConstants.hpp>
#include <ez/math/Triangle.hpp>
#include <ez/math/PsuedoRandom.hpp>

int main() {
	fmt::print("First, expected values: 0.5\n");
	{
		double
			a = 5112,
			b = -5112,
			c = 1278;

		std::array<double, 2> roots;
		int count = ez::Polynomial::solveQuadratic(a, b, c, roots.begin());

		for (int i = 0; i < count; ++i) {
			fmt::print("roots[{}] == {}\n", i, roots[i]);
		}
	}

	fmt::print("Second, expected values: 0.5\n");
	{
		double
			a = 0.0,
			b = 579.6,
			c = -289.8;

		std::array<double, 2> roots;
		int count = ez::Polynomial::solveQuadratic(a, b, c, roots.begin());

		for (int i = 0; i < count; ++i) {
			fmt::print("roots[{}] == {}\n", i, roots[i]);
		}
	}

	fmt::print("Third, expected values: 1, -1\n");
	{
		double 
			a = 1.0, 
			b = 0.0, 
			c = -1.0;
		std::array<double, 2> roots;
		int count = ez::Polynomial::solveQuadratic(a, b, c, roots.begin());

		for (int i = 0; i < count; ++i) {
			fmt::print("roots[{}] == {}\n", i, roots[i]);
		}
	}
	return 0;
};