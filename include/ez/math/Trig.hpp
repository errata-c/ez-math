#pragma once
#include <type_traits>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <cmath>
#include "MathConstants.hpp"

namespace ez {
	namespace intern {
		template<typename T, typename U>
		void barycentric(const T &p, const T& a, const T& b, const T& c, U& u, U& v, U& w) {
			T v0 = b - a, v1 = c - a, v2 = p - a;
			T d00 = glm::dot(v0, v0);
			T d01 = glm::dot(v0, v1);
			T d11 = glm::dot(v1, v1);
			T d20 = glm::dot(v2, v0);
			T d21 = glm::dot(v2, v1);
			T denom = d00 * d11 - d01 * d01;
			v = (d11 * d20 - d01 * d21) / denom;
			w = (d00 * d21 - d01 * d20) / denom;
			u = 1.0f - v - w;
		};

		template<typename T>
		T supplement(const T & value) {
			return ez::pi<T>() - value;
		}

		template<typename T>
		T complement(const T& value) {
			return ez::pi2<T>() - value;
		}
	};

	template<typename T, std::enable_if_t<std::is_floating_point_v<T>, int> = 0>
	T supplement(const T& value) {
		return intern::supplement(value);
	};

	template<typename T, ::glm::length_t L, std::enable_if_t<std::is_floating_point_v<T>, int> = 0 >
	glm::vec<L, T> supplement(const glm::vec<L, T>& value) {
		return intern::supplement(value);
	};

	template<typename T, std::enable_if_t<std::is_floating_point_v<T>, int> = 0>
	T complement(const T& value) {
		return intern::complement(value);
	};

	template<typename T, glm::length_t L, std::enable_if_t<std::is_floating_point_v<T>, int> = 0>
	glm::vec<L, T> complement(const glm::vec<L, T>& value) {
		return intern::complement(value);
	};
	
	template<typename T, std::enable_if_t<std::is_floating_point_v<T>, int> = 0>
	void barycentric(T p, T a, T b, T c, T& u, T& v, T& w) {
		intern::barycentric<T, T>(p, a, b, c, u, v, w);
	};

	template<typename T, glm::length_t L, std::enable_if_t<std::is_floating_point_v<T>, int> = 0>
	void barycentric(const glm::vec<L, T>& p, const glm::vec<L, T>& a, const glm::vec<L, T>& b, const glm::vec<L, T>& c, T& u, T& v, T& w) {
		intern::barycentric<glm::vec<L, T>, T>(p, a, b, c, u, v, w);
	};

	// Puts the angle into standard position, ie in the range [0, 2 * pi]
	template<typename T, std::enable_if_t<std::is_floating_point_v<T>, int> = 0>
	T standardPosition(const T& value) {
		T angle = std::fmod(value, ez::tau<T>());
		if (angle < T(0)) {
			angle += ez::tau<T>();
		}
		return angle;
	};
	
	// Puts the angle into standard position, ie in the range [0, 2 * pi]
	template<typename T, glm::length_t L, std::enable_if_t<std::is_floating_point_v<T>, int> = 0>
	glm::vec<L, T> standardPosition(const glm::vec<L, T>& value) {
		glm::vec<L, T> res;
		for (int i = 0; i < L; ++i) {
			res[i] = standardPosition<T>(value[i]);
		}
		return res;
	};

	// Normalizes the angle to the range [-pi/2, pi/2] 
	template<typename T, std::enable_if_t<std::is_floating_point_v<T>, int> = 0>
	T normalizeAngle(const T& value) {
		T angle = std::fmod(value + ez::pi<T>(), ez::tau<T>());
		if (angle < T(0)) {
			angle += ez::tau<T>();
		}
		return angle - ez::pi<T>();
	};

	// Normalizes the angle to the range [-pi/2, pi/2]
	template<typename T, glm::length_t L, std::enable_if_t<std::is_floating_point_v<T>, int> = 0>
	glm::vec<L, T> normalizeAngle(const glm::vec<L, T>& value) {
		glm::vec<L, T> res;
		for (int i = 0; i < L; ++i) {
			res[i] = normalizeAngle<T>(value[i]);
		}
		return res;
	};
};