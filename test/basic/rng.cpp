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

int rng_test() {
	srand(static_cast<unsigned>(time(NULL)));
	std::uint32_t rnum = rand();

	ez::SplitMix64 smix{ rnum };

	ez::XorShift128 xmix{
		static_cast<std::uint32_t>(smix.advance()),
		static_cast<std::uint32_t>(smix.advance()),
		static_cast<std::uint32_t>(smix.advance()),
		static_cast<std::uint32_t>(smix.advance())
	};

	std::uint8_t val0 = 1;
	std::uint16_t val1 = 1;
	std::uint32_t val2 = 0xF;
	std::uint64_t val3 = 0xFF;

	if (ez::rotl8(val0, 1) != 2) {
		fmt::print("Incorrect left rotation\n");
		return -1;
	}

	if (ez::rotr8(val0, 1) != 128) {
		fmt::print("Incorrect right rotation\n");
		return -1;
	}

	if (ez::rotl32(val2, 8) != (0xFu << 8)) {
		fmt::print("Incorrect left rotation\n");
		return -1;
	}

	if (ez::rotr32(val2, 8) != (0xFu << 24)) {
		fmt::print("Incorrect right rotation\n");
		return -1;
	}

	return 0;
}