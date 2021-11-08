#include <catch2/catch.hpp>

#include <limits>
#include <fmt/core.h>
#include <iostream>
#include <ctime>

#include <ez/math/complex.hpp>
#include <ez/math/poly.hpp>
#include <ez/math/constants.hpp>

bool approxEq(double a, double b) {
	constexpr double eps = 1E-12;
	return std::abs(a - b) < eps;
}
bool approxEq(float a, float b) {
	constexpr float eps = 1E-4f;
	return std::abs(a - b) < eps;
}

template<typename Array>
void sort(Array & arr, int len) {
	for (int i = 0; i < len -1; ++i) {
		int selMin = i;
		for (int j = i+1; j < len; ++j) {
			if (arr[j] < arr[selMin]) {
				selMin = j;
			}
		}
		if (selMin != i) {
			std::swap(arr[i], arr[selMin]);
		}
	}
}

TEST_CASE("simple quadratic test") {
	float a = 1, b = 0, c = -1;
	float roots[2];
	int count = ez::poly::solveQuadratic(a, b, c, roots);

	REQUIRE(count == 2);
	REQUIRE(approxEq(roots[0], -1.f));
	REQUIRE(approxEq(roots[1], +1.f));
}

TEST_CASE("large value quadratic test") {
	double
		a = 5112,
		b = -5112,
		c = 1278;

	std::array<double, 2> roots;
	int count = ez::poly::solveQuadratic(a, b, c, roots.begin());

	REQUIRE(count == 1);
	REQUIRE(approxEq(roots[0], 0.5));
}

TEST_CASE("second large quadratic test") {
	double
		a = 0.0,
		b = 579.6,
		c = -289.8;

	std::array<double, 2> roots;
	int count = ez::poly::solveQuadratic(a, b, c, roots.begin());

	REQUIRE(count == 1);
	REQUIRE(approxEq(roots[0], 0.5));
}



TEST_CASE("Cubic test 1") {
	double
		a = 0.6, 
		b = 2.4,
		c = 0.0,
		d = 0.0;

	std::array<double, 3> roots;
	int count = ez::poly::solveCubic(a, b, c, d, roots.begin());

	sort(roots, count);

	REQUIRE(count == 3);

	REQUIRE(approxEq(roots[0], -4.0));
	REQUIRE(approxEq(roots[1], 0.0));
	REQUIRE(approxEq(roots[2], 0.0));
}

TEST_CASE("Cubic test 2") {
	double
		a = -3.0,
		b = +2.4,
		c = +15.0,
		d = +0.2;

	using T = double;

	std::array<double, 3> roots;
	int count = ez::poly::solveCubic(a, b, c, d, roots.begin());

	sort(roots, count);

	REQUIRE(count == 3);

	REQUIRE(approxEq(roots[0], -1.863675860276));
	REQUIRE(approxEq(roots[1], -0.01336237902));
	REQUIRE(approxEq(roots[2], +2.677038239296));
}