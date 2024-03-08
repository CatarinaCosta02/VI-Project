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

static void PrintInfo (const ObjReader myObj) {
    const tinyobj::attrib_t attrib = myObj.GetAttrib();
    const std::vector<tinyobj::shape_t> shapes = myObj.GetShapes(); // access the shapes (each shape is one mesh)
    const std::vector<tinyobj::material_t> materials = myObj.GetMaterials();
    std::cout << "# of vertices  : " << (attrib.vertices.size() / 3) << std::endl;
    std::cout << "# of normals   : " << (attrib.normals.size() / 3) << std::endl;
    std::cout << "# of texcoords : " << (attrib.texcoords.size() / 2)
              << std::endl;

    std::cout << "# of shapes    : " << shapes.size() << std::endl;
    std::cout << "# of materials : " << materials.size() << std::endl;
    
    // Iterate shapes
    auto it_shape = shapes.begin();
    for ( ; it_shape != shapes.end() ; it_shape++) {
        // assume each face has 3 vertices
        std::cout << "Processing shape " << it_shape->name << std::endl;
        // iterate faces
        // assume each face has 3 vertices
        auto it_vertex = it_shape->mesh.indices.begin();
        for ( ; it_vertex != it_shape->mesh.indices.end() ; ) {
            // process 3 vertices
            for (int v=0 ; v<3 ; v++) {
                std::cout << it_vertex->vertex_index;
                it_vertex++;
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
        
        printf("There are %lu material indexes\n", it_shape->mesh.material_ids.size());
    }

// iterate over shapes (meshes)
for (auto shp = shps.begin() ; shp != shps.end() ; shp++) {
        Primitive *p = new Primitive;
        Mesh *m = new Mesh;
        p->g = m;
        // assume all faces in the mesh have the same material
        p->material_ndx = shp->mesh.material_ids[0];

        // the primitive's geometry bounding box is computed on the fly
        // initially set BB.min and BB.max to be the first vertex
        const int V1st = shp->mesh.indices.begin()->vertex_index * 3;
        m->bb.min.set(vtcs[V1st], vtcs[V1st+1], vtcs[V1st+2]);
        m->bb.max.set(vtcs[V1st], vtcs[V1st+1], vtcs[V1st+2]);


    std::set<rehash> vert_rehash;
    for (auto v_it=shp->mesh.indices.begin(); v_it!=shp-> mesh.indices.end() ;) {
        Face *f = new Face;
        Point myVtcs[3];
        // process 3 vertices
        for (int v=0 ; v<3 ; v++) {
            const int objNdx = v_it->vertex_index;
            myVtcs[v].set(vtcs[objNdx*3], vtcs[objNdx*3+1], vtcs[objNdx*3+2]);
            if (v==0) {
                f->bb.min.set(myVtcs[0].X, myVtcs[0].Y, myVtcs[0].Z);
                f->bb.max.set (myVtcs[0].X, myVtcs[0].Y, myVtcs[0].Z);
            } else face->bb.update(myVtcs[v]);

            // add vertex to mesh if new
                rehash new_vert={objNdx, 0};
                auto known_vert = vert_rehash.find(new_vert);
                if (known_vert == vert_rehash.end()) { // new vertice, add it to the mesh
                    new_vert.ourNdx = m->numVertices;
                    vert_rehash.insert(new_vert);
                    m->vertices.push_back(myVtcs[v]);   m->numVertices++;
                    // register in the face
                    f->vert_ndx[v] = new_vert.ourNdx;   m->bb.update(myVtcs[v]);
                } else f->vert_ndx[v] = known_vert->ourNdx;
                v_it++;  // next vértice within this face (there are 3)
            //    end vertices

        }

}

/*
 Use tiny load to load .obj scene descriptions
 https://github.com/tinyobjloader/tinyobjloader
 */

bool Scene::Load (const std::string &fname) {
    ObjReader myObjReader;
    int FaceID = 0;

    ObjReader myObj;

    if (!myObjReader.ParseFromFile(fname)) {
        return false;
    }
    
    const vector<material_t> materials = myObj.GetMaterials();
    for (auto it = materials.begin(); it != materials.end() ; it+++) {
        Phong *mat = new Phong;
        // Ka
        mat->Ka.R = it->ambient[0];
        mat->Ka.B = it->ambient[1];
        mat->Ka.G = it->ambient[2];

        mat->Kd.R = it->ambient[0];
        mat->Kd.B = it->ambient[1];
        mat->Kd.G = it->ambient[2];

        mat->Ns.R = it->ambient[0];
        mat->Ns.B = it->ambient[1];
        mat->Ns.G = it->ambient[2];

        mat->Ks.R = it->ambient[0];
        mat->Ks.B = it->ambient[1];
        mat->Ks.G = it->ambient[2];

        mat->Kt.R = it->ambient[0];
        mat->Kt.B = it->ambient[1];
        mat->Kt.G = it->ambient[2];
        
        BRDFs.push_back (mat);
        numBRDFs++;
        
    }
    
    //PrintInfo (myObjReader);

    // convert loader's representation to my representation
    // temos de fazer o set para armazenar as coordenadas uma vez que nao se podem repetir
    
    // usar os loops debaixo e adaptar
    // guardar as primitivas no ficheiro scene.hpp no vetor 'prims'


    // Loop over shapes
    for (size_t s = 0; s < shapes.size(); s++) {
    // Loop over faces(polygon)
    size_t index_offset = 0;
    for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
        size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

        // Loop over vertices in the face.
        for (size_t v = 0; v < fv; v++) {
        // access to vertex
        tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
        tinyobj::real_t vx = attrib.vertices[3*size_t(idx.vertex_index)+0];
        tinyobj::real_t vy = attrib.vertices[3*size_t(idx.vertex_index)+1];
        tinyobj::real_t vz = attrib.vertices[3*size_t(idx.vertex_index)+2];

        // Check if `normal_index` is zero or positive. negative = no normal data
        if (idx.normal_index >= 0) {
            tinyobj::real_t nx = attrib.normals[3*size_t(idx.normal_index)+0];
            tinyobj::real_t ny = attrib.normals[3*size_t(idx.normal_index)+1];
            tinyobj::real_t nz = attrib.normals[3*size_t(idx.normal_index)+2];
        }

        // Optional: vertex colors
        // tinyobj::real_t red   = attrib.colors[3*size_t(idx.vertex_index)+0];
        // tinyobj::real_t green = attrib.colors[3*size_t(idx.vertex_index)+1];
        // tinyobj::real_t blue  = attrib.colors[3*size_t(idx.vertex_index)+2];
        }
        index_offset += fv;

        // per-face material
        shapes[s].mesh.material_ids[f];
    }
    }

    
    // your code here, ok 
    return true;
}

bool Scene::trace (Ray r, Intersection *isect) {
    Intersection curr_isect;
    bool intersection = false;    
    
    if (numPrimitives==0) return false;
    
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
    return intersection;
}

// checks whether a point on a light source (distance maxL) is visible
bool Scene::visibility (Ray s, const float maxL) {
    bool visible = true;
    Intersection curr_isect;
    
    if (numPrimitives==0) return true;
    
    // iterate over all primitives while visible
    for (auto prim_itr = prims.begin() ; prim_itr != prims.end() && visible ; prim_itr++) {
        if ((*prim_itr)->g->intersect(s, &curr_isect)) {
            if (curr_isect.depth < maxL) {
                visible = false;
            }
        }
    }
    return visible;
}
