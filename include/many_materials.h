//
// Created by atwbzhang on 2024/7/10.
//
#pragma once
#include "common.h"
#include "vec3.h"
#include "material.h"
#include "hittable.h"
#include "texture.h"


class Lambertian : public Material {
  public:
    Lambertian(const Color& a) : texture_(std::make_shared<SolidColor>(a)) {}
    Lambertian(std::shared_ptr<Texture> texture) : texture_(texture) {}

    virtual bool scatter(const Ray& r_in, const HitRecord& rec, Color& attenuation, Ray& scattered) const override {
        Vec3 scatter_direction = rec.normal + random_unit_vector();

        // Catch degenerate scatter direction.
        if (scatter_direction.near_zero()) {
            scatter_direction = rec.normal;
        }
        
        scattered = Ray(rec.p, scatter_direction, r_in.time());
        attenuation = texture_->value(rec.u, rec.v, rec.p);
        return true;
    }

    // virtual double scattering_pdf(const Ray& r_in, const HitRecord& rec, const Ray& scattered) const override {
    //     auto cos_theta = dot(rec.normal, unit_vector(scattered.direction()));
    //     return cos_theta < 0 ? 0 : cos_theta / kPi;
    // }

  private:
    std::shared_ptr<Texture> texture_;
};


class Metal : public Material {
  public:
    Metal(const Color& a, double fuzz) : albedo_(a), fuzz_(ffmin(1, fuzz)) {}

    virtual bool scatter(const Ray& r_in, const HitRecord& rec, Color& attenuation, Ray& scattered) const override {
        Vec3 reflected = reflect(r_in.direction(), rec.normal);
        reflected = unit_vector(reflected) + fuzz_ * random_unit_vector();
        scattered = Ray(rec.p, reflected, r_in.time());
        attenuation = albedo_;
        return (dot(reflected, rec.normal) > 0);
    }

  private:
    Color albedo_;
    double fuzz_;
};


class Dielectric : public Material {
  public:
    Dielectric(double refractive_index) : refraction_index_(refractive_index) {}

    virtual bool scatter (const Ray& r_in, const HitRecord& rec, Color& attenuation, Ray& scattered) const override {
        attenuation = Color(1.0, 1.0, 1.0);
        double refraction_ratio = rec.front_face ? (1.0 / refraction_index_) : refraction_index_;

        Vec3 unit_direction = unit_vector(r_in.direction());
        
        double cos_theta = ffmin(dot(-unit_direction, rec.normal), 1.0);
        double sin_theta = sqrt(1.0 - cos_theta * cos_theta);
        
        bool cannot_refract = refraction_ratio * sin_theta > 1.0;
        Vec3 direction;
        
        if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double()) {
            direction = reflect(unit_direction, rec.normal);
        } else {
            direction = refract(unit_direction, rec.normal, refraction_ratio);
        }

        scattered = Ray(rec.p, direction, r_in.time());
        return true;
    }
  private:
    // Refractive index in vacuum or air, or the ratio of the material's refractive index over
    // the refractive index of the enclosing media
    double refraction_index_;

    static double reflectance(double cosine, double refraction_index) {
        // Use Schlick's approximation for reflectance.
        auto r0 = (1 - refraction_index) / (1 + refraction_index);
        r0 = r0 * r0;
        return r0 + (1 - r0) * pow((1 - cosine), 5);
    }
};

class DiffuseLight : public Material {
  public:
    DiffuseLight(std::shared_ptr<Texture> texture) : texture_(texture) {}
    DiffuseLight(const Color& emit) : texture_(std::make_shared<SolidColor>(emit)) {}

    virtual bool scatter(const Ray& r_in, const HitRecord& rec, Color& attenuation, Ray& scattered) const override {
        return false;
    }

    Color emitted(double u, double v, const Point3& p) const override {
        return texture_->value(u, v, p);
    }

  private:
    std::shared_ptr<Texture> texture_;
};

class Isotropic : public Material {
  public:
    Isotropic(const Color& albedo) : texture_(std::make_shared<SolidColor>(albedo)) {}
    Isotropic(std::shared_ptr<Texture> texture) : texture_(texture) {}

    virtual bool scatter(const Ray& r_in, const HitRecord& rec, Color& attenuation, Ray& scattered) const override {
        scattered = Ray(rec.p, random_unit_vector(), r_in.time());
        attenuation = texture_->value(rec.u, rec.v, rec.p);
        return true;
    }

  private:
    std::shared_ptr<Texture> texture_;
};