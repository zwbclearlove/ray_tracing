//
// Created by atwbzhang on 2024/7/9.
//

#include "common.h"
#include "sphere.h"
#include "hittable_list.h"
#include "camera.h"
#include "many_materials.h"

int main() {
    

    // Load the sphere data.
    HittableList world;
    auto material_ground = std::make_shared<Lambertian>(Color(0.8, 0.8, 0.0));
    auto material_center = std::make_shared<Lambertian>(Color(0.1, 0.2, 0.5));
    auto material_left   = std::make_shared<Dielectric>(1.33);
    auto material_bubble = std::make_shared<Dielectric>(1 / 1.33);
    auto material_right  = std::make_shared<Metal>(Color(0.8, 0.6, 0.2), 0.3);

    world.add(std::make_shared<Sphere>(Point3( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(std::make_shared<Sphere>(Point3( 0.0,    0.0, -1.2),   0.5, material_center));
    world.add(std::make_shared<Sphere>(Point3(-1.0,    0.0, -1.0),   0.5, material_left));
    world.add(std::make_shared<Sphere>(Point3(-1.0,    0.0, -1.0),  0.45, material_bubble));
    world.add(std::make_shared<Sphere>(Point3( 1.0,    0.0, -1.0),   0.5, material_right));
    
    
    // auto R = cos(kPi / 4);
    // auto material_left = std::make_shared<Lambertian>(Color(0, 0, 1));
    // auto material_right = std::make_shared<Lambertian>(Color(1, 0, 0));

    // world.add(std::make_shared<Sphere>(Point3(-R, 0, -1), R, material_left));
    // world.add(std::make_shared<Sphere>(Point3(R, 0, -1), R, material_right));

    // Render
    //Camera
    Camera camera(Point3(0, 0, 0));
    camera.set_aspect_ratio(16.0 / 9.0);
    camera.set_image_width(800);
    camera.set_samples_per_pixel(30);
    camera.set_max_depth(30);
    camera.set_fov(90);
    // camera.set_lookfrom(Point3(-2, -2, 1));
    // camera.set_lookat(Point3(0, 0, -1));
    // camera.set_vup(Vec3(0, 1, 0));
    camera.render(world);
    return 0;
}