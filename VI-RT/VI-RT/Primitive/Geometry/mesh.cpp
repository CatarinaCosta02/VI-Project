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
    // acessar os vertices da face
    Point p0 = vertices[f.vert_ndx[0]];
    Point p1 = vertices[f.vert_ndx[1]];
    Point p2 = vertices[f.vert_ndx[2]];

    // Point (float x, float y, float z):X(x),Y(y),Z(z){}
    //Vector edge1 = p1 - p0;
    Vector edge1(p1.X - p0.X, p1.Y - p0.Y, p1.Z - p0.Z);
    Vector edge2(p2.X - p0.X, p2.Y - p0.Y, p2.Z - p0.Z);
    Vector h = r.dir.cross(edge2);
    float a = edge1.dot(h);

    // verifica se o raio é paralelo ao triangulo
    if (a > -0.00001f && a < 0.00001f)
        return false;

    float inv_det = 1.0f / a;
    Vector s(r.o.X - p0.X, r.o.Y - p0.Y, r.o.Z - p0.Z);
    float u = inv_det * s.dot(h);

    // The coefficients must be non-negative and sum to 1, 
    if (u < 0.0f || u > 1.0f)
        return false;
    
    Vector q = s.cross(edge1);
    float v = inv_det * r.dir.dot(q);

    if (v < 0.0f || u + v > 1.0f)
        return false;
    
    // at this stage we can compute t to find out where the intersection point is on the line
    float t = inv_det * edge2.dot(q);

    if (t > 0.00001f) // ray intersection
    {
        // out_intersection_point = ray_origin + ray_vector * t;
        Point intersection_point = r.o + r.dir * t;

        // mandar informações para o isect
        isect->p = intersection_point;
        isect->gn = f.geoNormal;
        isect->sn = f.geoNormal;
        Vector wo = r.dir * -1.0f;
       // make sure the normal (gn and sn) points to the same side of the surface as w
        wo.normalize();
        isect->wo = wo;
        isect->depth = t;
        isect->FaceID = f.FaceID;

        return true;
    }
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
