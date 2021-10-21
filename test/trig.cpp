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
	
	REQUIRE(comp == Approx(0));

	REQUIRE(supl == Approx(ez::half_pi<float>()));

	angle = ez::pi<float>() * 3.5f;
	float norm = ez::trig::normalizeAngle(angle);
	float standard = ez::trig::standardPosition(angle);

	REQUIRE(norm == Approx(-ez::half_pi<float>()));
	REQUIRE(standard == Approx(ez::pi<float>() * 1.5f));
}

TEST_CASE("Law of cosines") {
	float a = 8.f;
	float b = 11.f;
	float theta = ez::radians(37.f);

	float expected = 6.666344591f;

	float c;
	REQUIRE(ez::trig::cosineRule(a, b, theta, c));

	REQUIRE(c == Approx(expected));
}

TEST_CASE("radians and degrees") {
	REQUIRE(ez::radians(180.f) == Approx(ez::pi<float>()));

	REQUIRE(ez::radians(360.f) == Approx(ez::tau<float>()));

	REQUIRE(ez::degrees(ez::pi<float>()) == Approx(180));

	REQUIRE(ez::degrees(ez::tau<float>()) == Approx(360));
}