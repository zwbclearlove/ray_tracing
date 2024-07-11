//
// Created by atwbzhang on 2024/7/10.
//
#pragma once
#include <iostream>
#include <fstream>

#include "common.h"
#include "hittable.h"
#include "ray.h"
#include "vec3.h"
#include "color.h"
#include "many_materials.h"

class Camera {
  public:
    Camera() {}
    Camera(const Point3& center) : center_(center) {}
    
    void render(Hittable& world) {
        initialize();

        std::ofstream file("test.ppm", std::ios::out);
        file << "P3\n" << image_width_ << ' ' << image_height_ << "\n255\n";

        for (int j = 0; j < image_height_; j++) {
            std::clog << "\nScanline remaining: " << (image_height_ - j) << ' ' << std::flush;
            for (int i = 0; i < image_width_; i++) {
                Color pixel_color(0, 0, 0);
                for (int s = 0; s < samples_per_pixel_; s++) {
                    Ray r = get_ray(i, j);
                    pixel_color += ray_color(r, max_depth_, world);
                }
                write_color(file, pixel_color * pixel_samples_scale_);
            }
        }
        std::clog << "\nDone.\n";
    }


    void set_aspect_ratio(double ar) { aspect_ratio_ = ar; }
    void set_image_width(int w) { image_width_ = w; }
    void set_samples_per_pixel(int s) { samples_per_pixel_ = s; }
    void set_max_depth(int d) { max_depth_ = d; }

  private:
    // Image
    double aspect_ratio_ = 16.0 / 9.0;
    int    image_width_ = 1600;
    int    samples_per_pixel_ = 10;
    int    max_depth_ = 10;
    double pixel_samples_scale_;
    int    image_height_;   // Rendered image height
    Point3 center_;         // Camera center
    Point3 pixel00_loc_;    // Location of pixel 0, 0
    Vec3   pixel_delta_u_;  // Offset to pixel to the right
    Vec3   pixel_delta_v_;  // Offset to pixel below


    void initialize() {
        // Caculate the image height, and ensure it is at least 1.
        int image_height = int(image_width_ / aspect_ratio_);
        image_height_ = (image_height < 1) ? 1 : image_height;
        pixel_samples_scale_ = 1.0 / samples_per_pixel_;

        // Camera
        auto focal_length = 1.0;
        auto viewport_height = 2.0;
        auto viewport_width = viewport_height * (double(image_width_) / image_height);

        // Caculate the vectors across the horizontal and down the vertical viewport edges.
        auto viewport_u = Vec3(viewport_width, 0, 0);
        auto viewport_v = Vec3(0, -viewport_height, 0);

        // Caculate the horizontal and vertical delta vectors from pixel to pixel.
        pixel_delta_u_ = viewport_u / image_width_;
        pixel_delta_v_ = viewport_v / image_height_;

        // Caculate the location of the upper left pixel.
        auto viewpoint_upper_left = center_ - Vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
        pixel00_loc_ = viewpoint_upper_left + 0.5 * (pixel_delta_u_ + pixel_delta_v_);
    }

    Ray get_ray(int i, int j) const {
        auto offset = sample_square();
        auto pixel_sample = pixel00_loc_ + (i + offset.x()) * pixel_delta_u_ + (j + offset.y()) * pixel_delta_v_;

        auto dir = pixel_sample - center_;
        return Ray(center_, dir);
    }

    Vec3 sample_square() const {
        // Returns the vector to a random point in the [-.5,-.5]-[+.5,+.5] unit square.
        return Vec3(random_double() - 0.5, random_double() - 0.5, 0);
    }

    Color ray_color(const Ray& r, int depth, const Hittable& world) {
        HitRecord rec;
        if (depth <= 0) {
            return Color(0, 0, 0);
        }

        // ignore hits that are very close to the calculated intersection point
        if (world.hit(r, Interval(0.0001, kInfinity), rec)) {
            // Vec3 direction = random_on_hemisphere(rec.normal);
            // return 0.5 * ray_color(Ray(rec.p, direction), depth - 1, world);
            // Vec3 target = rec.p + rec.normal + random_on_hemisphere(rec.normal);
            // return 0.5 * ray_color(Ray(rec.p, target - rec.p), depth - 1, world);
            Ray scattered;
            Color attenuation;
            if (rec.material_ptr->scatter(r, rec, attenuation, scattered))
                return attenuation * ray_color(scattered, depth - 1, world);
            return Color(0, 0, 0);
        }

        Vec3 unit_direction = unit_vector(r.direction());
        auto a = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - a) * Color(1.0, 1.0, 1.0) + a * Color(0.5, 0.7, 1.0);
    }

};