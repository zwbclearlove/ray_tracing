//
// Created by atwbzhang on 2024/7/10.
//
#pragma once
#include "common.h"
#include "material.h"
#include "hittable.h"

class Lambertian : public Material {
  public:
    Lambertian(const Color& a) : albedo_(a) {}

    virtual bool scatter(const Ray& r_in, const HitRecord& rec, Color& attenuation, Ray& scattered) const override {
        Vec3 scatter_direction = rec.normal + random_unit_vector();

        // Catch degenerate scatter direction.
        if (scatter_direction.near_zero()) {
            scatter_direction = rec.normal;
        }
        
        scattered = Ray(rec.p, scatter_direction);
        attenuation = albedo_;
        return true;
    }

  private:
    Color albedo_;
};


class Metal : public Material {
  public:
    Metal(const Color& a) : albedo_(a) {}

    virtual bool scatter(const Ray& r_in, const HitRecord& rec, Color& attenuation, Ray& scattered) const override {
        Vec3 reflected = reflect(r_in.direction(), rec.normal);
        scattered = Ray(rec.p, reflected);
        attenuation = albedo_;
        return true;
    }

  private:
    Color albedo_;
};