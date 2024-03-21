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

using namespace tinyobj;

static void PrintInfo(const ObjReader myObj)
{
    const tinyobj::attrib_t attrib = myObj.GetAttrib();
    const std::vector<tinyobj::shape_t> shps = myObj.GetShapes();
    const std::vector<tinyobj::material_t> materials = myObj.GetMaterials();
    std::cout << "# of vertices  : " << (attrib.vertices.size() / 3) << std::endl;
    std::cout << "# of normals   : " << (attrib.normals.size() / 3) << std::endl;
    std::cout << "# of texcoords : " << (attrib.texcoords.size() / 2)
              << std::endl;

    std::cout << "# of shapes    : " << shps.size() << std::endl;
    std::cout << "# of materials : " << materials.size() << std::endl;

    // Iterate shapes
    auto it_shape = shps.begin();
    for (; it_shape != shps.end(); it_shape++)
    {
        // assume each face has 3 vertices
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
        std::cout << std::endl;

        printf("There are %lu material indexes\n", it_shape->mesh.material_ids.size());
    }
}

/*
 Use tiny load to load .obj scene descriptions
 https://github.com/tinyobjloader/tinyobjloader
 */

bool Scene::Load(const std::string &fname)
{
    ObjReader myObjReader;
    int FaceID = 0;

    ObjReader myObj;
    if (!myObj.ParseFromFile(fname))
    {
        return false;
    }

    PrintInfo(myObjReader);
    // get the materials
    const std::vector<tinyobj::material_t> materials = myObj.GetMaterials();

    for (auto it = materials.begin(); it != materials.end(); it++)
    {
        // criação de um objeto Phong para cada material
        Phong *mat = new Phong;

        // Ka
        mat->Ka.R = it->ambient[0];
        mat->Ka.G = it->ambient[1];
        mat->Ka.B = it->ambient[2];

        // Kd (diffuse color)
        mat->Kd.R = it->diffuse[0];
        mat->Kd.G = it->diffuse[1];
        mat->Kd.B = it->diffuse[2];

        // Ns (specular exponent/shininess)
        mat->Ns = it->shininess;

        // Ks (specular color)
        mat->Ks.R = it->specular[0];
        mat->Ks.G = it->specular[1];
        mat->Ks.B = it->specular[2];

        // Kt (transmission color)
        mat->Kt.R = it->transmittance[0];
        mat->Kt.G = it->transmittance[1];
        mat->Kt.B = it->transmittance[2];

        // adicionar o material à lista de BRDFs, que é usada para modelar como a luz é refletida em superfícies
        BRDFs.push_back(mat);
        numBRDFs++;
    }

    // ter acesso aos vértices e à mesh

    // get the vertices
    //const tinyobj::attrib_t attrib = myObj.GetAttrib();
    //const float *vtcs = attrib.vertices.data();
    //const std::vector<tinyobj::shape_t> shapes = myObj.GetShapes();
        // get the vertices
    const tinyobj::attrib_t attrib = myObj.GetAttrib();
    const float *vtcs = attrib.vertices.data();

    // access the shapes (each shape is one mesh)
    const std::vector<tinyobj::shape_t> shps = myObj.GetShapes();
    for (auto shp = shps.begin(); shp != shps.end(); shp++){
        Primitive *p = new Primitive;
        Mesh *m = new Mesh;
        p->g = m;
        // assume all faces in the mesh have the same material
        p->material_ndx = shp->mesh.material_ids[0];

        // the primitive's geometry bounding box is computed on the fly
        // initially set BB.min and BB.max to be the first vertex
        const int V1st = shp->mesh.indices.begin()->vertex_index * 3;
        m->bb.min.set(vtcs[V1st], vtcs[V1st + 1], vtcs[V1st + 2]);
        m->bb.max.set(vtcs[V1st], vtcs[V1st + 1], vtcs[V1st + 2]);

        // estrutura rehash (não existia em nenhum lado do código)
        struct rehash{
            int objNdx;
            int ourNdx;

            bool operator<(const rehash &other) const{
            // Define your comparison logic here. For example:
                return objNdx < other.objNdx;
            }
        };

        // add faces and vertices
        std::set<rehash> vert_rehash;
        for (auto v_it = shp->mesh.indices.begin(); v_it != shp->mesh.indices.end();){
            Face *f = new Face;
            Point myVtcs[3];
            // process 3 vertices
            for (int v = 0; v < 3; v++){
                const int objNdx = v_it->vertex_index;
                myVtcs[v].set(vtcs[objNdx * 3], vtcs[objNdx * 3 + 1], vtcs[objNdx * 3 + 2]);
                if (v == 0){
                    f->bb.min.set(myVtcs[0].X, myVtcs[0].Y, myVtcs[0].Z);
                    f->bb.max.set(myVtcs[0].X, myVtcs[0].Y, myVtcs[0].Z);
                }else{
                        f->bb.update(myVtcs[v]);
                    }
                rehash new_vert={objNdx, 0};
                auto known_vert = vert_rehash.find(new_vert);
                if (known_vert == vert_rehash.end()){ // new vertice, add it to the mesh
                        new_vert.ourNdx = m->numVertices;
                        vert_rehash.insert(new_vert);
                        m->vertices.push_back(myVtcs[v]);
                        m->numVertices++;
                        // register in the face
                        f->vert_ndx[v] = new_vert.ourNdx;
                        m->bb.update(myVtcs[v]);
                    }
                    else
                        f->vert_ndx[v] = known_vert->ourNdx;
                    v_it++; // next vértice within this face (there are 3)
                }           //    end vertices

            // add face to mesh: compute the geometric normal
            Vector v1 = myVtcs[0].vec2point(myVtcs[1]);
            Vector v2 = myVtcs[0].vec2point(myVtcs[2]);
            f->edge1 = v1;f->edge2 = v2;
            Vector normal = v1.cross(v2);
            normal.normalize();
            f->geoNormal.set(normal);
            f->FaceID = FaceID++;
            // add face to mesh
            m->faces.push_back(*f);
            m->numFaces++;
            } // end iterate vértices in the mesh (shape)
            // add primitive to scene
            prims.push_back(p);
            numPrimitives++;
        } // end iterate over shapes
        return true;
    }

bool Scene::trace(Ray r, Intersection *isect)
{
    Intersection curr_isect;
    bool intersection = false;

    if (numPrimitives == 0)
        return false;

    // iterate over all primitives
    for (auto prim_itr = prims.begin(); prim_itr != prims.end(); prim_itr++)
    {
        if ((*prim_itr)->g->intersect(r, &curr_isect))
        {
            if (!intersection)
            { // first intersection
                intersection = true;
                *isect = curr_isect;
                isect->f = BRDFs[(*prim_itr)->material_ndx];
            }
            else if (curr_isect.depth < isect->depth)
            {
                *isect = curr_isect;
                isect->f = BRDFs[(*prim_itr)->material_ndx];
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
    for (auto prim_itr = prims.begin(); prim_itr != prims.end() && visible; prim_itr++)
    {
        if ((*prim_itr)->g->intersect(s, &curr_isect))
        {
            if (curr_isect.depth < maxL)
            {
                visible = false;
            }
        }
    }
    return visible;
}
