#pragma once
#include <cinttypes>
#include <type_traits>
#include <limits>

namespace ez {
	/// Use lower case for these constants, so as to not conflict with macros.

	// Returns the value of pi, cast to the template type.
	template<typename T = double>
	constexpr T pi() {
		return static_cast<T>(3.14159265358979323846264338327950288419716939937510582097);
	}

	// Returns the value of 2*pi, cast to the template type.
	template<typename T = double>
	constexpr T tau() {
		return static_cast<T>(2.0 * pi());
	}

	// Returns the value of pi / 2, cast to the template type.
	template<typename T = double>
	constexpr T pi2() {
		return static_cast<T>(pi() / 2.0);
	}

	// Returns the value of pi / 4, cast to the template type.
	template<typename T = double>
	constexpr T pi4() {
		return static_cast<T>(pi() / 4.0);
	}

	// Returns the value of e (euler's constant), cast to the template type.
	template<typename T = double>
	constexpr T e() {
		return static_cast<T>(2.71828182845904523536028747135266249775725);
	}

	// Returns the value of log base 2 of e, cast to the template type.
	template<typename T = double>
	constexpr T log2e() {
		return static_cast<T>(1.44269504088896340735992468100189213742665);
	}

	// Returns the value of natural log of 2, cast to the template type.
	template<typename T = double>
	constexpr T ln2() {
		return static_cast<T>(0.6931471805599453094172321214581765680755);
	}

	// Returns the value of square root of 2, cast to the template type.
	template<typename T>
	constexpr T root2() {
		return static_cast<T>(1.41421356237309504880168872420969807856967);
	}

	// Returns the value of cube root of 2, cast to the template type.
	template<typename T>
	constexpr T root3() {
		return static_cast<T>(1.73205080756887729352744634150587236694281);
	}

	template<typename T, std::enable_if_t<std::is_integral_v<T>, int> = 0>
	constexpr T epsilon() {
		return static_cast<T>(1);
	}

	namespace intern {
		template<typename T>
		struct eps {};

		template<>
		struct eps<float> {
			static constexpr float value = 1E-5f;
		};
		template<>
		struct eps<double> {
			static constexpr double value = 1E-14;
		};
		template<>
		struct eps<long double> {
			static constexpr long double value = ((sizeof(long double) == 8) || (sizeof(long double) == 10)) ? (1E-14l) :
				((sizeof(long double) == 12) ? 1E-17l : 1E-31l);
		};
	};

	template<typename T>
	constexpr T epsilon() {
		return intern::eps<T>::value;
	}
}
