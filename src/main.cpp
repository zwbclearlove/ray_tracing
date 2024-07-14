//
// Created by atwbzhang on 2024/7/9.
//

#include "common.h"
#include "bvh.h"
#include "sphere.h"
#include "hittable_list.h"
#include "camera.h"
#include "many_materials.h"
#include "texture.h"
#include "quad.h"


void bouncing_spheres() {
    // Load the sphere data.
    // HittableList world;
    // auto material_ground = std::make_shared<Lambertian>(Color(0.8, 0.8, 0.0));
    // auto material_center = std::make_shared<Lambertian>(Color(0.1, 0.2, 0.5));
    // auto material_left   = std::make_shared<Dielectric>(1.33);
    // auto material_bubble = std::make_shared<Dielectric>(1 / 1.33);
    // auto material_right  = std::make_shared<Metal>(Color(0.8, 0.6, 0.2), 0.3);

    // world.add(std::make_shared<Sphere>(Point3( 0.0, -100.5, -1.0), 100.0, material_ground));
    // world.add(std::make_shared<Sphere>(Point3( 0.0,    0.0, -1.2),   0.5, material_center));
    // world.add(std::make_shared<Sphere>(Point3(-1.0,    0.0, -1.0),   0.5, material_left));
    // world.add(std::make_shared<Sphere>(Point3(-1.0,    0.0, -1.0),   0.4, material_bubble));
    // world.add(std::make_shared<Sphere>(Point3( 1.0,    0.0, -1.0),   0.5, material_right));
    
    // load the world
    HittableList world;
    auto checker = std::make_shared<CheckerTexture>(0.32, Color(0.2, 0.3, 0.1), Color(0.9, 0.9, 0.9));
    auto ground_material = std::make_shared<Lambertian>(checker);
    world.add(std::make_shared<Sphere>(Point3(0, -1000, 0), 1000, ground_material));

    for (int a = -5; a < 5; a++) {
        for (int b = -5; b < 5; b++) {
            auto choose_mat = random_double();
            Point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

            if ((center - Point3(4, 0.2, 0)).length() > 0.9) {
                std::shared_ptr<Material> sphere_material;
                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = Color::random() * Color::random();
                    sphere_material = std::make_shared<Lambertian>(albedo);
                    auto center2 = center + Vec3(0, random_double(0, 0.5), 0);
                    // world.add(std::make_shared<Sphere>(center, center2, 0.2, sphere_material));
                    world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal 
                    auto albedo = Color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = std::make_shared<Metal>(albedo, fuzz);
                    world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = std::make_shared<Dielectric>(1.5);
                    world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
                }

            }
        }
    }

    auto material1 = std::make_shared<Dielectric>(1.5);
    world.add(std::make_shared<Sphere>(Point3(0, 1, 0), 1.0, material1));

    auto earth_texture = std::make_shared<ImageTexture>("D:\\Projects\\ray_tracing\\images\\earthmap.jpg");
    auto material2 = std::make_shared<Lambertian>(earth_texture);
    world.add(std::make_shared<Sphere>(Point3(-4, 1, 0), 1.0, material2));

    auto material3 = std::make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
    world.add(std::make_shared<Sphere>(Point3(4, 1, 0), 1.0, material3));

    world = HittableList(std::make_shared<BVHNode>(world));

    // Render
    // Camera
    Camera camera;
    camera.set_aspect_ratio(16.0 / 9.0);
    camera.set_image_width(1600);
    camera.set_samples_per_pixel(500);
    camera.set_max_depth(50);
    
    camera.set_fov(20);
    camera.set_lookfrom(Point3(13, 2, 3));
    camera.set_lookat(Point3(0, 0, 0));
    camera.set_vup(Vec3(0, 1, 0));

    // camera.set_defocus_angle(0.6);
    // camera.set_focus_dist(10.0);

    camera.render(world, "bouncing_spheres_with_earth.ppm");
}

