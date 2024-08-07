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
    
    float length(const Point& p1, const Point& p2) const {
        return sqrt(pow(p2.X - p1.X, 2) + pow(p2.Y - p1.Y, 2) + pow(p2.Z - p1.Z, 2));
    }
    
    // Heron's formula
    // https://www.mathopenref.com/heronsformula.html
    float area () {
        
        const float A = length(this->v1, this->v2);
        const float B = length(this->v2, this->v3);
        const float C = length(this->v3, this->v1);
        float p = (A + B + C) / 2.0f;
        return sqrt(p * (p - A) * (p - B) * (p - C));

    }
    float points_area (Point v1, Point v2, Point v3) {
        
        const float A = length(v1, v2);
        const float B = length(v2, v3);
        const float C = length(v3, v1);
        float p = (A + B + C) / 2.0f;
        return sqrt(p * (p - A) * (p - B) * (p - C));

    }
};

#endif /* triangle_hpp */
