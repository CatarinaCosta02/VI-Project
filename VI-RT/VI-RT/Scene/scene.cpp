//
//  Scene.cpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 30/01/2023.
//

#include "scene.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include "primitive.hpp"
#include "mesh.hpp"
#include "Phong.hpp"

#include <iostream>
#include <set>
#include <vector>
#include "AreaLight.hpp"
#include <algorithm> // Para std::reverse


using namespace tinyobj;

static void PrintInfo(const ObjReader myObj)
{
    const tinyobj::attrib_t attrib = myObj.GetAttrib();
    const std::vector<tinyobj::shape_t> shps = myObj.GetShapes();
    const std::vector<tinyobj::material_t> materials = myObj.GetMaterials();
    std::cout << std::endl;
    std::cout << "# of vertices  : " << (attrib.vertices.size() / 3) << std::endl;
    std::cout << "# of normals   : " << (attrib.normals.size() / 3) << std::endl;
    std::cout << "# of texcoords : " << (attrib.texcoords.size() / 2) << std::endl;

    std::cout << "# of shapes    : " << shps.size() << std::endl;
    std::cout << "# of materials : " << materials.size() << std::endl;

    // Iterate shapes
    auto it_shape = shps.begin();
    for (; it_shape != shps.end(); it_shape++)
    {
        // assume each face has 3 vertices
        std::cout << std::endl;
        std::cout << "Processing shape " << it_shape->name << std::endl;
        // iterate faces
        // assume each face has 3 vertices
        auto it_vertex = it_shape->mesh.indices.begin();
        for (; it_vertex != it_shape->mesh.indices.end();)
        {
            // process 3 vertices
            for (int v = 0; v < 3; v++)
            {
                std::cout << it_vertex->vertex_index;
                it_vertex++;
            }
            std::cout << std::endl;
        }
        printf("There are %lu material indexes\n", it_shape->mesh.material_ids.size());
    }
}

/*
 Use tiny load to load .obj scene descriptions
 https://github.com/tinyobjloader/tinyobjloader
 */



