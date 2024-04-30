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

    success = scene.Load("./VI-RT/Scene/tinyobjloader/models/cornell_box_VI.obj");
    if (!success) {
        std::cout << "ERROR!! :o\n";
        return 1;
    }
    std::cout << "Scene Load: SUCCESS!! :-)\n";

    // // Ambient light
    // AmbientLight *ambient = new AmbientLight(RGB(0.2f, 0.2f, 0.2f));
    // scene.lights.push_back(ambient);
    // scene.numLights++;

    // Point light
    PointLight *pl1 = new PointLight(RGB(0.75, 0.75, 0.75), Point(288, 508, 282));
    scene.lights.push_back(pl1);
    scene.numLights++;
    

    // Add ceiling lights (assuming ceiling positions)
    for (int i = 0; i < 4; i++) {
        PointLight *ceilingLight = new PointLight(RGB(0.65, 0.65, 0.65), Point(150 * i, 500, 150 * i));
        scene.lights.push_back(ceilingLight);
        scene.numLights++;
    }
    std::cout << "Point Light: SUCCESS!! :-)\n";

    scene.printSummary();
    std::cout << std::endl;

    // Image resolution
    const int W = 512;
    const int H = 512;
    img = new ImagePPM(W, H);

    // Camera parameters
    const Point Eye = {280, 275, -330}, At = {280, 265, 0};
    const Vector Up = {0, -1, 0};
    const float fovW = 90.f;
    const float fovH = fovW * (float)H / (float)W; // in degrees
    const float fovWrad = fovW * 3.14f / 180.f, fovHrad = fovH * 3.14f / 180.f; // to radians
    cam = new Perspective(Eye, At, Up, W, H, fovWrad, fovHrad);

    cam->Information();
    std::cout << std::endl;

    
    // Create the shader
    RGB background(0.5, 0.05, 0.5); // Dark purple background
    shd = new WhittedShader(&scene, background);

    // Declare the renderer with increased samples per pixel
    int spp = 32; // increased from 1 for better quality
    StandardRenderer myRender(cam, &scene, img, shd, spp);

    // Render
    start = clock();
    myRender.Render();
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    // Save the image
    img->Save("MyImage.ppm");

    fprintf(stdout, "Rendering time = %.3lf secs\n\n", cpu_time_used);

    std::cout << "That's all, folks!" << std::endl;
    return 0;
}
