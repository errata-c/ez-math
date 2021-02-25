#pragma once
#include <ez/meta.hpp>
#include "Constants.hpp"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/geometric.hpp>
#include <cmath>

namespace ez::trig {
	namespace intern {
		template<typename T, typename U>
		glm::vec<3, T> barycentric(const T &p, const T& a, const T& b, const T& c) {
			T v0 = b - a, v1 = c - a, v2 = p - a;
			T d00 = glm::dot(v0, v0);
			T d01 = glm::dot(v0, v1);
			T d11 = glm::dot(v1, v1);
			T d20 = glm::dot(v2, v0);
			T d21 = glm::dot(v2, v1);
			T denom = d00 * d11 - d01 * d01;

			glm::vec<3, T> result;
			result.y = (d11 * d20 - d01 * d21) / denom;
			result.z = (d00 * d21 - d01 * d20) / denom;
			result.x = 1.0f - result.y - result.z;

			return result;
		};

		template<typename T>
		T supplement(const T & value) {
			return ez::pi<T>() - value;
		}

		template<typename T>
		T complement(const T& value) {
			return ez::half_pi<T>() - value;
		}
	};

	template<typename T>
	T supplement(const T& value) {
		static_assert(std::is_floating_point_v<T>, "ez::supplement requires floating point types!");
		return intern::supplement(value);
	};

	template<typename T, ::glm::length_t L>
	glm::vec<L, T> supplement(const glm::vec<L, T>& value) {
		static_assert(std::is_floating_point_v<T>, "ez::supplement requires floating point types!");
		return intern::supplement(value);
	};

	template<typename T>
	T complement(const T& value) {
		static_assert(std::is_floating_point_v<T>, "ez::complement requires floating point types!");
		return intern::complement(value);
	};

	template<typename T, glm::length_t L>
	glm::vec<L, T> complement(const glm::vec<L, T>& value) {
		static_assert(std::is_floating_point_v<T>, "ez::complement requires floating point types!");
		return intern::complement(value);
	};
	
	template<typename T>
	glm::vec<3, T> barycentric(T p, T a, T b, T c) {
		static_assert(std::is_floating_point_v<T>, "ez::barycentric requires floating point types!");
		return intern::barycentric<T, T>(p, a, b, c);
	};

	template<typename T, glm::length_t L>
	glm::vec<3, T> barycentric(const glm::vec<L, T>& p, const glm::vec<L, T>& a, const glm::vec<L, T>& b, const glm::vec<L, T>& c) {
		static_assert(std::is_floating_point_v<T>, "ez::barycentric requires floating point types!");
		return intern::barycentric<glm::vec<L, T>, T>(p, a, b, c);
	};

	// Puts the angle into standard position, ie in the range [0, 2 * pi]
	template<typename T>
	T standardPosition(const T& value) {
		static_assert(std::is_floating_point_v<T>, "ez::standardPosition requires floating point types!");

		T angle = std::fmod(value, ez::tau<T>());
		if (angle < ez::zero<T>()) {
			angle += ez::tau<T>();
		}
		return angle;
	};
	
	// Puts the angle into standard position, ie in the range [0, 2 * pi]
	template<typename T, glm::length_t L>
	glm::vec<L, T> standardPosition(const glm::vec<L, T>& value) {
		static_assert(std::is_floating_point_v<T>, "ez::standardPosition requires floating point types!");

		glm::vec<L, T> res;
		for (int i = 0; i < L; ++i) {
			res[i] = standardPosition<T>(value[i]);
		}
		return res;
	};

	// Normalizes the angle to the range [-pi/2, pi/2] 
	template<typename T>
	T normalizeAngle(const T& value) {
		static_assert(std::is_floating_point_v<T>, "ez::normalizeAngle requires floating point types!");

		T angle = std::fmod(value + ez::pi<T>(), ez::tau<T>());
		if (angle < ez::zero<T>()) {
			angle += ez::tau<T>();
		}
		return angle - ez::pi<T>();
	};

	// Normalizes the angle to the range [-pi/2, pi/2]
	template<typename T, glm::length_t L>
	glm::vec<L, T> normalizeAngle(const glm::vec<L, T>& value) {
		static_assert(std::is_floating_point_v<T>, "ez::normalizeAngle requires floating point types!");

		glm::vec<L, T> res;
		for (int i = 0; i < L; ++i) {
			res[i] = normalizeAngle<T>(value[i]);
		}
		return res;
	};
};