//
// Created by atwbzhang on 2024/7/10.
//
#pragma once
#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>

#include "vec3.h"
#include "ray.h"
#include "color.h"
#include "interval.h"

// constants
constexpr double kPi = 3.1415926535897932385;

// utility functions
inline double degrees2radians(double degrees) {
  return degrees * kPi / 180.0;
}

inline double random_double() { return rand() / (RAND_MAX + 1.0); }
inline double random_double(double min, double max) {
    return min + (max - min) * random_double();
}

inline double ffmin(double a, double b) { return (a < b ? a : b); }
inline double ffmax(double a, double b) { return (a > b ? a : b); }
