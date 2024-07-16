//
// Created by atwbzhang on 2024/7/9.
//

#include "common.h"
#include "bvh.h"
#include "sphere.h"
#include "hittable_list.h"
#include "camera.h"
#include "many_materials.h"
#include "constant_medium.h"
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
    camera.set_background_color(Color(0.7, 0.8, 1.0));
    
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
    camera.set_background_color(Color(0.7, 0.8, 1.0));

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
    camera.set_background_color(Color(0.7, 0.8, 1.0));

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
    camera.set_background_color(Color(0.7, 0.8, 1.0));

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
    camera.set_background_color(Color(0.7, 0.8, 1.0));

    camera.set_fov(80);
    camera.set_lookfrom(Point3(0, 0, 9));
    camera.set_lookat(Point3(0, 0, 0));
    camera.set_vup(Vec3(0, 1, 0));

    camera.set_defocus_angle(0);

    camera.render(world, "quads.ppm");
}

void simple_light() {
    HittableList world;

    auto perlin_texture = std::make_shared<NoiseTexture>(4);
    auto perlin_surface = std::make_shared<Lambertian>(perlin_texture);
    auto ground = std::make_shared<Sphere>(Point3(0, -1000, 0), 1000, perlin_surface);
    world.add(ground);
    auto sphere = std::make_shared<Sphere>(Point3(0, 2, 0), 2, perlin_surface);
    world.add(sphere);

    auto difflight = std::make_shared<DiffuseLight>(Color(4, 4, 4));
    world.add(std::make_shared<Sphere>(Point3(0, 7, 0), 1, difflight));
    world.add(std::make_shared<Quad>(Point3(3, 1, -2), Vec3(2, 0, 0), Vec3(0, 2, 0), difflight));


    Camera camera;
    camera.set_aspect_ratio(16.0 / 9.0);
    camera.set_image_width(800);
    camera.set_samples_per_pixel(100);
    camera.set_max_depth(50);
    camera.set_background_color(Color(0, 0, 0));

    camera.set_fov(60);
    camera.set_lookfrom(Point3(13, 2, 3));
    camera.set_lookat(Point3(0, 0, 0));
    camera.set_vup(Vec3(0, 1, 0));

    camera.set_defocus_angle(0);
    
    camera.render(world, "simple_light.ppm");
}

void cornell_box() {
    HittableList world;

    auto red = std::make_shared<Lambertian>(Color(0.65, 0.05, 0.05));
    auto white = std::make_shared<Lambertian>(Color(0.73, 0.73, 0.73));
    auto green = std::make_shared<Lambertian>(Color(0.12, 0.45, 0.15));
    auto light = std::make_shared<DiffuseLight>(Color(15, 15, 15));

    world.add(std::make_shared<Quad>(Point3(555, 0, 0), Vec3(0, 555, 0), Vec3(0, 0, 555), green));
    world.add(std::make_shared<Quad>(Point3(0, 0, 0), Vec3(0, 555, 0), Vec3(0, 0, 555), red));
    world.add(std::make_shared<Quad>(Point3(343, 554, 332), Vec3(-130, 0, 0), Vec3(0, 0, -105), light));
    world.add(std::make_shared<Quad>(Point3(0, 0, 0), Vec3(555, 0, 0), Vec3(0, 0, 555), white));
    world.add(std::make_shared<Quad>(Point3(555, 555, 555), Vec3(-555, 0, 0), Vec3(0, 0, -555), white));
    world.add(std::make_shared<Quad>(Point3(0, 0, 555), Vec3(555, 0, 0), Vec3(0, 555, 0), white));

    std::shared_ptr<Hittable> box1 = box(Point3(0, 0, 0), Point3(165, 165, 165), white);
    box1 = std::make_shared<RotateY>(box1, -18);
    box1 = std::make_shared<Translate>(box1, Vec3(130, 0, 65));
    world.add(box1);
    
    std::shared_ptr<Hittable> box2 = box(Point3(0, 0, 0), Point3(165, 330, 165), white);
    box2 = std::make_shared<RotateY>(box2, 15);
    box2 = std::make_shared<Translate>(box2, Vec3(265, 0, 295));
    world.add(box2);

    world = HittableList(std::make_shared<BVHNode>(world));

    Camera camera;

    camera.set_aspect_ratio(1.0);
    camera.set_image_width(800);
    camera.set_samples_per_pixel(900);
    camera.set_max_depth(100);
    camera.set_background_color(Color(0, 0, 0));

    camera.set_fov(40);
    camera.set_lookfrom(Point3(278, 278, -800));
    camera.set_lookat(Point3(278, 278, 0));
    camera.set_vup(Vec3(0, 1, 0));

    camera.set_defocus_angle(0);

    // camera.render(world, "cornell_box.ppm");
    camera.render_multithread(world, "cornell_box.ppm");
}

