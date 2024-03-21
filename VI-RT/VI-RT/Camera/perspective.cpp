
//
//  perspective.cpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 10/02/2023.
//

#include "perspective.hpp"

bool Perspective::GenerateRay(const int x, const int y, Ray *r, const float *cam_jitter) {

    

    return false;
}











/*
// Constructor:
// Inherits from the Camera class, passing the provided parameters to the Camera constructor.
// Initializes the Perspective object with the specified parameters.

// computeCameraToWorld Method:
// Placeholder method to compute the camera-to-world matrix (c2w). This matrix represents the transformation from camera space 
// to world space. You should replace this placeholder with the actual computation based on the camera's position, orientation, 
// and field of view.

// GenerateRay Method:
// Converts pixel coordinates to normalized device coordinates (NDC).
// Computes the ray direction in camera space based on the normalized device coordinates.
// Transforms the ray direction from camera space to world space using the camera-to-world matrix (c2w).
// Normalizes the ray direction.
// Sets the ray's origin and direction and returns true.


// perspective.cpp
#include "perspective.hpp"
#include "vector.hpp"
#include "Ray.hpp" // Include the Ray header
#include "camera.hpp" // Include the Camera header
#include <iostream> // Include the iostream library for printing messages

// Constructor
Perspective::Perspective(const Point Eye, const Point At, const Vector Up, const int W, const int H, const float fovW, const float fovH)
    : Camera(Eye, At, Up, fovW, W, H), // Call the Camera constructor with the provided parameters
      Eye(Eye), At(At), Up(Up), W(W), H(H), fovW(fovW), fovH(fovH) {
    // Call the method to compute the camera-to-world matrix
    computeCameraToWorld();
}

// Method to compute the camera-to-world matrix
void Perspective::computeCameraToWorld() {
    // Compute the camera-to-world matrix based on the camera's position, orientation, and field of view
    // This is a placeholder implementation. You need to replace it with your actual computation.
    c2w[0][0] = 1.0f; c2w[0][1] = 0.0f; c2w[0][2] = 0.0f;
    c2w[1][0] = 0.0f; c2w[1][1] = 1.0f; c2w[1][2] = 0.0f;
    c2w[2][0] = 0.0f; c2w[2][1] = 0.0f; c2w[2][2] = 1.0f;
    // Print a message to indicate that Camara is complete
    std::cout << "Camara working." << std::endl;
}

// Method to generate a ray
bool Perspective::GenerateRay(const int x, const int y, Ray *r, const float *cam_jitter) {
    // Convert pixel coordinates to normalized device coordinates
     float ndcX = (2.0f * x - W) / W;
     float ndcY = (2.0f * y - H) / H;

    // cam_jitter (acho que temos já de fazer isto idk, tem a ver com o noise, já que estava em cima no Perspective::GenerateRay)
    
    /*bool Perspective::GenerateRay(const int x, const int y, Ray *r, const float *cam_jitter) {
    return false;
}

    
    // Compute the ray direction in camera space
    Vector dir(ndcX, ndcY, -1.0f); // Assuming the near plane is at z=-1

    // Transform the ray direction from camera space to world space
    Vector dirWorld(c2w[0][0] * dir.X + c2w[0][1] * dir.Y + c2w[0][2] * dir.Z,
                    c2w[1][0] * dir.X + c2w[1][1] * dir.Y + c2w[1][2] * dir.Z,
                    c2w[2][0] * dir.X + c2w[2][1] * dir.Y + c2w[2][2] * dir.Z);

    // Normalize the ray direction
    dirWorld.normalize();

    // Set the ray origin and direction
    r->o = Eye; // Use 'o' for origin
    r->dir = dirWorld; // Use 'dir' for direction
    // O prof pos isto:
    r->pix_x = x;
    r->pix_y = y;

    return true;
}
*/
