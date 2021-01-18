#pragma once
#include <glm/geometric.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace ez {
	namespace Triangle {
		template<typename T, glm::length_t N>
		void barycentric(glm::vec<N, T> p, glm::vec<N, T> a, glm::vec<N, T> b, glm::vec<N, T> c, T& u, T& v, T& w) {
			glm::vec<N, T> v0 = b - a, v1 = c - a, v2 = p - a;
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
	};
};