//
// Created by atwbzhang on 2024/7/9.
//

#ifndef RAY_TRACING_RAY_H
#define RAY_TRACING_RAY_H

#include "vec3.h"

class Ray {
  public:
    Ray() {}

    Ray(const Point3& origin, const Vec3& direction) : origin_(origin), direction_(direction) {}

    const Point3& origin() const {
        return origin_;
    }
    const Vec3& direction() const {
        return direction_;
    }

    Point3 at(double t) const {
        return origin_ + t * direction_;
    }

  private:
    Point3 origin_;
    Vec3 direction_;
};

#endif //RAY_TRACING_RAY_H
