#pragma once
#include <cinttypes>
#include <array>
#include <type_traits>
#include <cmath>
#include "MathConstants.hpp"

namespace ez {
	namespace Polynomial {
		template <typename Iter, typename T, typename = int>
		struct is_output_iterator : public std::false_type {};

		// This template matches container adaptors
		template <typename Iter, typename T>
		struct is_output_iterator <Iter, T, decltype((typename Iter::iterator_category*)0, (typename Iter::container_type*)0,  0)> {
			static constexpr bool value = 
				!std::is_same<typename Iter::iterator_category, std::input_iterator_tag>::value &&
				std::is_convertible<T, typename Iter::container_type::value_type>::value;
		};

		// This template matches normal iterators
		template <typename Iter, typename T>
		struct is_output_iterator <Iter, T, decltype((typename Iter::iterator_category*)0, (typename std::enable_if<!std::is_same<typename Iter::value_type, void>::value, int>::type)0)> {
			static constexpr bool value =
				!std::is_same<typename Iter::iterator_category, std::input_iterator_tag>::value &&
				std::is_convertible<T, typename Iter::value_type>::value;
		};

		// This template matches pointers
		template <typename Iter, typename T>
		struct is_output_iterator <Iter*, T, int> {
			static constexpr bool value = !std::is_const<T>::value &&
				std::is_convertible<T, Iter>::value;
		};

		template<typename T>
		int solveLinear(T a, T b, T& root) {
			if (std::abs(b) < ez::epsilon<T>()) {
				return 0;
			}
			else {
				root = -b / a;
				return 1;
			}
		};

		template<typename T, typename Iter>
		int solveQuadratic(T a, T b, T c, Iter output) {
			static_assert(is_output_iterator<Iter, T>::value, "ez::Polynomial::solveQuadratic requires the iterator passed in to be an output iterator with proper value_type.");

			if (std::abs(a) < ez::epsilon<T>()) {
				if (std::abs(b) < ez::epsilon<T>()) {
					return 0;
				}
				(*output) = -c / b;
				++output;
				return 1;
			}
			else if (std::abs(c) < ez::epsilon<T>()) {
				T tmp = -b / a;

				(*output) = std::min(T(0), tmp);
				++output;
				(*output) = std::max(T(0), tmp);
				++output;
				return 2;
			}

			T det = b * b - static_cast<T>(4)* a* c;
			if (det > ez::epsilon<T>()) {
				if (b < -ez::epsilon<T>())
				{
					det = (-b + std::sqrt(det)) / (static_cast<T>(2)* a);
					T tmp = c / (a * det);

					(*output) = std::min(det, tmp);
					++output;
					(*output) = std::max(det, tmp);
					++output;
					return 2;
				}
				else if (b > ez::epsilon<T>()) {
					det = (-b - std::sqrt(det)) / (static_cast<T>(2)* a);
					T tmp = c / (a * det);

					(*output) = std::min(det, tmp);
					++output;
					(*output) = std::max(det, tmp);
					++output;
					return 2;
				}
				else {
					det = std::sqrt(-c / a);
					(*output) = -det;
					++output;
					(*output) = det;
					++output;
					return 2;
				}
			}
			else if(det > -ez::epsilon<T>()) {
				(*output) = -b / (static_cast<T>(2) * a);
				++output;
				return 1;
			}
			return 0;
		};

		// untested.
		//std::uint32_t solveCubic(float a, float b, float c, float d, float* arr);

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
	}

};