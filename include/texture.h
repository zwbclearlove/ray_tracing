//
// Created by atwbzhang on 2024/7/13.
//
#pragma once
#include "common.h"
#include "color.h"
#include "vec3.h"
#include "image.h"

class Texture {
  public:
    virtual ~Texture() = default;

    virtual Color value(double u, double v, const Point3& p) const = 0;
};

class SolidColor : public Texture {
  public:
    SolidColor(const Color& albedo) : albedo_(albedo) {}
    SolidColor(double red, double green, double blue) : SolidColor(Color(red, green, blue)) {}

    Color value(double u, double v, const Point3& p) const override {
        return albedo_;
    }

  private:
    Color albedo_;
};

class CheckerTexture : public Texture {
  public:
    CheckerTexture(double scale, std::shared_ptr<Texture> even, std::shared_ptr<Texture> odd)
    : inv_scale_(1.0 / scale), even_(even), odd_(odd) {}

    CheckerTexture(double scale, const Color& c1, const Color& c2)
    : CheckerTexture(scale, std::make_shared<SolidColor>(c1), std::make_shared<SolidColor>(c2)) {}

    Color value(double u, double v, const Point3& p) const override {
      auto x_int = static_cast<int>(std::floor(inv_scale_ * p.x()));
      auto y_int = static_cast<int>(std::floor(inv_scale_ * p.y()));
      auto z_int = static_cast<int>(std::floor(inv_scale_ * p.z()));

      bool is_even = (x_int + y_int + z_int) % 2 == 0;

      return is_even ? even_->value(u, v, p) : odd_->value(u, v, p);
    }

  private:
    double inv_scale_; 
    std::shared_ptr<Texture> even_;
    std::shared_ptr<Texture> odd_;
};

class ImageTexture : public Texture {
  public:
    ImageTexture(const char* filename) : image_(filename) {}

    Color value(double u, double v, const Point3& p) const override {
        // If we have no texture data, then return solid cyan as a debugging aid.
        if (image_.height() <= 0) {
          return Color(0, 1, 1);
        }

        // Clamp input texture coordinates to [0,1] x [1,0]
        u = Interval(0, 1).clamp(u);
        v = 1.0 - Interval(0, 1).clamp(v);

        auto i = static_cast<int>(u * image_.width());
        auto j = static_cast<int>(v * image_.height());

        auto pixel = image_.pixel_data(i, j);

        auto color_scale = 1.0 / 255.0;
        return Color(color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2]);
    }

  private:
    Image image_;
};