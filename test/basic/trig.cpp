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

int trig_test() {
	float angle = 1.f;
	float comp = ez::complement(angle);
	float supl = ez::supplement(angle);

	fmt::print("Angle == {}, complement == {}, supplement == {}\n", angle, comp, supl);

	angle = ez::pi<float>() * 3.5f;
	float norm = ez::normalizeAngle(angle);
	float standard = ez::standardPosition(angle);

	if (std::abs(norm + ez::pi2<float>()) > 1e-5f) {
		fmt::print("Incorrect normalized angle result\n");
		return -1;
	}

	if (std::abs(standard - ez::pi<float>() * 1.5f) > 1e-5f) {
		fmt::print("Incorrect standard position angle\n");
		return -1;
	}

	return 0;
}