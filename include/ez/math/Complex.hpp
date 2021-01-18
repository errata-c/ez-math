#pragma once
#include <cmath>
#include <glm/vec2.hpp>

namespace ez {
	template<typename T>
	struct Complex
	{
		constexpr Complex()
			: r{ static_cast<T>(1) }
			, i{ static_cast<T>(0) }
		{}
		constexpr Complex(T _r, T _i)
			: r{ _r }
			, i{ _i }
		{}
		~Complex() = default;
		constexpr Complex(const Complex&) = default;
		constexpr Complex& operator=(const Complex&) = default;
		Complex(Complex&&) noexcept = default;
		Complex& operator=(Complex&&) noexcept = default;

		// These cannot be constexpr, because compiler treats 'data' as inactive union member.
		T& operator[](size_t index) {
			return data[index];
		}
		T operator[](size_t index) const {
			return data[index];
		}

		constexpr T real() const {
			return r;
		}
		constexpr T& real() {
			return r;
		}
		constexpr T imag() const {
			return i;
		}
		constexpr T& imag() {
			return i;
		}

		T length() const {
			return std::sqrt(norm());
		}
		constexpr T norm() const {
			return r * r + i * i;
		}
		T arg() const {
			return std::atan2(i, r);
		}
		T cosAngle() const {
			return r / length();
		}
		T sinAngle() const {
			return i / length();
		}
		T dotAngle() const {
			return std::acos(cosAngle());
		}
		constexpr T dot(const Complex& other) const {
			return r * other.r + i * other.i;
		}
		constexpr Complex conj() const {
			return Complex{r, -i};
		}
		Complex normal() const {
			return Complex{ *this } / length();
		}
		Complex& normalize() {
			(*this) /= length();
			return *this;
		}

		constexpr glm::tvec2<T> rotate(const glm::tvec2<T>& v) const {
			return glm::tvec2<T>{
				v.x* r - v.y * i,
				v.x* i + v.y * r
			};
		}
		constexpr glm::tvec2<T> reverseRotate(const glm::tvec2<T>& v) const {
			T denom = static_cast<T>(1) / norm();
			return glm::tvec2<T>{
				(v.x * r + v.y * i) * denom,
				(v.y * r - v.x * i) * denom
			};
		}
		constexpr Complex rotate(Complex o) const {
			return Complex{
				o.r * r - o.i * i,
				o.r * i + o.i * r
			};
		}
		constexpr Complex reverseRotate(Complex o) const {
			T denom = static_cast<T>(1) / norm();
			return Complex{
				(o.r* r + o.i * i)* denom,
				(o.i* r - o.r * i)* denom
			};
		}
		constexpr glm::tvec2<T> rotate(const glm::tvec2<T>& p, const glm::tvec2<T>& o) const {
			/// Instead of subtracting p from o, I distributed the products.
			/// For some reason this increases accuracy dramatically.
			return glm::vec2{
				p.x * r - o.x * r - p.y * i + o.y * i + o.x,
				p.x * i - o.x * i + p.y * r - o.y * r + o.y
			};
		}
		constexpr glm::tvec2<T> reverseRotate(const glm::tvec2<T>& p, const glm::tvec2<T>& o) const {
			T denom = static_cast<T>(1) / norm();
			return glm::tvec2<T>{
				(p.x * r - o.x * r + p.y * i - o.y * i) * denom,
				(p.y * r - o.y * r - p.x * i - o.x * i) * denom
			};
		}

		// sin and cos are not constexpr, so these can't be either.
		static Complex fromPolar(T theta) {
			return Complex{ std::cos(theta), std::sin(theta) };
		}
		static Complex fromPolar(T theta, T len) {
			return Complex{ std::cos(theta) * len, std::sin(theta) * len };
		}

		Complex& operator+=(const Complex& o) {
			r += o.r;
			i += o.i;
			return *this;
		}
		Complex& operator-=(const Complex& o) {
			r -= o.r;
			i -= o.i;
			return *this;
		}
		Complex& operator*=(const Complex& o) {
			(*this) = rotate(o);
			return *this;
		}
		Complex& operator/=(const Complex& o) {
			(*this) = reverseRotate(o);
			return *this;
		}

		Complex& operator+=(T amount) {
			r += amount;
			i += amount;
			return *this;
		}
		Complex& operator-=(T amount) {
			r -= amount;
			i -= amount;
			return *this;
		}
		Complex& operator*=(T amount) {
			r *= amount;
			i *= amount;
			return *this;
		}
		Complex& operator/=(T amount) {
			r /= amount;
			i /= amount;
			return *this;
		}

		union {
			struct {
				T r;
				T i;
			};
			T data[2];
		};
	};

	template<typename T>
	constexpr Complex<T> operator+(const Complex<T> & a, const Complex<T>& b) {
		return Complex<T>{a.r + b.r, a.i + b.i};
	}
	template<typename T>
	constexpr Complex<T> operator-(const Complex<T>& a, const Complex<T>& b) {
		return Complex<T>{a.r - b.r, a.i - b.i};
	}
	template<typename T>
	constexpr Complex<T> operator*(const Complex<T>& a, const Complex<T>& b) {
		return a.rotate(b);
	}
	template<typename T>
	constexpr Complex<T> operator/(const Complex<T>& a, const Complex<T>& b) {
		return a.reverseRotate(b);
	}

	template<typename T>
	constexpr Complex<T> operator+(T a, const Complex<T>& b) {
		return Complex<T>{a + b.r, a + b.i};
	}
	template<typename T>
	constexpr Complex<T> operator-(T a, const Complex<T>& b) {
		return Complex<T>{a - b.r, a - b.i};
	}
	template<typename T>
	constexpr Complex<T> operator*(T a, const Complex<T>& b) {
		return Complex<T>{a * b.r, a * b.i};
	}
	template<typename T>
	constexpr Complex<T> operator/(T a, const Complex<T>& b) {
		return Complex<T>{a / b.r, a / b.i};
	}

	template<typename T>
	constexpr Complex<T> operator+(const Complex<T>& a, T b) {
		return Complex<T>{a.r + b, a.i + b};
	}
	template<typename T>
	constexpr Complex<T> operator-(const Complex<T>& a, T b) {
		return Complex<T>{a.r - b, a.i - b};
	}
	template<typename T>
	constexpr Complex<T> operator*(const Complex<T>& a, T b) {
		return Complex<T>{a.r * b, a.i * b};
	}
	template<typename T>
	constexpr Complex<T> operator/(const Complex<T>& a, T b) {
		return Complex<T>{a.r / b, a.i / b};
	}

	using Complexd = Complex<double>;
	using Complexf = Complex<float>;
};