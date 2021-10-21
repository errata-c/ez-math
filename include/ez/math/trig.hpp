#pragma once
#include <ez/meta.hpp>
#include <ez/math/constants.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/geometric.hpp>
#include <cmath>

namespace ez {
	template<typename T>
	constexpr T radians(T angle) noexcept {
		static_assert(std::is_floating_point_v<T>, "ez::radians only accepts floating point types as input!");
		constexpr T factor = ez::tau<T>() / T(360);
		return angle * factor;
	}

	template<typename T>
	constexpr T degrees(T angle) noexcept {
		static_assert(std::is_floating_point_v<T>, "ez::degrees only accepts floating point types as input!");
		constexpr T factor = T(360) / ez::tau<T>();
		return angle * factor;
	}
}

namespace ez::trig {
	namespace intern {
		template<typename T>
		T supplement(const T & value) noexcept {
			return ez::pi<T>() - value;
		}

		template<typename T>
		T complement(const T& value) noexcept {
			return ez::half_pi<T>() - value;
		}
	};

	template<typename T>
	T supplement(const T& value) noexcept {
		static_assert(std::is_floating_point_v<T>, "ez::trig::supplement only accepts floating point types as input!");
		return intern::supplement(value);
	};

	template<typename T, ::glm::length_t L>
	glm::vec<L, T> supplement(const glm::vec<L, T>& value) noexcept {
		static_assert(std::is_floating_point_v<T>, "ez::trig::supplement only accepts floating point types as input!");
		return intern::supplement(value);
	};

	template<typename T>
	bool cosineRule(T sideA, T sideB, T theta, T& sideC) noexcept {
		static_assert(std::is_floating_point_v<T>, "ez::trig::cosineRule only accepts floating point types as input!");

		T A2 = sideA * sideA;
		T B2 = sideB * sideB;

		T determinant = A2 + B2 - T(2) * sideA * sideB * std::cos(theta);
		if (determinant >= T(0)) {
			sideC = std::sqrt(determinant);
			return true;
		}
		else {
			sideC = T(0);
			return false;
		}
	};

	template<typename T>
	T complement(const T& value) noexcept {
		static_assert(std::is_floating_point_v<T>, "ez::trig::complement only accepts floating point types as input!");
		return intern::complement(value);
	};

	template<typename T, glm::length_t L>
	glm::vec<L, T> complement(const glm::vec<L, T>& value) noexcept {
		static_assert(std::is_floating_point_v<T>, "ez::trig::complement only accepts floating point types as input!");
		return intern::complement(value);
	};

	template<typename T, glm::length_t L>
	glm::vec<3, T> toBarycentric(const glm::vec<L, T>& p, const glm::vec<L, T>& a, const glm::vec<L, T>& b, const glm::vec<L, T>& c) noexcept {
		static_assert(std::is_floating_point_v<T>, "ez::trig::toBarycentric only accepts floating point types as input!");
		using vec_t = glm::vec<L, T>;

		vec_t v0 = b - a, v1 = c - a, v2 = p - a;
		T d00 = glm::dot(v0, v0);
		T d01 = glm::dot(v0, v1);
		T d11 = glm::dot(v1, v1);
		T d20 = glm::dot(v2, v0);
		T d21 = glm::dot(v2, v1);
		T denom = d00 * d11 - d01 * d01;

		glm::vec<3, T> result;
		result.y = (d11 * d20 - d01 * d21) / denom;
		result.z = (d00 * d21 - d01 * d20) / denom;
		result.x = T(1) - result.y - result.z;

		return result;
	};
	template<typename T, glm::length_t L>
	glm::vec<L, T> fromBarycentric(const glm::vec<3, T>& coords, const glm::vec<L, T>& a, const glm::vec<L, T>& b, const glm::vec<L, T>& c) {
		static_assert(std::is_floating_point_v<T>, "ez::trig::fromBarycentric only accepts floating point types as input!");
		
		return a * coords.x + b * coords.y + c * coords.z;
	}

	// Puts the angle into standard position, ie in the range [0, 2 * pi]
	template<typename T>
	T standardPosition(const T& value) noexcept {
		static_assert(std::is_floating_point_v<T>, "ez::trig::standardPosition only accepts floating point types as input!");

		T angle = std::fmod(value, ez::tau<T>());
		if (angle < ez::zero<T>()) {
			angle += ez::tau<T>();
		}
		return angle;
	};
	
	// Puts the angle into standard position, ie in the range [0, 2 * pi]
	template<typename T, glm::length_t L>
	glm::vec<L, T> standardPosition(const glm::vec<L, T>& value) noexcept {
		static_assert(std::is_floating_point_v<T>, "ez::trig::standardPosition only accepts floating point types as input!");

		glm::vec<L, T> res;
		for (int i = 0; i < L; ++i) {
			res[i] = standardPosition<T>(value[i]);
		}
		return res;
	};

	// Normalizes the angle to the range [-pi/2, pi/2] 
	template<typename T>
	T normalizeAngle(const T& value) noexcept {
		static_assert(std::is_floating_point_v<T>, "ez::trig::normalizeAngle only accepts floating point types as input!");

		T angle = std::fmod(value + ez::pi<T>(), ez::tau<T>());
		if (angle < ez::zero<T>()) {
			angle += ez::tau<T>();
		}
		return angle - ez::pi<T>();
	};

	// Normalizes the angle to the range [-pi/2, pi/2]
	template<typename T, glm::length_t L>
	glm::vec<L, T> normalizeAngle(const glm::vec<L, T>& value) noexcept {
		static_assert(std::is_floating_point_v<T>, "ez::trig::normalizeAngle only accepts floating point types as input!");

		glm::vec<L, T> res;
		for (int i = 0; i < L; ++i) {
			res[i] = normalizeAngle<T>(value[i]);
		}
		return res;
	};
};