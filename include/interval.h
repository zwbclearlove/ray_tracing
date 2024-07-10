//
// Created by atwbzhang on 2024/7/10.
//
#pragma once
#include <limits>

constexpr double kInfinity = std::numeric_limits<double>::max();

class Interval {
  public:
    Interval() : min_(-kInfinity), max_(kInfinity) {}
    Interval(double min, double max) : min_(min), max_(max) {}

    inline double size() const {
        return max_ - min_;
    }

    inline bool contains(double t) const {
        return t >= min_ && t <= max_;
    }

    inline bool surrounds(double t) const {
        return t > min_ && t < max_;
    }

    inline double clamp(double x) const {
        if (x < min_) return min_;
        if (x > max_) return max_;
        return x;
    }

    static const Interval empty, universe;

    const double min() const { return min_; }
    const double max() const { return max_; }

  private:
    double min_;
    double max_;
};

const Interval Interval::empty(kInfinity, -kInfinity);
const Interval Interval::universe(-kInfinity, kInfinity);