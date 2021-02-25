#include <limits>
#include <fmt/printf.h>
#include <iostream>
#include <ctime>

#include <ez/math/complex.hpp>
#include <ez/math/poly.hpp>
#include <ez/math/constants.hpp>
#include <ez/math/trig.hpp>
#include <ez/math/prng.hpp>

#include <ez/math/Trig.hpp>

int trig_test() {
	float angle = 1.f;
	float comp = ez::trig::complement(angle);
	float supl = ez::trig::supplement(angle);

	fmt::print("Angle == {}, complement == {}, supplement == {}\n", angle, comp, supl);

	angle = ez::pi<float>() * 3.5f;
	float norm = ez::trig::normalizeAngle(angle);
	float standard = ez::trig::standardPosition(angle);

	if (std::abs(norm + ez::half_pi<float>()) > 1e-5f) {
		fmt::print("Incorrect normalized angle result\n");
		return -1;
	}

	if (std::abs(standard - ez::pi<float>() * 1.5f) > 1e-5f) {
		fmt::print("Incorrect standard position angle\n");
		return -1;
	}

	return 0;
}