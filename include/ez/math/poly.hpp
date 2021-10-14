#pragma once
#include <ez/meta.hpp>
#include <cinttypes>
#include <array>
#include <cmath>
#include "constants.hpp"
#include "complex.hpp"

namespace ez::poly {

	template<typename T, typename Iter>
	int solveLinear(T a, T b, Iter output) {
		static_assert(is_real_vec_v<T>, "ez::Polynomial::solveLinear requires floating point types!");
		static_assert(is_output_iterator_v<Iter>, "ez::Polynomial::solveLinear requires the iterator passed in to be an output iterator.");
		static_assert(is_iterator_writable_v<Iter, T>, "ez::Polynomial::solveLinear cannot convert type to iterator value_type!");
		
		// Pull namespace in for name resolution
		
		if (std::abs(b) < ez::epsilon<T>()) {
			return 0;
		}
		else {
			(*output++) = -b / a;
			return 1;
		}
	};

	template<typename T, typename Iter>
	int solveQuadratic(T a, T b, T c, Iter output) {
		static_assert(is_real_vec_v<T>, "ez::Polynomial::solveQuadratic requires floating point types!");
		static_assert(is_output_iterator_v<Iter>, "ez::Polynomial::solveQuadratic requires the iterator passed in to be an output iterator.");
		static_assert(is_iterator_writable_v<Iter, T>, "ez::Polynomial::solveQuadratic cannot convert type to iterator value_type!");

		if (std::abs(a) < ez::epsilon<T>()) {
			if (std::abs(b) < ez::epsilon<T>()) {
				return 0;
			}
			(*output++) = -c / b;
			return 1;
		}
		else if (std::abs(c) < ez::epsilon<T>()) {
			T tmp = -b / a;

			(*output++) = std::min(T(0), tmp);
			(*output++) = std::max(T(0), tmp);
			return 2;
		}

		T det = b * b - T(4) * a * c;
		if (det > ez::epsilon<T>()) {
			if (b < -ez::epsilon<T>())
			{
				det = (-b + std::sqrt(det)) / (T(2) * a);
				T tmp = c / (a * det);

				(*output++) = std::min(det, tmp);
				(*output++) = std::max(det, tmp);
				return 2;
			}
			else if (b > ez::epsilon<T>()) {
				det = (-b - std::sqrt(det)) / (T(2) * a);
				T tmp = c / (a * det);

				(*output++) = std::min(det, tmp);
				(*output++) = std::max(det, tmp);
				return 2;
			}
			else {
				det = sqrt(-c / a);
				(*output++) = -det;
				(*output++) = det;
				return 2;
			}
		}
		else if(det > -ez::epsilon<T>()) {
			(*output++) = -b / (T(2) * a);
			return 1;
		}
		return 0;
	};

	template<typename T, typename  Iter>
	int solveCubic(T a, T b, T c, T d, Iter output) {
		/// Aberth method

		static_assert(std::is_floating_point_v<T>, "ez::Polynomial::solveQuadratic requires floating point types!");
		static_assert(is_output_iterator_v<Iter>, "ez::Polynomial::solveQuadratic requires the iterator passed in to be an output iterator.");
		static_assert(is_iterator_writable_v<Iter, T>, "ez::Polynomial::solveQuadratic cannot convert type to iterator value_type!");
		
		// Equidistant points on unit circle
		static constexpr glm::tcomplex<T>
			guess0{ -0.5839603576017,  0.811782175678686 },
			guess1{ -0.4110438076762, -0.911615592325514 },
			guess2{  0.9950041652780,  0.099833416646828 };

		using namespace std;
		using namespace glm;

		if (std::abs(a) <= ez::epsilon<T>()) {
			return solveQuadratic(b, c, d, output);
		}

		T upperBound = T(1) + max(std::abs(b / a), max(std::abs(c / a), std::abs(d / a)));

		std::array<glm::tcomplex<T>, 4> coeff{
			glm::tcomplex<T>{a, T(0)},
			glm::tcomplex<T>{b, T(0)},
			glm::tcomplex<T>{c, T(0)},
			glm::tcomplex<T>{d, T(0)}
		};
		std::array<glm::tcomplex<T>, 3> deriv{
			T(3) * coeff[0], 
			T(2) * coeff[1], 
			coeff[2]
		};

		std::array<glm::tcomplex<T>, 3> z;
		z[0] = upperBound * guess0;
		z[1] = upperBound * guess1;
		z[2] = upperBound * guess2;

		std::array<glm::tcomplex<T>, 3> w = z;

		int icount = 32;
		for (int i = 0; i < icount; ++i) {
			for (int k = 0; k < 3; ++k) {

				glm::tcomplex<T> factor{ 
					eval(coeff[0], coeff[1], coeff[2], coeff[3], z[k]) / 
					eval(deriv[0], deriv[1], deriv[2], z[k]) 
				};

				glm::tcomplex<T> repulsion{ T(0), T(0) };

				for (int j = 0; j < 3; ++j) {
					if (j == k) {
						continue;
					}

					repulsion += T(1) / (z[k] - z[j]);
				}

				w[k] = factor / (glm::tcomplex<T>{T(1), T(0)} - factor * repulsion);
			}

			z = w;
		}

		int found = 0;
		for (int i = 0; i < 3; ++i) {
			if (abs(z[i].imag()) < ez::epsilon<T>()) {
				*output++ = z[i].real();
				++found;
			}
		}
		return found;
	}

	template<typename T>
	T eval(T a, T b, T t) {
		return a * t + b;
	};

	template<typename T>
	T eval(T a, T b, T c, T t) {
		return a * t * t + b * t + c;
	};

	template<typename T>
	T eval(T a, T b, T c, T d, T t) {
		T tt = t * t;
		return a * tt * t + b * tt + c * t + d;
	};

	template<typename T>
	T evalDerivative(T a, T t) {
		return a;
	}

	template<typename T>
	T evalDerivative(T a, T b, T t) {
		return T(2) * a * t + b;
	}

	template<typename T>
	T evalDerivative(T a, T b, T c, T t) {
		return T(3) * a * t * t + T(2) * b * t + c;
	}


	template<typename T>
	T evalIntegral(T a, T b, T t) {
		return a * t * t / T(2) + b * t;
	}

	template<typename T>
	T evalIntegral(T a, T b, T c, T t) {
		return a * t * t * t / T(3) + b * t * t / T(2) + c * t;
	}

	template<typename T>
	T evalIntegral(T a, T b, T c, T d, T t) {
		T tt = t * t;

		return a * tt * tt / T(4) + b * tt * t / T(3) + c * tt / T(2) + d * t;
	}
};