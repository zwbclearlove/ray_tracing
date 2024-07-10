//
// Created by atwbzhang on 2024/7/9.
//

#include "common.h"
#include "sphere.h"
#include "hittable_list.h"
#include "camera.h"

int main() {
    //Camera
    Camera camera(Vec3(0, 0, 0));
    camera.set_aspect_ratio(16.0 / 9.0);
    camera.set_image_width(800);
    camera.set_samples_per_pixel(100);

    // Load the sphere data.
    HittableList world;
    world.add(std::make_shared<Sphere>(Vec3(0, 0, -1), 0.5));
    world.add(std::make_shared<Sphere>(Vec3(0, -100.5, -1), 100));

    // Render
    camera.render(world);
    return 0;
}