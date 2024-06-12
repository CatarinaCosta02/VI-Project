//
//  main.cpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 30/01/2023.
//

#include <iostream>
#include "scene.hpp"
#include "perspective.hpp"
#include "StandardRenderer.hpp"
#include "ImagePPM.hpp"
#include "AmbientShader.hpp"
#include "WhittedShader.hpp"
#include "DistributedShader.hpp"
#include "PathTracerShader.hpp"
#include "AmbientLight.hpp"
#include "PointLight.hpp"
#include "AreaLight.hpp"

#include <time.h>

void addSquareLight(Point p, float size, RGB intensity, std::vector <Light *> *lights,int *num) {
    Point v1 = Point(p.X - size, p.Y, p.Z - size);
    Point v2 = Point(p.X + size, p.Y, p.Z - size);
    Point v3 = Point(p.X + size, p.Y, p.Z + size);
    Point v4 = Point(p.X - size, p.Y, p.Z + size);
    Vector ex = v2.vec2point(v1);
    Vector fx = v3.vec2point(v1);
    Vector n = ex.cross(fx);
    n.normalize();


    AreaLight* l1 = new AreaLight(intensity, v1, v2, v3, n);
    AreaLight* l2 = new AreaLight(intensity, v1, v3, v4, n);
    lights->push_back(l1);
    lights->push_back(l2);
    *num += 2;
}

// Function to add multiple point lights to the scene
// void addPointLights(Scene &scene, int numLights) {
//     RGB lightColor(0.85f, 0.85f, 0.85f);

//     Point basePosition(288,508,282);
//     int spacing = 5;

//     for (int i = 0; i < numLights; ++i) {
//         Point position(basePosition.X + i * spacing, basePosition.Y, basePosition.Z + i * spacing);
//         PointLight *pointLight = new PointLight(lightColor, position);
//         scene.lights.push_back(pointLight);
//         scene.numLights++;
//     }
// }



int main(int argc, const char * argv[]) {
    Scene scene;
    Perspective *cam; // Camera
    ImagePPM *img;    // Image
    Shader *shd;
    bool success;
    clock_t start, end;
    double cpu_time_used;

    // success = scene.Load("../../VI-RT/Scene/tinyobjloader/models/cornell_box_VI.obj"); // LINUX
    success = scene.Load("./VI-RT/Scene/tinyobjloader/models/cornell_box_VI.obj");
    // success = scene.Load("./VI-RT/Scene/tinyobjloader/models/debug.txt");
    
    if (!success) {
        std::cout << "ERROR!! :o\n";
        return 1;
    }
    std::cout << std::endl;
    std::cout << "Scene Load: SUCCESS!! :-)\n";


    // create the shader
    //RGB background(0.5, 0.05, 0.5); // roxo
    //shd = new DistributedShader(&scene, background);

    // add an ambient light to the scene
    AmbientLight *ambient = new AmbientLight(RGB(0.1,0.1,0.1));
    scene.lights.push_back(ambient);
    scene.numLights++;


    // Modifiquem aqui para adicionar quanta pointLights queremos
    //addPointLights(scene, 8);
    //PointLight *pl1 = new PointLight(RGB(1, 1, 1), Point(288, 508, 282));
    //scene.lights.push_back(pl1);
    //scene.numLights++;


    
    
    // Modifiquem aqui para adicionar quantas areaLights quiserem
    // luz do centro
    addSquareLight(Point(288, 547.99, 288), 60, RGB(0.6f, 0.6f, 0.6f),  &(scene.lights), &(scene.numLights));
    // // luzes dos cantos
    //addSquareLight(Point(110, 548, 110), 30, RGB(0.65, 0.65, 0.65),  &(scene.lights), &(scene.numLights));
    //addSquareLight(Point(450, 548, 450), 30, RGB(0.65, 0.65, 0.65),  &(scene.lights), &(scene.numLights));
    //addSquareLight(Point(110, 548, 450), 30, RGB(0.65, 0.65, 0.65),  &(scene.lights), &(scene.numLights));
    //addSquareLight(Point(450, 548, 110), 30, RGB(0.65, 0.65, 0.65),  &(scene.lights), &(scene.numLights));
    

    for (auto l = scene.lights.begin() ; l != scene.lights.end() ; l++) {
        if ((*l)->type == AMBIENT_LIGHT) {
            std::cout << "AmbientLight guardada" << std::endl;
        }
        if ((*l)->type == POINT_LIGHT) {
            std::cout << "PointLight guardada" << std::endl;
        }
        if ((*l)->type == AREA_LIGHT) {
            std::cout << "AreaLight guardada" << std::endl;
        }
    }
    

    scene.printSummary();
    std::cout << std::endl;
        
    // Image resolution
    const int W= 512;
    const int H= 512;
    
    img = new ImagePPM(W,H);
    
    // Camera parameters
    const Point Eye ={280,275,-330}, At={280,265,0};
    const Vector Up={0,-1,0};
    const float fovW = 90.f;
    const float fovH = fovW * (float)H/(float)W;  // in degrees
    const float fovWrad = fovW*3.14f/180.f, fovHrad = fovH*3.14f/180.f;    // to radians
    cam = new Perspective(Eye, At, Up, W, H, fovWrad, fovHrad);

    cam->Information();
    std::cout << std::endl;

    RGB background(0.2, 0.2, 0.2); // cinza forte

    // AmbienteShader
    // shd = new AmbientShader(&scene, background);

    // DistributedShader
    // shd = new DistributedShader(&scene, background);
    
    // PathTracerShader
    shd = new PathTracerShader(&scene, background);
    

    
    
    // declare the renderer
    // int spp=64;
    int spp=120;     // samples per pixel
    StandardRenderer myRender(cam, &scene, img, shd, spp);

    // sem spp

    // render
    start = clock();
    myRender.Render();
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    // save the image
    img->Save("MyImage.ppm");
    
    fprintf (stdout, "Rendering time = %.3lf secs\n\n", cpu_time_used);
    
    std::cout << "That's all, folks!" << std::endl;
    return 0;
}