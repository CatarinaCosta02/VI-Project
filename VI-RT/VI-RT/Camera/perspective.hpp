// Constructor:
// Takes parameters for the camera's eye position (Eye), look-at point (At), up vector (Up), image width (W), image height (H), 
// and field of view in the horizontal (fovW) and vertical (fovH) directions.
// Calls the constructor of the base class Camera with the provided parameters.
// Initializes member variables with the provided values.

// computeCameraToWorld Method:
// Declaration for the method responsible for computing the camera-to-world matrix.
// The actual implementation is provided in perspective.cpp.

// GenerateRay Method:
// Declaration for the method responsible for generating a ray from the camera.
// The actual implementation is provided in perspective.cpp.

// getResolution Method:
// Declaration for the method responsible for getting the resolution (width and height) of the camera.
// The actual implementation is missing, and it's something you might want to add based on your needs.

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

    // Method to compute the camera-to-world matrix
    void computeCameraToWorld();

    // Method to generate a ray from the camera
    bool GenerateRay(const int x, const int y, Ray *r, const float *cam_jitter = nullptr);

    // Method to get the resolution
    void getResolution(int *_W, int *_H) {
        *_W = W;
        *_H = H;
    }
};

#endif /* perspective_hpp */