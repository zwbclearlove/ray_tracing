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
    
    void render(Hittable& world, const std::string& filename) {
        initialize();

        std::ofstream file(filename, std::ios::out);
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
    void set_fov(double f) { vfov_ = f; }
    void set_samples_per_pixel(int s) { samples_per_pixel_ = s; }
    void set_max_depth(int d) { max_depth_ = d; }
    void set_lookfrom(const Point3& lookfrom) { lookfrom_ = lookfrom; }
    void set_lookat(const Point3& lookat) { lookat_ = lookat; }
    void set_vup(const Vec3& vup) { vup_ = vup; }
    void set_defocus_angle(double angle) { defocus_angle_ = angle; }
    void set_focus_dist(double dist) { focus_distance_ = dist; }
    void set_background_color(const Color& color) { background_color_ = color; }

  private:
    // Image
    double aspect_ratio_ = 16.0 / 9.0;
    int    image_width_ = 1600;
    double vfov_ = 90.0;
    Point3 lookfrom_ = Point3(0, 0, 0);
    Point3 lookat_ = Point3(0, 0, -1);
    Vec3   vup_ = Vec3(0, 1, 0);
    double defocus_angle_ = 0.0;
    double focus_distance_ = 10;

    int    samples_per_pixel_ = 10;
    int    max_depth_ = 10;
    Color  background_color_ = Color(0, 0, 0);

    double pixel_samples_scale_;
    int    image_height_;   // Rendered image height
    Point3 center_;         // Camera center
    Point3 pixel00_loc_;    // Location of pixel 0, 0
    Vec3   pixel_delta_u_;  // Offset to pixel to the right
    Vec3   pixel_delta_v_;  // Offset to pixel below
    Vec3   u_, v_, w_;      // Camera basis vectors
    Vec3   defocus_disk_u_; // Defocus disk basis vector horizontal
    Vec3   defocus_disk_v_; // Defocus disk basis vector vertical

    void initialize() {
        // Caculate the image height, and ensure it is at least 1.
        int image_height = int(image_width_ / aspect_ratio_);
        image_height_ = (image_height < 1) ? 1 : image_height;
        pixel_samples_scale_ = 1.0 / samples_per_pixel_;

        center_ = lookfrom_;

        // Camera
        // auto focal_length = (lookfrom_ - lookat_).length();
        // auto focal_length = 1.0;
        auto theta = degrees2radians(vfov_);
        auto h = tan(theta / 2);

        auto viewport_height = 2 * h * focus_distance_;
        auto viewport_width = viewport_height * (double(image_width_) / image_height);

        // Caculate the camera basis vectors.
        w_ = unit_vector(lookfrom_ - lookat_);
        u_ = unit_vector(cross(vup_, w_));
        v_ = cross(w_, u_);

        // Caculate the vectors across the horizontal and down the vertical viewport edges.
        auto viewport_u = viewport_width * u_;
        auto viewport_v = viewport_height * -v_;
        // Vec3 viewport_u(viewport_width, 0, 0);
        // Vec3 viewport_v(0, -viewport_height, 0);

        // Caculate the horizontal and vertical delta vectors from pixel to pixel.
        pixel_delta_u_ = viewport_u / image_width_;
        pixel_delta_v_ = viewport_v / image_height_;

        // Caculate the location of the upper left pixel.
        auto viewpoint_upper_left = center_ - focus_distance_ * w_ - viewport_u / 2 - viewport_v / 2;
        // auto viewpoint_upper_left = center_ - Vec3(0, 0, focal_length) - 0.5 * (viewport_u + viewport_v);
        pixel00_loc_ = viewpoint_upper_left + 0.5 * (pixel_delta_u_ + pixel_delta_v_);

        // Caculate the defocus disk basis vectors.
        auto defocus_disk_radius = focus_distance_ * tan(degrees2radians(defocus_angle_) / 2);
        defocus_disk_u_ = defocus_disk_radius * u_;
        defocus_disk_v_ = defocus_disk_radius * v_;
    }

    Ray get_ray(int i, int j) const {
        // Construct a camera ray originating from the defocus disk and directed at a randomly
        // sampled point around the pixel location i, j.
        auto offset = sample_square();
        auto pixel_sample = pixel00_loc_ + (i + offset.x()) * pixel_delta_u_ + (j + offset.y()) * pixel_delta_v_;

        auto ray_origin = (defocus_angle_ <= 0) ? center_ : defocus_disk_sample();
        auto ray_direction = pixel_sample - center_;
        auto ray_time = random_double();
        return Ray(ray_origin, ray_direction, ray_time);
    }

    Vec3 sample_square() const {
        // Returns the vector to a random point in the [-.5,-.5]-[+.5,+.5] unit square.
        return Vec3(random_double() - 0.5, random_double() - 0.5, 0);
    }

    Point3 defocus_disk_sample() const {
        // Returns a random point in the defocus disk.
        auto p = random_in_unit_disk();
        return center_ + p.x() * defocus_disk_u_ + p.y() * defocus_disk_v_;
    }

    Color ray_color(const Ray& r, int depth, const Hittable& world) {
        if (depth <= 0) {
            return Color(0, 0, 0);
        }

        HitRecord rec;

        // If the ray hits nothing, return the background color.
        if (!world.hit(r, Interval(0.001, kInfinity), rec)) {
            return background_color_;
        }

        Ray scattered;
        Color attenuation;
        Color emission = rec.material_ptr->emitted(rec.u, rec.v, rec.p);

        if (!rec.material_ptr->scatter(r, rec, attenuation, scattered)) {
            return emission;
        }

        Color scatter = attenuation * ray_color(scattered, depth - 1, world);

        return emission + scatter;

        // // ignore hits that are very close to the calculated intersection point
        // if (world.hit(r, Interval(0.0001, kInfinity), rec)) {
        //     // Vec3 direction = random_on_hemisphere(rec.normal);
        //     // return 0.5 * ray_color(Ray(rec.p, direction), depth - 1, world);
        //     // Vec3 target = rec.p + rec.normal + random_on_hemisphere(rec.normal);
        //     // return 0.5 * ray_color(Ray(rec.p, target - rec.p), depth - 1, world);
        //     Ray scattered;
        //     Color attenuation;
        //     if (rec.material_ptr->scatter(r, rec, attenuation, scattered))
        //         return attenuation * ray_color(scattered, depth - 1, world);
        //     return Color(0, 0, 0);
        // }

        // Vec3 unit_direction = unit_vector(r.direction());
        // auto a = 0.5 * (unit_direction.y() + 1.0);
        // return (1.0 - a) * Color(1.0, 1.0, 1.0) + a * Color(0.5, 0.7, 1.0);
    }

};