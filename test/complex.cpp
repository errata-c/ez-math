#include <catch2/catch.hpp>

#include <limits>
#include <fmt/core.h>

#include <glm/geometric.hpp>

#include <ez/math/complex.hpp>
//#include <ez/math/poly.hpp>
//#include <ez/math/constants.hpp>
//#include <ez/math/trig.hpp>

// Test the constexpr capabilities of the complex class.
static constexpr glm::dcomplex cpx0{ 1, 0 };
static constexpr glm::dcomplex cpx1 = cpx0;

TEST_CASE("complex") {
    glm::vec2 val{1, -1};
    glm::complex cpx{1, -1};

    REQUIRE(glm::dot(val, val) == Approx(glm::dot(cpx, cpx)));
    REQUIRE(glm::length(val) == Approx(glm::length(cpx)));
}