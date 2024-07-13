//
// Created by atwbzhang on 2024/7/13.
//
#pragma once
// Disable strict warnings for this header from the Microsoft Visual C++ compiler.
#ifdef _MSC_VER
    #pragma warning (push, 0)
#endif

#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include "stb_image.h"

#include <cstdlib>
#include <iostream>

#include "common.h"


class Image {
  public:
    Image() {}

    Image(const std::string& image_filename) {
        // Loads image data from the specified file. If the RTW_IMAGES environment variable is
        // defined, looks only in that directory for the image file. If the image was not found,
        // searches for the specified image file first from the current directory, then in the
        // images/ subdirectory, then the _parent's_ images/ subdirectory, and then _that_
        // parent, on so on, for six levels up. If the image was not loaded successfully,
        // width() and height() will return 0.
        
        if (load(image_filename)) return;

        std::cerr << "ERROR: Could not load image file '" << image_filename << "'.\n";
    }
    
    ~Image() {
        delete[] bdata_;
        STBI_FREE(fdata_);   
    }

    bool load(const std::string& filename) {
        // Loads the linear (gamma=1) image data from the given file name. Returns true if the
        // load succeeded. The resulting data buffer contains the three [0.0, 1.0]
        // floating-point values for the first pixel (red, then green, then blue). Pixels are
        // contiguous, going left to right for the width of the image, followed by the next row
        // below, for the full height of the image.

        auto n = bytes_per_pixel_;
        fdata_ = stbi_loadf(filename.c_str(), &image_width_, &image_height_, &n, bytes_per_pixel_);
        if (fdata_ == nullptr) {
            return false;
        }

        bytes_per_scanline_ = bytes_per_pixel_ * image_width_;
        convert2bytes();
        return true;
    }

    int width() const { return (fdata_ == nullptr) ? 0 : image_width_; }
    int height() const { return (fdata_ == nullptr) ? 0 : image_height_; }

    const unsigned char* pixel_data(int x, int y) const {
        // Return the address of the three RGB bytes of the pixel at x,y. If there is no image
        // data, returns magenta.
        static unsigned char magenta[3] = {255, 0, 255};
        if (bdata_ == nullptr) {
            return magenta;
        }
        x = clamp(x, 0, image_width_);
        y = clamp(y, 0, image_height_);
        return bdata_ + (y * bytes_per_scanline_) + (x * bytes_per_pixel_);
    }

  private:
    const int bytes_per_pixel_ = 3;
    float* fdata_ = nullptr;
    unsigned char* bdata_ = nullptr;
    int image_width_ = 0;
    int image_height_ = 0;
    int bytes_per_scanline_ = 0;

    static int clamp(int x, int low, int high) {
        // Return the value clamped to the range [low, high).
        if (x < low) {
            return low;
        }
        if (x < high) {
            return x;
        }
        return high - 1;
    }

    static unsigned char float2byte(float value) {
        // Convert a float in the range [0, 1) to an unsigned char in the range [0, 255].
        if (value <= 0.0) {
            return 0;
        }
        if (value >= 1.0) {
            return 255;
        }
        return static_cast<unsigned char>(value * 256);
    }

    void convert2bytes() {
        // Convert the linear floating point pixel data to bytes, storing the resulting byte
        // data in the `bdata` member.
        int total_bytes = image_width_ * image_height_ * bytes_per_pixel_;
        bdata_ = new unsigned char[total_bytes];

        // Iterate through all pixel components, converting from [0.0, 1.0] float values to
        // unsigned [0, 255] byte values.

        auto *bptr = bdata_;
        auto *fptr = fdata_;
        for (auto i = 0; i < total_bytes; i++) {
            *bptr++ = float2byte(*fptr++);
        }
    }
    
};

// Restore MSVC compiler warnings
#ifdef _MSC_VER
    #pragma warning (pop)
#endif
