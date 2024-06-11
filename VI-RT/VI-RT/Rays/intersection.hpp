#ifndef Intersection_hpp
#define Intersection_hpp

#pragma once
#include "vector.hpp"
#include "BRDF.hpp"

class Intersection {
public:
    Point p;
    Vector gn;  // geometric normal
    Vector sn;  // shading normal (the same as gn for the time being)
    Vector wo;
    float depth;
    BRDF *f;
    int pix_x, pix_y;
    int FaceID;  // ID of the intersected face 
    bool isLight;  // for intersections with light sources
    RGB Le;         // for intersections with light sources
    
    // Constructor with initialization
    Intersection(const Point& p, const Vector& n, const Vector& wo, float depth, BRDF* f, int pix_x, int pix_y, int FaceID, bool isLight, const RGB& Le)
        : p(p), gn(n), sn(n), wo(wo), depth(depth), f(f), pix_x(pix_x), pix_y(pix_y), FaceID(FaceID), isLight(isLight), Le(Le) {}

    // Default constructor
    Intersection() : f(nullptr) {}
};
#endif /* Intersection_hpp */
