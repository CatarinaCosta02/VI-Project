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


int main(int argc, const char * argv[]) {
    Scene scene;
    Perspective *cam; // Camera
    ImagePPM *img;    // Image
    Shader *shd;
    bool success;
    clock_t start, end;
    double cpu_time_used;
    
    // add an ambient light to the scene
    // AmbientLight ambient(RGB(0.9f,0.9f,0.9f));
    // scene.lights.push_back(&ambient);
    // scene.numLights++;

    success = scene.Load("./VI-RT/Scene/tinyobjloader/models/cornell_box_VI.obj");
    // success = scene.Load("./VI-RT/Scene/tinyobjloader/models/debug.txt");
    
    if (!success) {
        std::cout << "ERROR!! :o\n";
        return 1;
    }
    std::cout << std::endl;
    std::cout << "Scene Load: SUCCESS!! :-)\n";
    scene.printSummary();
    std::cout << std::endl;
        
    // Image resolution
    const int W= 512;
    const int H= 512;
    
    img = new ImagePPM(W,H);
    
    // Camera parameters
    // const Point Eye ={0,0,0}, At={0,0,1}; 
    const Point Eye ={280,275,-330}, At={280,265,0};
    // const Point Eye ={0,125,-100}, At={0,125,0}; // debug
    const Vector Up={0,-1,0};
    const float fovW = 90.f;
    const float fovH = fovW * (float)H/(float)W;  // in degrees
    const float fovWrad = fovW*3.14f/180.f, fovHrad = fovH*3.14f/180.f;    // to radians
    cam = new Perspective(Eye, At, Up, W, H, fovWrad, fovHrad);

    cam->Information();
    std::cout << std::endl;
    
    // create the shader
    // RGB background(0.5, 0.05, 0.5); // roxo
    // shd = new AmbientShader(&scene, background);

// add an ambient light to the scene
    AmbientLight *ambient = new AmbientLight(RGB(0.05,0.05,0.05));
    scene.lights.push_back(ambient);
    scene.numLights++;

// add a point light to the scene
    PointLight *pl1 = new PointLight(RGB(0.65,0.65,0.65), Point(288,508,282));
    scene.lights.push_back(pl1);
    scene.numLights++;

    
    // declare the renderer
    // int spp=64;
    int spp=1;     // samples per pixel
    StandardRenderer myRender (cam, &scene, img, shd, spp);

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
