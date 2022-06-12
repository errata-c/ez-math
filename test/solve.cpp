#include <catch2/catch_all.hpp>

#include <limits>
#include <fmt/core.h>
#include <iostream>
#include <ctime>

#include <ez/math/complex.hpp>
#include <ez/math/poly.hpp>
#include <ez/math/constants.hpp>

using Approx = Catch::Approx;

template<typename T, typename output_iter>
void cubicFromRoots(T r0, T r1, T r2, output_iter output) {
	T a = T(1);
	T b = -(r0 + r1 + r2);
	T c = r0*r1 + r0*r2 + r1*r2;
	T d = -r0*r1*r2;

	// It seems that even with some rounds of gradient descent,
	// Some polynomials just cannot be built with the desired precision.
	// For instance, the polynomial with roots -4, 0, 0 ends up as 
	// -4, -0.000000091900, -0.000000091900
	// About 7-8 decimal places when using double precision

	int iters;
	T eps;
	if constexpr (sizeof(T) == 4) {
		eps = T(1E-3);
		iters = 12;
	}
	else {
		eps = T(1E-5);
		iters = 32;
	}

	T derror;
	T da(0), db(0), dc(0), dd(0);
	for (int i = 0; i < 64; ++i) {
		derror = -ez::poly::evaluate(a, b, c, d, r0);
		da += derror * r0 * r0 * r0;
		db += derror * r0 * r0;
		dc += derror * r0;
		dd += derror;

		derror = -ez::poly::evaluate(a, b, c, d, r1);
		da += derror * r0 * r0 * r0;
		db += derror * r0 * r0;
		dc += derror * r0;
		dd += derror;

		derror = -ez::poly::evaluate(a, b, c, d, r2);
		da += derror * r0 * r0 * r0;
		db += derror * r0 * r0;
		dc += derror * r0;
		dd += derror;

		a -= da * eps;
		b -= db * eps;
		c -= dc * eps;
		d -= dd * eps;
		eps *= T(0.5);
	}

	*output++ = a;
	*output++ = b;
	*output++ = c;
	*output++ = d;
}

template<typename T>
void printRoots(const T * roots, int count) {
	for (int i = 0; i < count; ++i) {
		fmt::print("roots[{}] == {:.12f}\n", i, roots[i]);
	}
}


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
	double co[4];
	cubicFromRoots(-4.0, 0.0, 0.0, &co[0]);

	std::array<double, 3> roots;
	int count = ez::poly::solveCubic(co[0], co[1], co[2], co[3], roots.begin());
	sort(roots, count);

	int firstCount = count;
	std::array<double, 3> firstRoots = roots;

	printRoots(roots.data(), count);

	// Check the precision
	for (int i = 0; i < count; ++i) {
		REQUIRE(std::abs(ez::poly::evaluate(co[0], co[1], co[2], co[3], roots[i])) < 1E-12);
	}

	for (double & val: co) {
		val *= 5.0;
	}

	count = ez::poly::solveCubic(co[0], co[1], co[2], co[3], roots.begin());
	sort(roots, count);

	REQUIRE(firstCount == count);
	for (int i = 0; i < count; ++i) {
		REQUIRE(std::abs(ez::poly::evaluate(co[0], co[1], co[2], co[3], roots[i])) < 1E-12);
	}
}

TEST_CASE("Cubic test 2") {
	double co[4];
	cubicFromRoots(-1.863675860276, -0.01336237902, +2.677038239296, &co[0]);

	std::array<double, 3> roots;
	int count = ez::poly::solveCubic(co[0], co[1], co[2], co[3], roots.begin());

	sort(roots, count);

	REQUIRE(count == 3);

	REQUIRE(std::abs(ez::poly::evaluate(co[0], co[1], co[2], co[3], roots[0])) < 1E-12);
	REQUIRE(std::abs(ez::poly::evaluate(co[0], co[1], co[2], co[3], roots[1])) < 1E-12);
	REQUIRE(std::abs(ez::poly::evaluate(co[0], co[1], co[2], co[3], roots[2])) < 1E-12);
}

TEST_CASE("Cubic test 3") {
	double co[4];
	cubicFromRoots(1.0, 1.0, 1.0, &co[0]);

	std::array<double, 3> roots;
	int count = ez::poly::solveCubic(co[0], co[1], co[2], co[3], roots.begin());

	sort(roots, count);

	REQUIRE(count == 1);

	REQUIRE(std::abs(ez::poly::evaluate(co[0], co[1], co[2], co[3], roots[0])) < 1E-12);
}