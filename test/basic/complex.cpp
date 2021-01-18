#include <limits>
#include <fmt/printf.h>
#include <iostream>
#include <ctime>

#include <ez/math/Complex.hpp>
#include <ez/math/Polynomial.hpp>
#include <ez/math/MathConstants.hpp>
#include <ez/math/Triangle.hpp>
#include <ez/math/PsuedoRandom.hpp>

#include <ez/math/Trig.hpp>

// Test the constexpr capabilities of the complex class.
static constexpr ez::Complex<double> cpx0{ 1, 0 };
static constexpr ez::Complex<double> cpx1 = cpx0;
static constexpr ez::Complex<double> cpx2 = cpx0 + cpx1;
static constexpr ez::Complex<double> cpx3 = cpx0 - cpx1;
static constexpr ez::Complex<double> cpx4 = cpx0 * cpx1;
static constexpr ez::Complex<double> cpx5 = cpx0 / cpx1;