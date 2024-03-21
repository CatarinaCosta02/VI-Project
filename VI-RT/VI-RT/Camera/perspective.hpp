
// perspective.hpp
#ifndef perspective_hpp
#define perspective_hpp

#include "camera.hpp"
#include "ray.hpp"
#include "vector.hpp"

class Perspective: public Camera {
    Point Eye, At;
    Vector Up;
    float fovW, fovH;
    int W, H;
    float c2w[3][3]; // camera 2 world transform
public:
    // Constructor
    Perspective(const Point Eye, const Point At, const Vector Up, const int W, const int H, const float fovW, const float fovH);

    // Method to generate a ray from the camera
    bool GenerateRay(const int x, const int y, Ray *r, const float *cam_jitter = nullptr);

    // Method to get the resolution
    void getResolution(int *_W, int *_H) {
        *_W = W;
        *_H = H;
    }
    void Information();
};

#endif /* perspective_hpp */