#pragma once
#include <ez/meta.hpp>
#include <cinttypes>
#include <array>
#include <cmath>
#include "constants.hpp"
#include "complex.hpp"

namespace ez::poly {
	// Linear polynomial
	template<typename T, typename U>
	U evaluate(T a, T b, U t) {
		return a * t + b;
	};

	// Quadratic polynomial
	template<typename T, typename U>
	U evaluate(T a, T b, T c, U t) {
		return a * t * t + b * t + c;
	};

	// Cubic polynomial
	template<typename T, typename U>
	U evaluate(T a, T b, T c, T d, U t) {
		U tt = t * t;
		return a * tt * t + b * tt + c * t + d;
	};

	// Linear polynomial
	template<typename T, typename U>
	U derivativeAt(T a, U t) {
		return a;
	}

	// Quadratic polynomial
	template<typename T, typename U>
	U derivativeAt(T a, T b, U t) {
		return T(2) * a * t + b;
	}

	// Cubic polynomial
	template<typename T, typename U>
	U derivativeAt(T a, T b, T c, U t) {
		return T(3) * a * t * t + T(2) * b * t + c;
	}


	template<typename T, typename U>
	T evalIntegral(T a, T b, U t) {
		return a * t * t / T(2) + b * t;
	}

	template<typename T, typename U>
	T evalIntegral(T a, T b, T c, U t) {
		return a * t * t * t / T(3) + b * t * t / T(2) + c * t;
	}

	template<typename T, typename U>
	T evalIntegral(T a, T b, T c, T d, U t) {
		U tt = t * t;

		return a * tt * tt / T(4) + b * tt * t / T(3) + c * tt / T(2) + d * t;
	}

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
		
		// Roughly equidistant points on unit circle
		// Slightly non-symmetrical to prevent convergence issues
		static constexpr glm::tcomplex<T>
			guess0{ -0.5839603576017,  0.811782175678686 },
			guess1{ -0.4110438076762, -0.911615592325514 },
			guess2{  0.9950041652780,  0.099833416646828 };

		if (std::abs(a) < ez::epsilon<T>()) {
			return solveQuadratic(b, c, d, output);
		}

		// The bounds of the polynomials roots
		T upper = T(1) + (T(1) / (std::abs(a)) * std::max(std::abs(d), std::max(std::abs(c), std::abs(b))));
		T lower = std::abs(d) / (std::abs(d) + std::max(std::abs(c), std::max(std::abs(b), std::abs(a))));
		T mid = (upper + lower) * T(0.5);

		// The roots we are iterating on
		std::array<glm::tcomplex<T>, 3> roots{
			mid * guess0,
			mid * guess1,
			mid * guess2
		};

		std::array<T, 3> deriv{
			T(3) * a, 
			T(2) * b, 
			c
		};

		// number of iterations to perform
		int icount = 48;
		constexpr T eps = ez::epsilon<T>() * T(10);

		for (int i = 0; i < icount; ++i) {
			int valid = 0;
			for (int k = 0; k < 3; ++k) {
				glm::tcomplex<T> root = roots[k];

				glm::tcomplex<T> factor =  
					poly::evaluate(a, b, c, d, root) /
					poly::evaluate(deriv[0], deriv[1], deriv[2], root);
				
				glm::tcomplex<T> repulsion{ T(0), T(0) };
				for (int j = 0; j < 3; ++j) {
					if (j != k) {
						repulsion += T(1) / (root - roots[j]);
					}
				}

				glm::tcomplex<T> offset = factor / (T(1) - factor * repulsion);
				roots[k] -= offset;

				if (std::abs(offset.real()) < eps && std::abs(offset.imag()) < eps) {
					++valid;
				}
			}

			if (valid == 3) {
				break;
			}
		}

		int found = 0;
		for (int i = 0; i < 3; ++i) {
			if (std::abs(roots[i].imag()) < eps) {
				*output++ = roots[i].real();
				++found;
			}
		}
		return found;
	}

	
};