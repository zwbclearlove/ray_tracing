//
// Created by atwbzhang on 2024/7/10.
//
#pragma once
#include "common.h"
#include "ray.h"
#include "color.h"
#include "vec3.h"

class HitRecord;

class Material {
  public:
    virtual bool scatter(const Ray& r_in, const HitRecord& rec, Color& attenuation, Ray& scattered) const = 0;
};

