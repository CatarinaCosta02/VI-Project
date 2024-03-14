#ifndef triangle_hpp
#define triangle_hpp

#include "geometry.hpp"
#include "vector.hpp"
#include <cmath>

class Triangle: public Geometry {
public:
    Point v1, v2, v3;
    Vector normal;           // geometric normal
    Vector edge1, edge2;
    BB bb;      // face bounding box
                // this is min={0.,0.,0.} , max={0.,0.,0.} due to the Point constructor
    bool intersect(Ray r, Intersection *isect);
    bool isInside(Point p);
    
    Triangle(Point _v1, Point _v2, Point _v3, Vector _normal): v1(_v1), v2(_v2), v3(_v3), normal(_normal) {
        edge1 = v1.vec2point(v2);
        edge2 = v1.vec2point(v3);
        bb.min.set(v1.X, v1.Y, v1.Z);
        bb.max.set(v1.X, v1.Y, v1.Z);
        bb.update(v2);
        bb.update(v3);
    }
    
    // Heron's formula to calculate the area of the triangle
    float area() {
        float a = edge1.length();
        float b = edge2.length();
        float c = (v2 - v3).length(); // Third edge, calculated from vertices

        // Calculate semiperimeter
        float s = (a + b + c) / 2.0f;

        // Heron's formula
        float A = sqrt(s * (s - a) * (s - b) * (s - c));

        return A;
    }

    // Heron's formula to calculate the area of a triangle formed by three given points
    float points_area(Point v1, Point v2, Point v3) {
        // Calculate lengths of edges
        float a = (v1 - v2).length();
        float b = (v1 - v3).length();
        float c = (v2 - v3).length();

        // Calculate semiperimeter
        float s = (a + b + c) / 2.0f;

        // Heron's formula
        float A = sqrt(s * (s - a) * (s - b) * (s - c));

        return A;
    }
};

#endif /* triangle_hpp */