void checkered_spheres() {
    HittableList world;

    auto checker = std::make_shared<CheckerTexture>(0.33, Color(0.2, 0.3, 0.1), Color(0.9, 0.9, 0.9));

    world.add(std::make_shared<Sphere>(Point3(0, -10, 0), 10, std::make_shared<Lambertian>(checker)));
    world.add(std::make_shared<Sphere>(Point3(0, 10, 0), 10, std::make_shared<Lambertian>(checker)));

    world = HittableList(std::make_shared<BVHNode>(world));

    Camera camera;
    camera.set_aspect_ratio(16.0 / 9.0);
    camera.set_image_width(800);
    camera.set_samples_per_pixel(100);
    camera.set_max_depth(50);

    camera.set_fov(20);
    camera.set_lookfrom(Point3(13, 2, 3));
    camera.set_lookat(Point3(0, 0, 0));
    camera.set_vup(Vec3(0, 1, 0));

    camera.set_defocus_angle(0);

    camera.render(world, "checkered_spheres.ppm");
}

void earth() {
    HittableList world;
    auto earth_texture = std::make_shared<ImageTexture>("D:\\Projects\\ray_tracing\\images\\earthmap.jpg");
    auto earth_surface = std::make_shared<Lambertian>(earth_texture);
    auto globe = std::make_shared<Sphere>(Point3(0, 0, 0), 2, earth_surface);
    world.add(globe);

    Camera camera;
    camera.set_aspect_ratio(16.0 / 9.0);
    camera.set_image_width(800);
    camera.set_samples_per_pixel(100);
    camera.set_max_depth(50);

    camera.set_fov(20);
    camera.set_lookfrom(Point3(0, 0, 12));
    camera.set_lookat(Point3(0, 0, 0));
    camera.set_vup(Vec3(0, 1, 0));

    camera.set_defocus_angle(0);
    
    camera.render(world, "earth.ppm");
}

void perlin_spheres() {
    HittableList world;

    auto perlin_texture = std::make_shared<NoiseTexture>(4);
    auto perlin_surface = std::make_shared<Lambertian>(perlin_texture);
    auto ground = std::make_shared<Sphere>(Point3(0, -1000, 0), 1000, perlin_surface);
    world.add(ground);
    auto sphere = std::make_shared<Sphere>(Point3(0, 2, 0), 2, perlin_surface);
    world.add(sphere);


    Camera camera;
    camera.set_aspect_ratio(16.0 / 9.0);
    camera.set_image_width(800);
    camera.set_samples_per_pixel(100);
    camera.set_max_depth(50);

    camera.set_fov(60);
    camera.set_lookfrom(Point3(13, 2, 3));
    camera.set_lookat(Point3(0, 0, 0));
    camera.set_vup(Vec3(0, 1, 0));

    camera.set_defocus_angle(0);
    
    camera.render(world, "perlin_spheres.ppm");
}

void quads() {
    HittableList world;

    // Materials
    auto left_red = std::make_shared<Lambertian>(Color(1.0, 0.2, 0.2));
    auto back_green = std::make_shared<Lambertian>(Color(0.2, 1.0, 0.2));
    auto right_blue = std::make_shared<Lambertian>(Color(0.2, 0.2, 1.0));
    auto upper_orange = std::make_shared<Lambertian>(Color(1.0, 0.5, 0.0));
    auto lower_teal = std::make_shared<Lambertian>(Color(0.2, 0.8, 0.8));

    // Quads
    world.add(std::make_shared<Quad>(Point3(-3, -2, 5), Vec3(0, 0, -4), Vec3(0, 4, 0), left_red));
    world.add(std::make_shared<Quad>(Point3(-2, -2, 0), Vec3(4, 0, 0), Vec3(0, 4, 0), back_green));
    world.add(std::make_shared<Quad>(Point3(3, -2, 1), Vec3(0, 0, 4), Vec3(0, 4, 0), right_blue));
    world.add(std::make_shared<Quad>(Point3(-2, 3, 1), Vec3(4, 0, 0), Vec3(0, 0, 4), upper_orange));
    world.add(std::make_shared<Quad>(Point3(-2, -3, 5), Vec3(4, 0, 0), Vec3(0, 0, -4), lower_teal));

    Camera camera;

    camera.set_aspect_ratio(1.0);
    camera.set_image_width(800);
    camera.set_samples_per_pixel(100);
    camera.set_max_depth(50);

    camera.set_fov(80);
    camera.set_lookfrom(Point3(0, 0, 9));
    camera.set_lookat(Point3(0, 0, 0));
    camera.set_vup(Vec3(0, 1, 0));

    camera.set_defocus_angle(0);

    camera.render(world, "quads.ppm");
}

int main() {
    // bouncing_spheres();
    // checkered_spheres();
    // earth();
    // perlin_spheres();
    quads();
    return 0;
}