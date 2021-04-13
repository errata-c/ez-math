#include <catch.hpp>

#include <limits>
#include <fmt/printf.h>
#include <iostream>
#include <ctime>

#include <ez/math/complex.hpp>
#include <ez/math/poly.hpp>
#include <ez/math/constants.hpp>
#include <ez/math/trig.hpp>

TEST_CASE("simple quadratic test") {
	float a = 1, b = 0, c = -1;
	float roots[2];
	int count = ez::poly::solveQuadratic(a, b, c, roots);

	REQUIRE(count == 2);
	REQUIRE(std::abs(roots[0] + 1) < 1e-5f);
	REQUIRE(std::abs(roots[1] - 1) < 1e-5f);
}

TEST_CASE("large value quadratic test") {
	double
		a = 5112,
		b = -5112,
		c = 1278;

	std::array<double, 2> roots;
	int count = ez::poly::solveQuadratic(a, b, c, roots.begin());

	REQUIRE(count == 1);
	REQUIRE(std::abs(roots[0] - 0.5) < 1e-5f);
}

TEST_CASE("second large quadratic test") {
	double
		a = 0.0,
		b = 579.6,
		c = -289.8;

	std::array<double, 2> roots;
	int count = ez::poly::solveQuadratic(a, b, c, roots.begin());

	REQUIRE(count == 1);
	REQUIRE(std::abs(roots[0] - 0.5) < 1e-5f);
}