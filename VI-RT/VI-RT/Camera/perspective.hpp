
// // perspective.hpp
// #ifndef perspective_hpp
// #define perspective_hpp

// #include "camera.hpp"
// #include "ray.hpp"
// #include "vector.hpp"

// class Perspective: public Camera {
//     Point Eye, At;
//     Vector Up;
//     float fovW, fovH;
//     int W, H;
//     float c2w[3][3]; // camera 2 world transform
// public:
//     // Constructor
//     Perspective(const Point Eye, const Point At, const Vector Up, const int W, const int H, const float fovW, const float fovH);

//     // Method to generate a ray from the camera
//     bool GenerateRay(const int x, const int y, Ray *r, const float *cam_jitter = nullptr);
//     Ray GenerateRay2(const int x, const int y, Ray *r);

//     // Method to get the resolution
//     void getResolution(int *_W, int *_H) {
//         *_W = W;
//         *_H = H;
//     }
//     void Information();
// };

// #endif /* perspective_hpp */


//
//  perspective.hpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 10/02/2023.
//

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
    float dist;
    Vector forward, right, up;
    float c2w[3][3];  // camera 2 world transform
public:
    Perspective (Point Eye, Point At, Vector Up, int W, int H, float fovW, float fovH);
    bool GenerateRay(int x, int y, Ray *r, const float *cam_jitter = nullptr);
    void getResolution(int *outW, int *outH) {
        *outW = W;
        *outH = H;
    }
    void Information();
};

#endif /* perspective_hpp */
