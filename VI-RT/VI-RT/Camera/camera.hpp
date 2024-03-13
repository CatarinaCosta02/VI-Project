//
//  camera.hpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 10/02/2023.
//

// Now includes a constructor that takes parameters for the camera's position (eye), look-at point (at), up direction (up), 
// field of view (fov), image width (width), and image height (height).
// The GenerateRay function is declared as virtual, suggesting that it needs to be implemented by derived classes. This function is 
// responsible for generating rays based on the camera parameters and screen coordinates.
// The getResolution function provides an interface to retrieve the camera's image resolution.

#ifndef camera_hpp
#define camera_hpp

#include "ray.hpp"

// based on pbrt book, sec 6.1, pag. 356
class Camera {
public:
    Camera(const Point& eye, const Point& at, const Vector& up, float fov, int width, int height)
        : Eye(eye), At(at), Up(up), fov(fov), W(width), H(height) {}
    virtual ~Camera() {}
    virtual bool GenerateRay(const int x, const int y, Ray* r, const float* cam_jitter = nullptr) { return false; }
    virtual void getResolution(int* _W, int* _H) { *_W = W; *_H = H; }

protected:
    Point Eye; // Camera position
    Point At; // Look-at point
    Vector Up; // Up direction
    float fov; // Field of view
    int W;     // Image width
    int H;     // Image height
};

#endif /* camera_hpp */