bool Scene::Load(const std::string &fname)
{
    ObjReader myObj;
    int FaceID = 0;

    if (!myObj.ParseFromFile(fname))
    {
        std::cout << "Error loading file: " << fname << std::endl;
        return false;
    }
    else
        std::cout << "Successfully loaded file: " << fname << std::endl;

    std::cout << std::endl;

    const std::vector<tinyobj::material_t> materials = myObj.GetMaterials();
    for (auto it = materials.begin(); it != materials.end(); it++)
    {
        Phong *mat = new Phong;

        mat->Ka.R = it->ambient[0];
        mat->Ka.G = it->ambient[1];
        mat->Ka.B = it->ambient[2];

        mat->Kd.R = it->diffuse[0];
        mat->Kd.G = it->diffuse[1];
        mat->Kd.B = it->diffuse[2];

        mat->Ns = it->shininess;

        mat->Ks.R = it->specular[0];
        mat->Ks.G = it->specular[1];
        mat->Ks.B = it->specular[2];

        mat->Kt.R = it->transmittance[0];
        mat->Kt.G = it->transmittance[1];
        mat->Kt.B = it->transmittance[2];

        this->BRDFs.push_back(mat);
        this->numBRDFs++;
    }

    tinyobj::attrib_t attrib = myObj.GetAttrib();
    const float *vtcs = attrib.vertices.data();
    const std::vector<tinyobj::shape_t> shps = myObj.GetShapes();

    for (size_t s = 0; s < shps.size(); s++) {
        Primitive *p = new Primitive;
        Mesh *m = new Mesh;
        p->g = m;
        p->material_ndx = shps[s].mesh.material_ids[0];
        p->material_name = materials[p->material_ndx].name;
        p->object_name = shps[s].name;

        const int V1st = shps[s].mesh.indices.begin()->vertex_index * 3;
        m->bb.min.set(vtcs[V1st], vtcs[V1st + 1], vtcs[V1st + 2]);
        m->bb.max.set(vtcs[V1st], vtcs[V1st + 1], vtcs[V1st + 2]);

        struct rehash{
            int objNdx;
            int ourNdx;

            bool operator<(const rehash &other) const{
                return objNdx < other.objNdx;
            }
        };

        std::set<rehash> vert_rehash;

        for (auto v_it = shps[s].mesh.indices.begin(); v_it != shps[s].mesh.indices.end();) {
            Face *f = new Face;
            Point myVtcs[3];
            
            for (int v = 0; v < 3; v++) {
                const int objNdx = v_it->vertex_index;
                myVtcs[v].set(vtcs[objNdx * 3], vtcs[objNdx * 3 + 1], vtcs[objNdx * 3 + 2]);
                
                if (v == 0) {
                    f->bb.min.set(myVtcs[0].X, myVtcs[0].Y, myVtcs[0].Z);
                    f->bb.max.set(myVtcs[0].X, myVtcs[0].Y, myVtcs[0].Z);
                } else {
                    f->bb.update(myVtcs[v]);
                }
                
                rehash new_vert={objNdx, 0};
                auto known_vert = vert_rehash.find(new_vert);
                
                if (known_vert == vert_rehash.end()) { 
                    new_vert.ourNdx = m->numVertices;
                    vert_rehash.insert(new_vert);
                    m->vertices.push_back(myVtcs[v]);
                    m->numVertices++;
                    f->vert_ndx[v] = new_vert.ourNdx;
                    m->bb.update(myVtcs[v]);
                } else {
                    f->vert_ndx[v] = known_vert->ourNdx;
                }
                v_it++;
            }

            Point v0 = m->vertices[f->vert_ndx[0]];
            Point v1 = m->vertices[f->vert_ndx[1]];
            Point v2 = m->vertices[f->vert_ndx[2]];

            Vector edge1 = Vector(v1.X - v0.X, v1.Y - v0.Y, v1.Z - v0.Z);
            Vector edge2 = Vector(v2.X - v0.X, v2.Y - v0.Y, v2.Z - v0.Z);

            Vector normal = edge1.cross(edge2);
            normal.normalize();
            f->geoNormal.set(normal);
            f->FaceID = FaceID++;

            m->faces.push_back(*f);
            m->numFaces++;
        }
        
        prims.push_back(p);
        numPrimitives++;
    }

    // Swap the order of the primitives at positions 2 and 3
    std::iter_swap(prims.begin() + 5, prims.begin() + 6);
    std::reverse(prims.begin(), prims.end());

    for (auto prim_itr = prims.begin() ; prim_itr != prims.end() ; prim_itr++) {
        std::cout << "Primitiva - " << (*prim_itr)->object_name << std::endl;
        // std::cout << "Number of Vertices: " << m->numVertices << std::endl;
        // std::cout << "Number of Faces: " << m->numFaces << std::endl;
        std::cout << "Material name: " << (*prim_itr)->material_name << " (index: " << (*prim_itr)->material_ndx << ")" << std::endl;
        // std::cout << "Bounding Box Min: (" << m->bb.min.X << ", " << m->bb.min.Y << ", " << m->bb.min.Z << ")" << std::endl;
        // std::cout << "Bounding Box Max: (" << m->bb.max.X << ", " << m->bb.max.Y << ", " << m->bb.max.Z << ")" << std::endl;
        std::cout << std::endl;   
    }

    PrintInfo(myObj);  
    return true;
}



bool Scene::trace (Ray r, Intersection *isect) {
    Intersection curr_isect;
    bool intersection = false;    
    
    if (numPrimitives==0) 
        return false;
    
    // iterate over all primitives
    for (auto prim_itr = prims.begin() ; prim_itr != prims.end() ; prim_itr++) {
        if ((*prim_itr)->g->intersect(r, &curr_isect)) {
            if (!intersection) { // first intersection
                intersection = true;
                *isect = curr_isect;
                isect->f = BRDFs[(*prim_itr)->material_ndx];
            }
            else if (curr_isect.depth < isect->depth) {
                *isect = curr_isect;
                isect->f = BRDFs[(*prim_itr)->material_ndx];
            }
        }
    }
    isect->isLight = false;

    // now iterate over light sources and intersect with those that have geometry
    for (auto l = lights.begin() ; l != lights.end() ; l++) {
        if ((*l)->type == AREA_LIGHT) {
            AreaLight *al = (AreaLight *)(*l);
            if (al->gem->intersect(r, &curr_isect)) {
                if (!intersection) { // first intersection
                    intersection = true;
                    *isect = curr_isect;
                    isect->isLight = true;
                    isect->Le = al->L();
                }
                else if (curr_isect.depth < isect->depth) {
                    *isect = curr_isect;
                    isect->isLight = true;
                    isect->Le = al->L();
                }
            }
        }
    }
    return intersection;
}


// checks whether a point on a light source (distance maxL) is visible
bool Scene::visibility(Ray s, const float maxL)
{
    bool visible = true;
    Intersection curr_isect;

    if (numPrimitives == 0)
        return true;

    // iterate over all primitives while visible
    for (auto p = prims.begin(); p != prims.end() && visible; p++)
    {
        if ((*p)->g->intersect(s, &curr_isect))
        {
            if (curr_isect.depth < maxL)
            {
                visible = false;
            }
        }
    }
    return visible;
}
