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
    AmbientLight ambient(RGB(0.9,0.9,0.9));
    scene.lights.push_back(&ambient);
    scene.numLights++;

    // success = scene.Load("./VI-RT/Scene/tinyobjloader/models/cornell_box.obj");
    success = scene.Load("./VI-RT/Scene/tinyobjloader/models/debug.txt");
    
    if (!success) {
        std::cout << "ERROR!! :o\n";
        return 1;
    }
    std::cout << std::endl;
    std::cout << "Scene Load: SUCCESS!! :-)\n";
    scene.printSummary();
    std::cout << std::endl;
        
    // Image resolution
    const int W= 640;
    const int H= 480;
    
    img = new ImagePPM(W,H);
    
    // Camera parameters
    // const Point Eye ={0,0,0}, At={0,0,1}; 
    // const Point Eye ={280,275,-330}, At={280,265,0};
    const Point Eye ={0,125,-200}, At={0,125,0}; // debug
    const Vector Up={0,1,0};
    const float fovW = 90.f;
    const float fovH = fovW * (float)H/(float)W;  // in degrees
    const float fovWrad = fovW*3.14f/180.f, fovHrad = fovH*3.14f/180.f;    // to radians
    cam = new Perspective(Eye, At, Up, W, H, fovWrad, fovHrad);

    cam->Information();
    std::cout << std::endl;
    
    // create the shader
    RGB background(0.05, 0.05, 0.55); // Azul
    shd = new AmbientShader(&scene, background);
    // declare the renderer
    // int spp=64;
    int spp=1;     // samples per pixel
    StandardRenderer myRender (cam, &scene, img, shd, spp);


    // const float jitterAmount = 0.5f; 
    // // Allocate memory for cam_jitter array
    // float* cam_jitter = new float[W * H * 2]; // 2 values (x and y) per pixel

    // // Generate jittered values
    // for (int y = 0; y < H; ++y) {
    //     for (int x = 0; x < W; ++x) {
    //         // Convert pixel coordinates to [-0.5, 0.5] range
    //         float jitterX = ((float)x + 0.5f) / W - 0.5f;
    //         float jitterY = ((float)y + 0.5f) / H - 0.5f;

    //         // Apply jitter amount
    //         jitterX *= jitterAmount;
    //         jitterY *= jitterAmount;

    //         // Store jitter values in the array
    //         cam_jitter[(y * W + x) * 2] = jitterX;   // x-direction jitter
    //         cam_jitter[(y * W + x) * 2 + 1] = jitterY; // y-direction jitter
    //     }
    // }

    // render
    start = clock();
    // myRender.Render(cam_jitter);
    myRender.Render();
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    // save the image
    img->Save("MyImage.ppm");
    
    fprintf (stdout, "Rendering time = %.3lf secs\n\n", cpu_time_used);
    
    std::cout << "That's all, folks!" << std::endl;
    return 0;
}
