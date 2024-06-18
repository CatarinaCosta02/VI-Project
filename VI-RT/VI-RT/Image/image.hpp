// image.hpp

// Represents a basic image with RGB colors.
// Includes functions for setting and adding colors to pixels.
// Provides a basic save function (though it's not fully implemented).
#ifndef IMAGE_HPP
#define IMAGE_HPP

#include "RGB.hpp"
#include <string>

class Image {
protected:
    RGB *imagePlane;
    int W, H;

public:
    Image() : W(0), H(0), imagePlane(nullptr) { /* empty image */ }

    Image(const int _W, const int _H, const RGB &initialColor = RGB(0, 0, 0)) : W(_W), H(_H) {
        imagePlane = new RGB[W * H];
        for (int i = 0; i < W * H; ++i) {
            imagePlane[i] = initialColor;
        }
    }

    ~Image() {
        delete[] imagePlane;
    }

    // troquei os "or" porque davam erro
    bool set(int x, int y, const RGB &rgb) {
        if (x >= 0 && x < W && y >= 0 && y < H) {
            imagePlane[y * W + x] = rgb;
            return true;
        }
        return false;
    }

    bool add(int x, int y, const RGB &rgb) {
        if (x >= 0 && x < W && y >= 0 && y < H) {
            imagePlane[y * W + x] += rgb;
            return true;
        }
        return false;
    }

    bool Save(std::string filename) {
        // Placeholder for saving logic
        return true;
    }
};

#endif /* IMAGE_HPP */
