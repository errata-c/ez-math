#include <catch.hpp>

#include <limits>
#include <fmt/printf.h>
#include <iostream>
#include <ctime>

#include <ez/math/complex.hpp>
#include <ez/math/poly.hpp>
#include <ez/math/constants.hpp>
#include <ez/math/trig.hpp>

TEST_CASE("angles test") {
	float angle = ez::half_pi<float>();
	float comp = ez::trig::complement(angle);
	float supl = ez::trig::supplement(angle);
	
	REQUIRE(std::abs(comp) < 1e-5f);

	REQUIRE(std::abs(supl - ez::half_pi<float>()) < 1e-5f);

	angle = ez::pi<float>() * 3.5f;
	float norm = ez::trig::normalizeAngle(angle);
	float standard = ez::trig::standardPosition(angle);

	REQUIRE(std::abs(norm + ez::half_pi<float>()) < 1e-5f);

	REQUIRE(std::abs(standard - ez::pi<float>() * 1.5f) < 1e-5f);
}

TEST_CASE("Law of cosines") {
	float a = 8.f;
	float b = 11.f;
	float theta = ez::toRadians(37.f);

	float expected = 6.666344591f;

	float c;
	REQUIRE(ez::trig::cosineRule(a, b, theta, c));

	REQUIRE(std::abs(expected - c) < 1e-5f);
}