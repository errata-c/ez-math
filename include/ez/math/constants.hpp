#pragma once
#include <cinttypes>
#include <type_traits>
#include <limits>
#include <glm/gtc/constants.hpp>

namespace ez {
	/// Use lower case for these constants, so as to not conflict with macros.

	// Returns the value of pi, cast to the template type.
	template<typename T = double>
	constexpr T pi() {
		return glm::pi<T>();
	}

	// Returns the value of 2*pi, cast to the template type.
	template<typename T = double>
	constexpr T tau() {
		return T(2) * glm::pi<T>();
	}

	// Returns the value of pi / 2, cast to the template type.
	template<typename T = double>
	constexpr T half_pi() {
		return glm::half_pi<T>();
	}

	// Returns the value of pi / 6, cast to the template type.
	template<typename T = double>
	constexpr T sixth_pi() {
		return pi<T>() / T(6);
	}

	// Returns the value of pi / 4, cast to the template type.
	template<typename T = double>
	constexpr T quarter_pi() {
		return glm::quarter_pi<T>();
	}

	// Returns the value of e (euler's constant), cast to the template type.
	template<typename T = double>
	constexpr T e() {
		return glm::e<T>();
	}

	// Returns the value of natural log of 2, cast to the template type.
	template<typename T = double>
	constexpr T ln_two() {
		return glm::ln_two<T>();
	}

	// Returns the value of square root of 2, cast to the template type.
	template<typename T>
	constexpr T root_two() {
		return glm::root_two<T>();
	}

	// Returns the value of cube root of 2, cast to the template type.
	template<typename T>
	constexpr T root_three() {
		return glm::root_three<T>();
	}

	template<typename T>
	constexpr T zero() {
		return T(0);
	}

	template<typename T>
	constexpr T one() {
		return T(1);
	}

	template<typename T>
	constexpr T root_pi() {
		return glm::root_pi<T>();
	}

	template<typename T, std::enable_if_t<std::is_integral_v<T>, int> = 0>
	constexpr T epsilon() {
		return static_cast<T>(1);
	}

	namespace intern {
		template<std::size_t N>
		struct eps {};

		template<>
		struct eps<4> { // 32 bits
			static constexpr float value = 1E-4f;
		};
		template<>
		struct eps<8> { // 64 bits
			static constexpr double value = 1E-14;
		};
		template<>
		struct eps<10> { // 80 bits
			static constexpr long double value = 1E-17l;
		};
		template<>
		struct eps<12> { // 96 bits
			static constexpr long double value = 1E-20l;
		};
	};

	template<typename T, std::enable_if_t<std::is_floating_point_v<T>, int> = 0>
	constexpr T epsilon() {
		return intern::eps<sizeof(T)>::value;
	}
}
