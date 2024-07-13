//
// Created by atwbzhang on 2024/7/13.
//
#pragma once
#include "common.h"
#include "vec3.h"

class Perlin {
  public:
    Perlin() {
        rand_vec_ = new Vec3[point_count];
        for (int i = 0; i < point_count; ++i) {
            rand_vec_[i] = unit_vector(Vec3::random(-1, 1));
        }

        perm_x_ = perlin_generate_perm();
        perm_y_ = perlin_generate_perm();
        perm_z_ = perlin_generate_perm();
    }

    ~Perlin() {
        delete[] rand_vec_;
        delete[] perm_x_;
        delete[] perm_y_;
        delete[] perm_z_;
    }

    double noise(const Point3& p) const {
        auto u = p.x() - std::floor(p.x());
        auto v = p.y() - std::floor(p.y());
        auto w = p.z() - std::floor(p.z());

        auto i = static_cast<int>(std::floor(p.x()));
        auto j = static_cast<int>(std::floor(p.y()));
        auto k = static_cast<int>(std::floor(p.z()));

        Vec3 c[2][2][2];

        for (int di = 0; di < 2; ++di) {
            for (int dj = 0; dj < 2; ++dj) {
                for (int dk = 0; dk < 2; ++dk) {
                    c[di][dj][dk] = rand_vec_[
                        perm_x_[(i + di) & 255] ^ 
                        perm_y_[(j + dj) & 255] ^ 
                        perm_z_[(k + dk) & 255]
                    ];
                }
            }
        }
       
        return trilinear_interp(c, u, v, w);
    }

    double turb(const Point3& p, int depth = 7) const {
        auto accum = 0.0;
        auto temp_p = p;
        auto weight = 1.0;

        for (int i = 0; i < depth; ++i) {
            accum += weight * noise(temp_p);
            weight *= 0.5;
            temp_p *= 2;
        }

        return std::abs(accum);
    }

  private:
    static constexpr int point_count = 256;
    Vec3* rand_vec_;
    int* perm_x_;
    int* perm_y_;
    int* perm_z_;

    static int* perlin_generate_perm() {
        auto p = new int[point_count];

        for (int i = 0; i < point_count; ++i) {
            p[i] = i;
        }

        permute(p, point_count);

        return p;
    }

    static void permute(int* p, int n) {
        for (int i = n - 1; i > 0; i--) {
            int target = random_int(0, i);
            int tmp = p[i];
            p[i] = p[target];
            p[target] = tmp;
        }
    }

    static double trilinear_interp(const Vec3 c[2][2][2], double u, double v, double w) {
        auto uu = u * u * (3 - 2 * u);
        auto vv = v * v * (3 - 2 * v);
        auto ww = w * w * (3 - 2 * w);
        auto accum = 0.0;

        for (int i = 0; i < 2; ++i) {
            for (int j = 0; j < 2; ++j) {
                for (int k = 0; k < 2; ++k) {
                    Vec3 weight_v(u - i, v - j, w - k);
                    accum += (i * uu + (1 - i) * (1 - uu)) *
                             (j * vv + (1 - j) * (1 - vv)) *
                             (k * ww + (1 - k) * (1 - ww)) *
                             dot(c[i][j][k], weight_v);
                }
            }
        }

        return accum;
    }
};