void cornell_smoke() {
    HittableList world;

    auto red = std::make_shared<Lambertian>(Color(0.65, 0.05, 0.05));
    auto white = std::make_shared<Lambertian>(Color(0.73, 0.73, 0.73));
    auto green = std::make_shared<Lambertian>(Color(0.12, 0.45, 0.15));
    auto light = std::make_shared<DiffuseLight>(Color(7, 7, 7));

    world.add(std::make_shared<Quad>(Point3(555, 0, 0), Vec3(0, 555, 0), Vec3(0, 0, 555), green));
    world.add(std::make_shared<Quad>(Point3(0, 0, 0), Vec3(0, 555, 0), Vec3(0, 0, 555), red));
    world.add(std::make_shared<Quad>(Point3(113, 554, 127), Vec3(330, 0, 0), Vec3(0, 0, 305), light));
    world.add(std::make_shared<Quad>(Point3(0, 555, 0), Vec3(555, 0, 0), Vec3(0, 0, 555), white));
    world.add(std::make_shared<Quad>(Point3(0, 0, 0), Vec3(555, 0, 0), Vec3(0, 0, 555), white));
    world.add(std::make_shared<Quad>(Point3(0, 0, 555), Vec3(555, 0, 0), Vec3(0, 555, 0), white));

    std::shared_ptr<Hittable> box1 = box(Point3(0, 0, 0), Point3(165, 330, 165), white);
    box1 = std::make_shared<RotateY>(box1, 15);
    box1 = std::make_shared<Translate>(box1, Vec3(265, 0, 295));
    world.add(std::make_shared<ConstantMedium>(box1, 0.01, Color(0, 0, 0)));

    std::shared_ptr<Hittable> box2 = box(Point3(0, 0, 0), Point3(165, 165, 165), white);
    box2 = std::make_shared<RotateY>(box2, -18);
    box2 = std::make_shared<Translate>(box2, Vec3(130, 0, 65));
    world.add(std::make_shared<ConstantMedium>(box2, 0.01, Color(1, 1, 1)));

    world = HittableList(std::make_shared<BVHNode>(world));

    Camera camera;

    camera.set_aspect_ratio(1.0);
    camera.set_image_width(400);
    camera.set_samples_per_pixel(100);
    camera.set_max_depth(30);
    camera.set_background_color(Color(0, 0, 0));

    camera.set_fov(40);
    camera.set_lookfrom(Point3(278, 278, -800));
    camera.set_lookat(Point3(278, 278, 0));
    camera.set_vup(Vec3(0, 1, 0));

    camera.set_defocus_angle(0);

    camera.render_multithread(world, "cornell_smoke.ppm");
}

void final_scene(int image_width, int samples_per_pixel, int max_depth) {
    HittableList boxes1;
    auto ground = std::make_shared<Lambertian>(Color(0.48, 0.83, 0.53));

    const int boxes_per_side = 20;
    for (int i = 0; i < boxes_per_side; i++) {
        for (int j = 0; j < boxes_per_side; j++) {
            auto w = 100.0;
            auto x0 = -1000.0 + i * w;
            auto z0 = -1000.0 + j * w;
            auto y0 = 0.0;
            auto x1 = x0 + w;
            auto y1 = random_double(1, 101);
            auto z1 = z0 + w;

            boxes1.add(box(Point3(x0, y0, z0), Point3(x1, y1, z1), ground));
        }
    }

    HittableList world;

    world.add(std::make_shared<BVHNode>(boxes1));

    auto light = std::make_shared<DiffuseLight>(Color(7, 7, 7));
    world.add(std::make_shared<Quad>(Point3(123, 554, 147), Vec3(300, 0, 0), Vec3(0, 0, 265), light));

    auto center1 = Point3(400, 400, 200);
    auto center2 = center1 + Vec3(30, 0, 0);
    auto sphere_material = std::make_shared<Lambertian>(Color(0.7, 0.3, 0.1));
    world.add(std::make_shared<Sphere>(center1, center2, 50, sphere_material));

    auto dielectric = std::make_shared<Dielectric>(1.5);
    world.add(std::make_shared<Sphere>(Point3(260, 150, 45), 50, dielectric));
    auto metal = std::make_shared<Metal>(Color(0.8, 0.8, 0.9), 1.0);
    world.add(std::make_shared<Sphere>(Point3(0, 150, 145), 50, metal));

    auto boundary = std::make_shared<Sphere>(Point3(360, 150, 145), 70, dielectric);
    world.add(boundary);
    world.add(std::make_shared<ConstantMedium>(boundary, 0.2, Color(0.2, 0.4, 0.9)));
    boundary = std::make_shared<Sphere>(Point3(0, 0, 0), 5000, dielectric);
    world.add(std::make_shared<ConstantMedium>(boundary, 0.0001, Color(1, 1, 1)));

    auto emat = std::make_shared<Lambertian>(std::make_shared<ImageTexture>("D:\\Projects\\ray_tracing\\images\\earthmap.jpg"));
    world.add(std::make_shared<Sphere>(Point3(400, 200, 400), 100, emat));
    auto pertext = std::make_shared<NoiseTexture>(0.2);
    world.add(std::make_shared<Sphere>(Point3(220, 280, 300), 80, std::make_shared<Lambertian>(pertext)));

    HittableList boxes2;
    auto white = std::make_shared<Lambertian>(Color(0.73, 0.73, 0.73));
    const int ns = 1000;
    for (int j = 0; j < ns; j++) {
        boxes2.add(std::make_shared<Sphere>(Point3::random(0, 165), 10, white));
    }

    world.add(std::make_shared<Translate>(std::make_shared<RotateY>(std::make_shared<BVHNode>(boxes2), 15), Vec3(-100, 270, 395)));

    Camera camera;

    camera.set_aspect_ratio(1.0);
    camera.set_image_width(image_width);
    camera.set_samples_per_pixel(samples_per_pixel);
    camera.set_max_depth(max_depth);
    camera.set_background_color(Color(0, 0, 0));

    camera.set_fov(40);
    camera.set_lookfrom(Point3(478, 278, -600));
    camera.set_lookat(Point3(278, 278, 0));
    camera.set_vup(Vec3(0, 1, 0));

    camera.set_defocus_angle(0);

    camera.render_multithread(world, "final_scene.ppm");
}

int main() {
    // bouncing_spheres();
    // checkered_spheres();
    // earth();
    // perlin_spheres();
    // quads();
    // simple_light();
    // cornell_box();
    // cornell_smoke();
    final_scene(800, 6400, 40);
    // final_scene(800, 10000, 40);
    return 0;
}