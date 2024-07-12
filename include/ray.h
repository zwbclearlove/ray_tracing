//
// Created by atwbzhang on 2024/7/10.
//
#pragma once
#include "vec3.h"

class Ray {
  public:
    Ray() {}

    Ray(const Point3& origin, const Vec3& direction) 
    : origin_(origin), direction_(direction), time_(0) {}
    Ray(const Point3& origin, const Vec3& direction, double time) 
    : origin_(origin), direction_(direction), time_(time) {}

    const Point3& origin() const {
        return origin_;
    }
    const Vec3& direction() const {
        return direction_;
    }
    const double time() const {
        return time_;
    }

    Point3 at(double t) const {
        return origin_ + t * direction_;
    }

  private:
    Point3 origin_;
    Vec3 direction_;
    double time_;
};
