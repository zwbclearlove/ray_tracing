//
// Created by atwbzhang on 2024/7/10.
//
#pragma once
#include "common.h"
#include "ray.h"
#include "color.h"
#include "vec3.h"
#include "texture.h"

class HitRecord;

class Material {
  public:
    virtual ~Material() = default;

    virtual Color emitted(double u, double v, const Point3& p) const {
        return Color(0, 0, 0);
    }

    virtual bool scatter(const Ray& r_in, const HitRecord& rec, Color& attenuation, Ray& scattered) const = 0;

    // virtual double scattering_pdf(const Ray& r_in, const HitRecord& rec, const Ray& scattered) const {
    //     return 0;
    // }
};

