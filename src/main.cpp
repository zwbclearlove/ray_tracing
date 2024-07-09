//
// Created by atwbzhang on 2024/7/9.
//
#include <iostream>
#include <fstream>

#include "vec3.h"
#include "color.h"

int main() {
    // Image
    auto aspect_ratio = 16.0 / 9.0;
    int image_width = 800;
    int image_height = int(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    // Viewport widths less than one are ok since they are real valued.
    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * (double(image_width) / image_height);

    // Render
    std::ofstream file("test.ppm", std::ios::out);
    file << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = 0; j < image_height; j++) {
        std::cerr << "\nScanline remaining: " << (image_height - j) << ' ' << std::flush;
        for (int i = 0; i < image_width; i++) {
            auto pixel_color = Color(double(i) / (image_width - 1), double(j) / (image_height - 1), 0);
            write_color(file, pixel_color);
        }
    }
    std::cerr << "\nDone.\n";
    return 0;
}