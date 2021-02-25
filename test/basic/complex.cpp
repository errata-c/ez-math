#include <limits>
#include <fmt/printf.h>
#include <iostream>
#include <ctime>

#include <ez/math/complex.hpp>
#include <ez/math/poly.hpp>
#include <ez/math/constants.hpp>
#include <ez/math/trig.hpp>
#include <ez/math/prng.hpp>

// Test the constexpr capabilities of the complex class.
static constexpr glm::dcomplex cpx0{ 1, 0 };
static constexpr glm::dcomplex cpx1 = cpx0;