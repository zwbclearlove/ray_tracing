//
// Created by atwbzhang on 2024/7/9.
//
#pragma once
#include <iostream>

#include "vec3.h"
#include "interval.h"

using Color = Vec3;

inline double linear2gamma(double linear_component) {
    // Convert a linear RGB component to a gamma-corrected value.
    if (linear_component > 0) {
        return sqrt(linear_component);
    }
    return 0;
}

void write_color(std::ostream &out, const Color &pixel_color) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // apply a linear to gamma conversion
    r = linear2gamma(r);
    g = linear2gamma(g);
    b = linear2gamma(b);

    static const Interval intensity(0.000, 0.999);
    // Translate the [0,1] component values to the byte range [0,255].
    int rbyte = static_cast<int>(256 * intensity.clamp(r));
    int gbyte = static_cast<int>(256 * intensity.clamp(g));
    int bbyte = static_cast<int>(256 * intensity.clamp(b));

    // Write out the pixel color components.
    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}
