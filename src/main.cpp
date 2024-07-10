//
// Created by atwbzhang on 2024/7/9.
//

#include "common.h"
#include "sphere.h"
#include "hittable_list.h"
#include "camera.h"
#include "many_materials.h"

int main() {
    //Camera
    Camera camera(Vec3(0, 0, 0));
    camera.set_aspect_ratio(16.0 / 9.0);
    camera.set_image_width(400);
    camera.set_samples_per_pixel(100);
    camera.set_max_depth(50);

    // Load the sphere data.
    HittableList world;
    // world.add(std::make_shared<Sphere>(Vec3(0, 0, -1), 0.5));
    // world.add(std::make_shared<Sphere>(Vec3(0, -100.5, -1), 100));
    auto material_ground = std::make_shared<Lambertian>(Color(0.8, 0.8, 0.0));
    auto material_center = std::make_shared<Lambertian>(Color(0.1, 0.2, 0.5));
    auto material_left   = std::make_shared<Metal>(Color(0.8, 0.8, 0.8));
    auto material_right  = std::make_shared<Metal>(Color(0.8, 0.6, 0.2));

    world.add(std::make_shared<Sphere>(Point3( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(std::make_shared<Sphere>(Point3( 0.0,    0.0, -1.2),   0.5, material_center));
    world.add(std::make_shared<Sphere>(Point3(-1.0,    0.0, -1.0),   0.5, material_left));
    world.add(std::make_shared<Sphere>(Point3( 1.0,    0.0, -1.0),   0.5, material_right));

    // Render
    camera.render(world);
    return 0;
}