//
//  Ray.hpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 30/01/2023.
//

#ifndef Ray_hpp
#define Ray_hpp

#include "vector.hpp"
#include <cfloat>

const float EPSILON=1e-3;

class Ray {
public:
    Point o; // ray origin
    Vector dir; // ray direction
    int FaceID;  // ID of the face where the origin lays in
    Vector invDir;  // ray direction reciprocal for intersections
    int pix_x, pix_y;
    // para a Cat: foram adicionados valores min e max do ray - são necessários para delimitar o "campo de visão" 
    // (sem eles, o programa percore uma direção infinita e não sabe "quando parar",
    // e assim a imagem aparece toda preta, porque fica com problemas a renderizar)
    float intersectionMin, intersectionMax;
    // para a Cat: foram adicionados os parametros da class ray na construção
    Ray() {}
    Ray(Point o, Vector d) : o(o), dir(d), FaceID(0), invDir(Vector(0, 0, 0)), pix_x(0), pix_y(0), intersectionMin(0.0), intersectionMax(FLT_MAX) {}
    Ray(Point o, Vector d, int FaceID, Vector invDir, int pix_x = 0, int pix_y = 0, float intersectionMin = 0.0, float intersectionMax = FLT_MAX)
        : o(o), dir(d), FaceID(FaceID), invDir(invDir), pix_x(pix_x), pix_y(pix_y), intersectionMin(intersectionMin), intersectionMax(intersectionMax) {}
    ~Ray() {}
    void adjustOrigin (Vector normal) {
        Vector offset = EPSILON * normal;
        if (dir.dot(normal) < 0)
            offset = -1.f * offset;
        o = o + offset;
        //o.X = o.X + offset.X;
        //o.Y = o.Y + offset.Y;
        //o.Z = o.Z + offset.Z;
    }
};

#endif /* Ray_hpp */
