//
//  mesh.cpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 05/02/2023.
//

#include "mesh.hpp"

// see pbrt book (3rd ed.), sec 3.6.2, pag 157
//
// Suggestion: use:
// // https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm
// Moller Trumbore intersection algorithm

#include <cfloat> // Add this include directive

bool Mesh::TriangleIntersect (Ray r, Face f, Intersection *isect) {

    // n * (p - p0) = 0, onde n é a normal da face, p é um ponto na face e p0 é um ponto na face
    // n = (v1 - v0) x (v2 - v0)
    Point p0 = vertices[f.vert_ndx[0]];
    Point p1 = vertices[f.vert_ndx[1]];
    Point p2 = vertices[f.vert_ndx[2]];

    Vector edge1 = p1 - p0;
    Vector edge2 = p2 - p0;
    Vector h = r.direction.cross(edge2);
    float a = edge1.dot(h);

    // verifica se o raio é paralelo ao triangulo
    if (a > -0.00001f && a < 0.00001f)
        return false;

    float f = 1.0f / a;
    Vector s = r.origin - p0;
    float u = f * s.dot(h);

    if (u < 0.0f || u > 1.0f)
        return false;
    
    Vector q = s.cross(edge1);
    

    return false;
}

bool Mesh::intersect (Ray r, Intersection *isect) {
    bool intersect = true, intersect_this_face;
    Intersection min_isect, curr_isect;
    float min_depth = FLT_MAX;
    // intersect the ray with the mesh BB
    if (!bb.intersect(r)) return false;
    
    // If it intersects then loop through the faces
    intersect = false;
    for (auto face_it=faces.begin() ; face_it != faces.end() ; face_it++) {
        intersect_this_face = TriangleIntersect(r, *face_it, &curr_isect);
        if (!intersect_this_face) continue;
        
        intersect = true;
        if (curr_isect.depth < min_depth) {  // this is closer
            min_depth = curr_isect.depth;
            min_isect = curr_isect;
        }
    }
    
    return intersect;
}
