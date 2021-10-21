#pragma once
#include <cmath>
#include <complex>
#include <glm/vec2.hpp>

/*
	The goal of this header is to integrate the std::complex type into glm seamlessly.
	Specifically using all the conventions that glm::quat uses.
	This gives the best of both worlds, the standard library's complex type, and the glm ecosystem.
*/

namespace glm {
	template<typename T>
	using tcomplex = std::complex<T>;

	// long double is unsupported. I've never seen any code actually using it anyway.
	using complex = tcomplex<float>;
	using dcomplex = tcomplex<double>;

	template<typename T>
	T dot(const tcomplex<T>& lh, const tcomplex<T>& rh) noexcept {
		return lh.real() * rh.real() + lh.imag() * rh.imag();
	}
	template<typename T>
	T dot(const tcomplex<T>& lh, const glm::vec<2, T>& rh) noexcept {
		return lh.real() * rh.x + lh.imag() * rh.y;
	}
	template<typename T>
	T dot(const glm::vec<2, T>& lh, const tcomplex<T>& rh) noexcept {
		return dot(rh, lh);
	}

	template<typename T>
	T length(const tcomplex<T>& value) noexcept {
		return std::abs(value);
	}
	template<typename T>
	T length2(const tcomplex<T>& value) noexcept {
		return std::norm(value);
	}

	template<typename T>
	tcomplex<T> normalize(const tcomplex<T>& value) noexcept {
		return value / length(value);
	}

	template<typename T>
	glm::tvec2<T> vec2_cast(const tcomplex<T>& value) noexcept {
		return glm::tvec2<T>{value.real(), value.imag()};
	}

	template<typename T>
	glm::tmat2x2<T> mat2_cast(const tcomplex<T>& value) noexcept {
		return glm::tmat2x2<T>{
			glm::tvec2<T>{value.real(), value.imag()},
				glm::tvec2<T>{-value.imag(), value.real()}
		};
	}

	template<typename T>
	glm::tmat3x3<T> mat3_cast(const tcomplex<T>& value) noexcept {
		return glm::tmat3x3<T>{
			glm::tvec3<T>{value.real(), value.imag(), T(0)},
				glm::tvec3<T>{-value.imag(), value.real(), T(0)},
				glm::tvec3<T>{T(0), T(0), T(1)},
		};
	}

	template<typename T>
	glm::tmat4x4<T> mat4_cast(const tcomplex<T>& value) noexcept {
		return glm::tmat4x4<T>{
			glm::tvec4<T>{value.real(), value.imag(), T(0), T(0)},
				glm::tvec4<T>{-value.imag(), value.real(), T(0), T(0)},
				glm::tvec4<T>{T(0), T(0), T(1), T(0)},
				glm::tvec4<T>{T(0), T(0), T(0), T(1)},
		};
	}

	template<typename T>
	tcomplex<T> complex_cast(const glm::tvec2<T>& value) noexcept {
		return tcomplex<T>{value.x, value.y};
	}

	template<typename T>
	tcomplex<T> complex_cast(const glm::tmat2x2<T>& value) noexcept {
		return tcomplex<T>{value[0].x, value[0].y};
	}

	template<typename T>
	tcomplex<T> conjugate(const tcomplex<T>& value) noexcept {
		return std::conj(value);
	}

	template<typename T>
	tcomplex<T> inverse(const tcomplex<T> & value) noexcept {
		return conjugate(value) / std::norm(value);
	}

	template<typename T>
	T angle(const tcomplex<T>& value) noexcept {
		return std::arg(value);
	}

	// Certain headers in glm bring these into glm namespace, but I put these here anyways just in case
	using std::exp;
	using std::pow;
	using std::sqrt;
	using std::log;

	template<typename T>
	tcomplex<T> polar(const T& theta, const T& radius = T(1)) noexcept {
		return tcomplex<T>{ std::cos(theta) * radius, std::sin(theta) * radius };
	}

	template<typename T>
	glm::tvec2<T> rotate(const tcomplex<T>& value, const glm::tvec2<T>& v) noexcept {
		tcomplex<T> tmp{ v.x, v.y };
		return vec2_cast(value * tmp);
	}

	template<typename T>
	glm::tcomplex<T> rotation(const glm::tvec2<T>& from, const glm::tvec2<T>& to) noexcept {
		return glm::complex_cast(to) * glm::conjugate(glm::complex_cast(from));
	}
};