//
// Created by atwbzhang on 2024/7/9.
//

#ifndef RAY_TRACING_COLOR_H
#define RAY_TRACING_COLOR_H

#include "vec3.h"

#include <iostream>

using Color = Vec3;

void write_color(std::ostream &out, const Color &pixel_color) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Translate the [0,1] component values to the byte range [0,255].
    int rbyte = int(255.999 * r);
    int gbyte = int(255.999 * g);
    int bbyte = int(255.999 * b);

    // Write out the pixel color components.
    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

#endif //RAY_TRACING_COLOR_H
